# Reconstruction boundaries

This document records what must be checked before this repository is treated as a portfolio artifact.

## Verified facts

From the current manifest, the repository contains C header and source files under:

- `projects/graph-adt/`
- `projects/instruction-encoding/`
- `projects/machine-simulator/`
- `projects/process-runner/`

The repository metadata describes this as a private staging repository for independently reconstructed systems projects.

The release blockers explicitly state:

- systems coursework in this area has high academic-integrity risk if published as raw submissions
- manual review is needed to ensure no assignment-specific material remains

## Required exclusions

The following material must not be published in this repository:

- assignment instructions or prompts
- autograder files
- instructor tests
- grading metadata
- submission packaging files
- copied starter code unless licensing and permission are clear
- collaborator-owned material without permission and attribution

## Manual review questions

For each project, confirm:

1. Was the implementation written independently rather than copied from a course release or prior submission?
2. Do comments, identifiers, APIs, file names, or output formats mirror assignment text closely enough to create integrity risk?
3. Are there hidden dependencies on omitted course files?
4. Is there enough standalone context to document the code without reproducing assignment wording?
5. Is public release actually useful relative to stronger portfolio projects?

## Project-specific caution

### graph-adt

Likely suitable only if the public API and examples can be described generically as a graph data structure implementation without reproducing assignment framing.

### instruction-encoding

Needs review for any assignment-specific instruction set definitions, constants, or formatting conventions that may have been inherited from coursework.

### machine-simulator

Needs review for overlap with course machine models, assembler syntax, interpreter behavior, and expected outputs.

### process-runner

Needs review for shell/process-management requirements that may map directly to a course specification.

## Publication standard

A project in this repository is ready for public release only if all of the following are true:

- the implementation is independently reconstructed
- no assignment-specific text or grading artifacts remain
- build and usage instructions are verified from the codebase
- tests are authored for the reconstructed version, not copied from course materials
- ownership and licensing are clear

Until then, keep the repository private.