#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Independent reconstruction of a small process-runner utility.
 *
 * Concept preserved from the source material:
 * - load command lines from text files
 * - run a sequence of build commands
 * - if build succeeds, run a sequence of test commands
 * - support simple trailing redirection in test commands:
 *     command < infile
 *     command > outfile
 *     command < infile > outfile
 *
 * This implementation intentionally avoids assignment-specific headers,
 * interfaces, and scaffolding.
 */

typedef struct {
    char **items;
    size_t count;
} CommandList;

typedef struct {
    CommandList build;
    CommandList test;
} CommandSet;

typedef struct {
    char **argv;
    size_t argc;
    char *input_path;
    char *output_path;
} ParsedCommand;

static void free_command_list(CommandList *list) {
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->count; ++i) {
        free(list->items[i]);
    }

    free(list->items);
    list->items = NULL;
    list->count = 0;
}

static void free_command_set(CommandSet *set) {
    if (set == NULL) {
        return;
    }

    free_command_list(&set->build);
    free_command_list(&set->test);
}

static char *duplicate_string(const char *src) {
    size_t len;
    char *copy;

    if (src == NULL) {
        return NULL;
    }

    len = strlen(src);
    copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, src, len + 1);
    return copy;
}

static void trim_trailing_newlines(char *line) {
    size_t len;

    if (line == NULL) {
        return;
    }

    len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[len - 1] = '\0';
        --len;
    }
}

static int append_line(CommandList *list, const char *line) {
    char **resized;
    char *copy;

    if (list == NULL || line == NULL) {
        return -1;
    }

    copy = duplicate_string(line);
    if (copy == NULL) {
        return -1;
    }

    resized = realloc(list->items, (list->count + 1) * sizeof(*list->items));
    if (resized == NULL) {
        free(copy);
        return -1;
    }

    list->items = resized;
    list->items[list->count] = copy;
    list->count += 1;
    return 0;
}

static int load_command_file(const char *path, CommandList *out) {
    FILE *fp;
    char buffer[1024];

    if (path == NULL || out == NULL) {
        return -1;
    }

    fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        trim_trailing_newlines(buffer);

        if (buffer[0] == '\0') {
            continue;
        }

        if (append_line(out, buffer) != 0) {
            fclose(fp);
            return -1;
        }
    }

    if (ferror(fp)) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

static int load_command_set(const char *build_path, const char *test_path, CommandSet *out) {
    if (build_path == NULL || test_path == NULL || out == NULL) {
        return -1;
    }

    memset(out, 0, sizeof(*out));

    if (load_command_file(build_path, &out->build) != 0) {
        free_command_set(out);
        return -1;
    }

    if (load_command_file(test_path, &out->test) != 0) {
        free_command_set(out);
        return -1;
    }

    return 0;
}

static void free_parsed_command(ParsedCommand *cmd) {
    size_t i;

    if (cmd == NULL) {
        return;
    }

    if (cmd->argv != NULL) {
        for (i = 0; i < cmd->argc; ++i) {
            free(cmd->argv[i]);
        }
    }

    free(cmd->argv);
    free(cmd->input_path);
    free(cmd->output_path);

    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->input_path = NULL;
    cmd->output_path = NULL;
}

static int push_token(char ***tokens, size_t *count, const char *start, size_t len) {
    char **resized;
    char *copy;

    copy = malloc(len + 1);
    if (copy == NULL) {
        return -1;
    }

    memcpy(copy, start, len);
    copy[len] = '\0';

    resized = realloc(*tokens, (*count + 1) * sizeof(**tokens));
    if (resized == NULL) {
        free(copy);
        return -1;
    }

    *tokens = resized;
    (*tokens)[*count] = copy;
    *count += 1;
    return 0;
}

static int tokenize_simple(const char *line, char ***tokens_out, size_t *count_out) {
    const char *p;
    char **tokens = NULL;
    size_t count = 0;

    if (line == NULL || tokens_out == NULL || count_out == NULL) {
        return -1;
    }

    p = line;
    while (*p != '\0') {
        const char *start;

        while (*p == ' ' || *p == '\t') {
            ++p;
        }

        if (*p == '\0') {
            break;
        }

        if (*p == '<' || *p == '>') {
            if (push_token(&tokens, &count, p, 1) != 0) {
                goto fail;
            }
            ++p;
            continue;
        }

        start = p;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '<' && *p != '>') {
            ++p;
        }

        if (push_token(&tokens, &count, start, (size_t)(p - start)) != 0) {
            goto fail;
        }
    }

    *tokens_out = tokens;
    *count_out = count;
    return 0;

fail:
    if (tokens != NULL) {
        size_t i;
        for (i = 0; i < count; ++i) {
            free(tokens[i]);
        }
        free(tokens);
    }
    return -1;
}

static int parse_command_line(const char *line, bool allow_redirection, ParsedCommand *out) {
    char **tokens = NULL;
    size_t token_count = 0;
    size_t i;
    size_t argv_count = 0;

    if (line == NULL || out == NULL) {
        return -1;
    }

    memset(out, 0, sizeof(*out));

    if (tokenize_simple(line, &tokens, &token_count) != 0) {
        return -1;
    }

    if (token_count == 0) {
        free(tokens);
        return -1;
    }

    out->argv = calloc(token_count + 1, sizeof(*out->argv));
    if (out->argv == NULL) {
        goto fail;
    }

    for (i = 0; i < token_count; ++i) {
        if (allow_redirection && strcmp(tokens[i], "<") == 0) {
            if (i + 1 >= token_count || out->input_path != NULL) {
                goto fail;
            }
            out->input_path = duplicate_string(tokens[++i]);
            if (out->input_path == NULL) {
                goto fail;
            }
            continue;
        }

        if (allow_redirection && strcmp(tokens[i], ">") == 0) {
            if (i + 1 >= token_count || out->output_path != NULL) {
                goto fail;
            }
            out->output_path = duplicate_string(tokens[++i]);
            if (out->output_path == NULL) {
                goto fail;
            }
            continue;
        }

        out->argv[argv_count] = duplicate_string(tokens[i]);
        if (out->argv[argv_count] == NULL) {
            goto fail;
        }
        ++argv_count;
    }

    out->argc = argv_count;
    out->argv[argv_count] = NULL;

    for (i = 0; i < token_count; ++i) {
        free(tokens[i]);
    }
    free(tokens);

    return argv_count > 0 ? 0 : -1;

fail:
    for (i = 0; i < token_count; ++i) {
        free(tokens[i]);
    }
    free(tokens);
    free_parsed_command(out);
    return -1;
}

static void redirect_stderr_to_devnull(void) {
    int fd = open("/dev/null", O_WRONLY);
    if (fd >= 0) {
        (void)dup2(fd, STDERR_FILENO);
        close(fd);
    }
}

static int apply_redirection(const ParsedCommand *cmd) {
    int fd;

    if (cmd->input_path != NULL) {
        fd = open(cmd->input_path, O_RDONLY);
        if (fd < 0) {
            return -1;
        }
        if (dup2(fd, STDIN_FILENO) < 0) {
            close(fd);
            return -1;
        }
        close(fd);
    }

    if (cmd->output_path != NULL) {
        fd = open(cmd->output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            return -1;
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            close(fd);
            return -1;
        }
        close(fd);
    }

    return 0;
}

static int run_single_command(const char *line, bool allow_redirection) {
    pid_t pid;
    int status = 0;
    ParsedCommand cmd;

    if (parse_command_line(line, allow_redirection, &cmd) != 0) {
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        free_parsed_command(&cmd);
        return -1;
    }

    if (pid == 0) {
        if (allow_redirection && apply_redirection(&cmd) != 0) {
            _exit(127);
        }

        redirect_stderr_to_devnull();
        execvp(cmd.argv[0], cmd.argv);
        _exit(127);
    }

    free_parsed_command(&cmd);

    if (waitpid(pid, &status, 0) < 0) {
        return -1;
    }

    if (!WIFEXITED(status)) {
        return -1;
    }

    return WEXITSTATUS(status);
}

static int run_build_commands(const CommandList *build) {
    size_t i;

    if (build == NULL) {
        return 0;
    }

    for (i = 0; i < build->count; ++i) {
        int exit_code = run_single_command(build->items[i], false);
        if (exit_code != 0) {
            return 0;
        }
    }

    return 1;
}

static int run_test_commands(const CommandSet *set) {
    size_t i;
    int passed = 0;

    if (set == NULL) {
        return 0;
    }

    if (!run_build_commands(&set->build)) {
        return 0;
    }

    for (i = 0; i < set->test.count; ++i) {
        int exit_code = run_single_command(set->test.items[i], true);
        if (exit_code == 0) {
            ++passed;
        }
    }

    return passed;
}

int main(int argc, char **argv) {
    CommandSet commands;
    int passed;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <build-commands.txt> <test-commands.txt>\n", argv[0]);
        return 2;
    }

    if (load_command_set(argv[1], argv[2], &commands) != 0) {
        fprintf(stderr, "failed to load command files\n");
        return 1;
    }

    passed = run_test_commands(&commands);
    printf("%d\n", passed);

    free_command_set(&commands);
    return 0;
}
