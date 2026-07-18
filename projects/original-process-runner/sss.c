#define _POSIX_C_SOURCE 200809L

#include "sss.h"
#include "split.h"
#include "memory-checking.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void free_words(char **words);
static int load_commands(const char *path, char ***commands, int *count);
static int wait_for_child(pid_t pid, int *status);
static void suppress_stderr(void);
static int configure_redirection(char **words);

Commands read_commands(const char *compile_cmds, const char *test_cmds) {
  Commands commands = {0};

  if (compile_cmds == NULL || test_cmds == NULL) {
    return commands;
  }

  if (!load_commands(compile_cmds, &commands.compile_arr, &commands.comp_num)
      || !load_commands(test_cmds, &commands.test_arr, &commands.test_num)) {
    clear_commands(&commands);
  }

  return commands;
}

void clear_commands(Commands *const commands) {
  int i;

  if (commands == NULL) {
    return;
  }

  for (i = 0; i < commands->comp_num; i++) {
    free(commands->compile_arr[i]);
  }
  free(commands->compile_arr);

  for (i = 0; i < commands->test_num; i++) {
    free(commands->test_arr[i]);
  }
  free(commands->test_arr);

  commands->compile_arr = NULL;
  commands->test_arr = NULL;
  commands->comp_num = 0;
  commands->test_num = 0;
}

int compile_program(Commands commands) {
  int i;

  for (i = 0; i < commands.comp_num; i++) {
    pid_t pid = fork();
    int status;

    if (pid < 0) {
      return 0;
    }

    if (pid == 0) {
      char **words = split(commands.compile_arr[i]);

      suppress_stderr();
      if (words == NULL || words[0] == NULL) {
        free_words(words);
        _exit(127);
      }

      execvp(words[0], words);
      free_words(words);
      _exit(127);
    }

    if (!wait_for_child(pid, &status)
        || !WIFEXITED(status)
        || WEXITSTATUS(status) != 0) {
      return 0;
    }
  }

  return 1;
}

int test_program(Commands commands) {
  int i;
  int passed = 0;

  if (!compile_program(commands)) {
    return 0;
  }

  for (i = 0; i < commands.test_num; i++) {
    pid_t pid = fork();
    int status;

    if (pid < 0) {
      return 0;
    }

    if (pid == 0) {
      char **words = split(commands.test_arr[i]);

      suppress_stderr();
      if (words == NULL || words[0] == NULL || !configure_redirection(words)) {
        free_words(words);
        _exit(127);
      }

      execvp(words[0], words);
      free_words(words);
      _exit(127);
    }

    if (!wait_for_child(pid, &status)) {
      return 0;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      passed++;
    }
  }

  return passed;
}

static int load_commands(const char *path, char ***commands, int *count) {
  FILE *file;
  char *line = NULL;
  size_t capacity = 0;
  ssize_t length;

  file = fopen(path, "r");
  if (file == NULL) {
    return 0;
  }

  while ((length = getline(&line, &capacity, file)) != -1) {
    char *copy;
    char **expanded;

    if (*count == INT_MAX) {
      free(line);
      fclose(file);
      return 0;
    }

    copy = malloc((size_t)length + 1);
    if (copy == NULL) {
      free(line);
      fclose(file);
      return 0;
    }
    memcpy(copy, line, (size_t)length + 1);

    expanded = realloc(*commands, (size_t)(*count + 1) * sizeof(**commands));
    if (expanded == NULL) {
      free(copy);
      free(line);
      fclose(file);
      return 0;
    }

    *commands = expanded;
    (*commands)[*count] = copy;
    (*count)++;
  }

  free(line);
  if (ferror(file)) {
    fclose(file);
    return 0;
  }

  fclose(file);
  return 1;
}

static int wait_for_child(pid_t pid, int *status) {
  while (waitpid(pid, status, 0) == -1) {
    if (errno != EINTR) {
      return 0;
    }
  }
  return 1;
}

static void suppress_stderr(void) {
  int null_fd = open("/dev/null", O_WRONLY);

  if (null_fd >= 0) {
    (void)dup2(null_fd, STDERR_FILENO);
    close(null_fd);
  }
}

static int configure_redirection(char **words) {
  int count = 0;
  int input_fd;
  int output_fd;
  int cutoff;

  while (words[count] != NULL) {
    count++;
  }

  if (count > 4
      && strcmp(words[count - 4], "<") == 0
      && strcmp(words[count - 2], ">") == 0) {
    input_fd = open(words[count - 3], O_RDONLY);
    output_fd = open(words[count - 1], O_WRONLY | O_CREAT, 0666);
    cutoff = count - 4;

    if (input_fd < 0 || output_fd < 0) {
      if (input_fd >= 0) {
        close(input_fd);
      }
      if (output_fd >= 0) {
        close(output_fd);
      }
      return 0;
    }

    (void)dup2(input_fd, STDIN_FILENO);
    (void)dup2(output_fd, STDOUT_FILENO);
    close(input_fd);
    close(output_fd);
  } else if (count >= 2 && strcmp(words[count - 2], "<") == 0) {
    input_fd = open(words[count - 1], O_RDONLY);
    if (input_fd < 0) {
      return 0;
    }

    (void)dup2(input_fd, STDIN_FILENO);
    close(input_fd);
    cutoff = count - 2;
  } else if (count >= 2 && strcmp(words[count - 2], ">") == 0) {
    output_fd = open(words[count - 1], O_WRONLY | O_CREAT, 0666);
    if (output_fd < 0) {
      return 0;
    }

    (void)dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
    cutoff = count - 2;
  } else {
    return 1;
  }

  while (cutoff < count) {
    free(words[cutoff]);
    cutoff++;
  }
  words[count == 0 ? 0 : (count > 4
      && strcmp(words[count - 4], "<") == 0
      && strcmp(words[count - 2], ">") == 0 ? count - 4 : count - 2)] = NULL;

  return 1;
}

static void free_words(char **words) {
  int i;

  if (words == NULL) {
    return;
  }

  for (i = 0; words[i] != NULL; i++) {
    free(words[i]);
  }
  free(words);
}
