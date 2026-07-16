# Project index

## 1. Line checker

- **Locations:** `projects/project1/linecheck.c`
- **Contents:** A single recovered C implementation named `linecheck.c`.
- **Setup:** No project-local Makefile, header, sample input, or executable name is listed. Inspect the source to determine its command-line and input contract before compiling it.
- **Validation status:** No test source, expected output, or successful run is recorded in the recovered file list.
- **Private-data requirements:** None identified. Use only non-sensitive sample input when exercising line-oriented behavior.
- **Limitations:** The intended line format and error-handling requirements cannot be inferred from the filename alone.
- **Provenance:** Recovered project material associated with 216. Original assignment context is intentionally not reproduced.

## 2. Instruction encoding / machine support

- **Locations:** `projects/project2/{machine.c,machine.h,test.c}` and `projects/instruction-encoding/{include/machine.h,src/machine.c}`
- **Contents:** A recovered machine interface and implementation, plus `projects/project2/test.c`. The reorganized `instruction-encoding` directory separates the header and implementation into `include/` and `src/`.
- **Setup:** Compile the implementation with its corresponding include directory. The recovered `test.c` may depend on interface details or build assumptions that must be inspected locally.
- **Validation status:** A test-named source exists, but no test command, expected output, or passing result is established.
- **Private-data requirements:** None identified.
- **Limitations:** The exact relationship between the `project2` and `instruction-encoding` copies has not been established from the file list. Instruction-set semantics, encoded values, and boundary conditions must come from the code and available documentation rather than inferred course requirements.
- **Provenance:** Recovered 216 material. The header may contain framework-defined API material and should retain attribution where applicable.

## 3. Machine simulator

- **Locations:** `projects/project3/` and `projects/machine-simulator/`
- **Contents:**
  - Recovered files: `machine.c`, `machine.h`, `interpreter.c`, `interpreter.h`, `assembler.h`, `assembler-example.c`, `assembly-program`, and `Makefile`.
  - Reorganized files: machine, interpreter, and assembler headers under `include/`; machine and interpreter implementations under `src/`.
- **Setup:** Review `projects/project3/Makefile` for the recovered build path. The reorganized directory has no listed Makefile, so compile with `-Iprojects/machine-simulator/include` or integrate it into another build system.
- **Validation status:** `assembler-example.c` and `assembly-program` are recovered example/program material. Their presence does not show that assembly, interpretation, or machine execution has been validated.
- **Private-data requirements:** None identified. The checked-in assembly program should be treated as source input, not as a private workload.
- **Limitations:** The assembler implementation itself is not listed in either location; only `assembler.h` and recovered example material are listed. Whether assembly support is complete, externally supplied, or omitted is unresolved. Program syntax and expected output require source-level review.
- **Provenance:** Recovered 216 project material, reorganized without claiming new functionality.

## 4. Recovered graph implementation

- **Location:** `projects/project4/`
- **Contents:** `graph.c`, `graph.h`, `graph-implementation.h`, and `Makefile`.
- **Setup:** Use the project-local Makefile after reviewing its targets and compiler options.
- **Validation status:** No test sources or successful build result are listed for this directory.
- **Private-data requirements:** None identified.
- **Limitations:** The split between `graph.h` and `graph-implementation.h` suggests a public/internal distinction, but API guarantees, ownership rules, and graph invariants must be confirmed from the headers and implementation.
- **Provenance:** Recovered 216 graph project material.

## 5. Graph ADT and name-list formatting

- **Locations:** `projects/project5/` and `projects/graph-adt/`
- **Contents:**
  - Recovered files: graph source and headers, `name-list-to-string.c`, `name-list-to-string.h`, `memory-checking.h`, and `Makefile`.
  - Reorganized files: `graph.c`, `name_list_format.c`, public `graph.h`, internal `graph_internal.h`, and `name_list_format.h`.
- **Setup:** The recovered directory has a Makefile. For the reorganized source, use `-Iprojects/graph-adt/include` when compiling.
- **Validation status:** `memory-checking.h` is support material, not a recorded memory-safety result. No test execution, leak check, or passing build is documented.
- **Private-data requirements:** None identified.
- **Limitations:** The relation between `project4`, `project5`, and `graph-adt` is not fully resolved. In particular, the file list does not establish whether they are successive versions, separate assignments, or reorganized copies. Memory ownership and formatting behavior should be reviewed before reuse.
- **Provenance:** Recovered 216 graph-related material. Names have been normalized in the reorganized directory, but behavior is not claimed beyond the supplied source.

## 6. Process runner

- **Locations:** `projects/project6/` and `projects/process-runner/`
- **Contents:**
  - Recovered implementation and interfaces: `sss.c`, `sss.h`, `sss-implementation.h`, `split.h`, and `memory-checking.h`.
  - Recovered harness-like material: files named `public*.c`, `ourtest*.c`, `studentcode*.c`, `studentprogram04.c`, `header*.h`, and `public09*.input`.
  - Reorganized component: `process_runner.c`, `process_runner.h`, `process_runner_internal.h`, and `split.h`.
- **Setup:** Review and run `make -C projects/project6` only after inspecting the Makefile. The reorganized component has no listed build definition and requires explicit include paths.
- **Validation status:** The directory contains many test-like and input-like files, but no test transcript, expected outputs, or passing status is available. Names such as `public`, `studentcode`, and `ourtest` indicate recovered course-era structure rather than a curated public test suite.
- **Private-data requirements:** No private dataset or external service is identified. The `public09a.input`, `public09b.input`, and `public09c.input` files are versioned local fixtures. Any additional process arguments, environment variables, file paths, or fixtures should be checked for sensitive content before publication.
- **Limitations:** The `sss` naming and project-specific headers have not been mapped conclusively to the reorganized `process_runner` API. Process behavior can be platform-sensitive, and no portability claim is made. Recovered harness material may require attribution or removal if it is course-supplied and not cleared for redistribution.
- **Provenance:** Recovered 216 material. The reorganized directory exposes a clearer process-runner-oriented layout without asserting a complete replacement for the recovered project directory.

## 7. Machine-related C and assembly programs

- **Location:** `projects/project7/`
- **Contents:** `prog1.c`, `prog2.c`, `prog3.c`, `prog1.s`, `prog2.s`, and `prog3.s`.
- **Setup:** No Makefile is listed. Determine the required assembler, simulator, architecture, and invocation method from the program contents and related machine-project interfaces.
- **Validation status:** C/assembly pairs are present, but no expected outputs, execution traces, or successful assembly/run results are recorded.
- **Private-data requirements:** None identified.
- **Limitations:** The `.s` extension alone does not identify the target architecture or whether these programs are intended for a host assembler or the recovered simulator. Do not assume they are portable host assembly.
- **Provenance:** Recovered 216 material. The original exercise framing and grading expectations are not included.

## Repository-wide provenance and review notes

- The repository includes `LICENSE_REVIEW.md`; unresolved licensing or attribution questions should be addressed before reuse or redistribution beyond this reconstruction.
- `docs/reconstruction-boundaries.md` documents limits on what was reconstructed or inferred.
- The repository includes a CI workflow, but its existence is not evidence of successful validation.
- No source-level comparison result is provided for the recovered and reorganized directories. Any duplicate, derived, or divergent relationship should be documented only after review of the actual files.
