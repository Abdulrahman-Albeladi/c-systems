# systems-c-reconstructions

Recovered C systems-programming work reconstructed from course and research files associated with 216. This repository preserves implementation material that can be reviewed independently while keeping reconstruction boundaries, licensing questions, and validation gaps visible.

The repository contains both recovered project directories (`projects/project1` through `projects/project7`) and reorganized project directories intended to provide clearer source/include layouts for selected components. The reorganized directories are not presented as independent reimplementations unless their source history establishes that distinction.

## Scope

The recovered material covers:

- line-oriented C input checking;
- instruction and machine-state encoding support;
- a small machine simulator with assembler/interpreter interfaces;
- graph abstract data type implementations and name-list formatting;
- a process-runner component with string splitting support;
- assembly and C program pairs used with the machine-related work.

See [docs/reconstruction-boundaries.md](docs/reconstruction-boundaries.md) for repository-editing and reconstruction constraints. See [LICENSE_REVIEW.md](LICENSE_REVIEW.md) before reusing code whose origin or redistribution status is not yet resolved.

## Repository layout

```text
projects/
  project1/                Recovered line-checking source
  project2/                Recovered machine/instruction-encoding source
  project3/                Recovered assembler/interpreter/machine project
  project4/                Recovered graph implementation
  project5/                Recovered graph and name-list formatting project
  project6/                Recovered process-runner project and harness material
  project7/                Recovered C and assembly program pairs
  instruction-encoding/    Reorganized instruction-encoding source and headers
  machine-simulator/       Reorganized machine simulator source and headers
  graph-adt/               Reorganized graph ADT source and headers
  process-runner/          Reorganized process-runner source and headers
```

Project-specific details, known relationships, and limitations are listed in the [project index](#project-index).

## Setup

A C compiler and `make` are required for the recovered directories that provide Makefiles. No root-level build interface is established by the recovered file set.

```sh
git clone <repository-url>
cd systems-c-reconstructions

# Inspect the recovered build definitions before running them.
make -C projects/project3
make -C projects/project4
make -C projects/project5
make -C projects/project6
```

The commands above are candidate build commands, not recorded successful builds. Their availability follows from the presence of project-local Makefiles; target names, compiler assumptions, and runtime behavior must be checked from those files in the checkout.

The reorganized directories currently contain source and headers but no listed Makefiles. Compile or integrate them with explicit include paths appropriate to the component being examined. For example, syntax-only compilation can be attempted as follows:

```sh
cc -std=c11 -Wall -Wextra -Iprojects/instruction-encoding/include \
  -fsyntax-only projects/instruction-encoding/src/machine.c
cc -std=c11 -Wall -Wextra -Iprojects/machine-simulator/include \
  -fsyntax-only projects/machine-simulator/src/machine.c \
  projects/machine-simulator/src/interpreter.c
cc -std=c11 -Wall -Wextra -Iprojects/graph-adt/include \
  -fsyntax-only projects/graph-adt/src/graph.c \
  projects/graph-adt/src/name_list_format.c
cc -std=c11 -Wall -Wextra -Iprojects/process-runner/include \
  -fsyntax-only projects/process-runner/src/process_runner.c
```

These are recommended checks, not evidence that the code has compiled cleanly under a particular toolchain.

## Validation status

No successful build, test, sanitizer, or continuous-integration result is recorded here. The repository includes `.github/workflows/ci.yml`, but the file list alone does not establish which jobs run or whether they have succeeded.

Several recovered directories contain files named `test`, `public`, `ourtest`, `studentcode`, `studentprogram`, or `memory-checking`. Their presence indicates recovered harness, sample, or support material; it does not establish coverage, expected behavior, or a passing test result. In particular, `projects/project6` contains a mixture of implementation files, headers, test-like sources, and `.input` fixtures that should be reviewed before being treated as a public test suite.

Recommended validation is listed in the JSON `recommended_tests` and `validation_commands` fields accompanying this README.

## Data and external requirements

No private dataset, credential, network service, or database is identified by the recovered file list. The checked-in `.input` files under `projects/project6` are local input fixtures, not evidence of access to external or private data.

Before publishing additional fixtures, traces, grading outputs, or course materials, verify that they do not contain private data, protected assignment content, or material distributed under restrictions. Do not add secrets, machine-specific paths, or unreviewed external inputs to the repository.

## Limitations

- Recovery is constrained to the files currently present; missing prompts, specifications, expected outputs, and build logs are not reconstructed as facts.
- The precise relationship between each recovered `projectN` directory and its reorganized counterpart requires source-level comparison.
- Interfaces may reflect course-provided headers or frameworks. Their provenance and redistribution status require review where not already documented.
- The repository does not claim portability across compilers, operating systems, or C language modes.
- `LICENSE_REVIEW.md` signals that licensing and attribution review remains part of publication readiness.

## Provenance and attribution

This repository is a reconstruction from university and research files associated with 216. It preserves demonstrated implementation logic rather than recreating assignment prompts, grading rubrics, hidden tests, or unobserved results.

Files and interfaces that originated in a course framework, starter code, or other external material should retain their original attribution where known. If a source file cannot be confidently attributed or cleared for redistribution, treat it as under review and follow [LICENSE_REVIEW.md](LICENSE_REVIEW.md). Contributions should follow [CONTRIBUTING.md](CONTRIBUTING.md), and security reports should follow [SECURITY.md](SECURITY.md).

## Project index

| Project | Primary location | Summary | Build/validation position |
| --- | --- | --- | --- |
| Line checker | `projects/project1` | One recovered C source file, `linecheck.c`. | No Makefile or test fixture is listed; behavior requires source review. |
| Instruction encoding | `projects/project2`, `projects/instruction-encoding` | Machine-related implementation and interface files. | A recovered `test.c` exists in `project2`; no recorded result is available. |
| Machine simulator | `projects/project3`, `projects/machine-simulator` | Machine state, interpreter, and assembler interfaces; recovered example/program material. | `project3` has a Makefile. Build and execution are unverified. |
| Graph ADT | `projects/project4`, `projects/project5`, `projects/graph-adt` | Graph implementations, public/internal headers, and name-list formatting support. | `project4` and `project5` have Makefiles. No passing result is recorded. |
| Process runner | `projects/project6`, `projects/process-runner` | Process-runner-related implementation, internal/public interfaces, and splitting support. | `project6` has a Makefile and recovered harness-like files. No result is recorded. |
| Machine programs | `projects/project7` | Recovered paired `.c` and `.s` programs. | No Makefile is listed; execution environment and expected outputs are unresolved. |

<!-- portfolio-public-release-license:start -->

## License and public-release status

This repository is published under an all-rights-reserved
portfolio license. Viewing the repository does not grant permission to reuse its code,
documentation, datasets, or assets. Third-party and collaborator materials retain
their original rights.

Before changing visibility from private to public, the owner must complete the
ownership checklist in `LICENSE_REVIEW.md`.

<!-- portfolio-public-release-license:end -->

<!-- release-license:start -->

## License and public-release status

This repository uses an all-rights-reserved portfolio license. Review `LICENSE_REVIEW.md` and `THIRD_PARTY_NOTICES.md` before changing visibility to public.

<!-- release-license:end -->
