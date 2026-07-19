# C Systems

C and assembly projects covering machine simulation, graphs, process execution, and low-level data structures.

Technologies: C · Assembly · Unix

## Highlights

- Instruction encoding and a small machine-simulation workflow.
- Graph utilities (including name-list formatting helper).
- Process running, string tokenizing, and C/assembly examples.

## Projects

| Project | Location |
|---|---|
| Instruction Encoding | projects/instruction-encoding |
| Machine Simulator | projects/machine-simulator |
| Graph ADT | projects/graph-adt |
| Process Runner | projects/process-runner |
| Line Checker | projects/line-checker |
| C and Assembly Examples | projects/c-and-assembly-examples |

## Quick start

These examples compile on a standard Linux or macOS toolchain with a C99 (or newer) compiler.

- Line checker (standalone executable):
  - cc -std=c99 -Wall -Wextra -O2 -o linecheck projects/line-checker/linecheck.c
  - Usage: cat file.txt | ./linecheck

- Process runner (build + test command executor):
  - cc -std=c99 -Wall -Wextra -O2 -o process-runner projects/process-runner/src/process_runner.c
  - Usage: ./process-runner build-commands.txt test-commands.txt

- Instruction encoding (compile as an object for use in your program):
  - cc -std=c99 -Wall -Wextra -O2 -I projects/instruction-encoding/include \
       -c projects/instruction-encoding/src/machine.c -o ie_machine.o

- Machine simulator core (compile as objects for linking into your program):
  - cc -std=c99 -Wall -Wextra -O2 -I projects/machine-simulator/include \
       -c projects/machine-simulator/src/machine.c -o ms_machine.o
  - cc -std=c99 -Wall -Wextra -O2 -I projects/machine-simulator/include \
       -c projects/machine-simulator/src/interpreter.c -o ms_interpreter.o

- Graph ADT:
  - The graph sources and the name-list formatter live under projects/graph-adt. Include headers from projects/graph-adt/include in your own program and compile the needed source files alongside it.

## License and attribution

Use and redistribution are governed by the repository's LICENSE.

This public repository presents independently reconstructed modules intended for portfolio review. Earlier coursework snapshots and provenance materials are retained in a private archive and are not published.
