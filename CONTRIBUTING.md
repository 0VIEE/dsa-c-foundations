# Contributing

This repository is a collection of learning-oriented C examples. Contributions should make an example easier to understand, verify, or compare with another approach.

## Adding an example

- Put the source file in the relevant topic folder and use a descriptive filename.
- Prefer one independently compilable example per `.c` file.
- Add a short explanation of the algorithm, important assumptions, and expected input where those details are not obvious from the code.
- Include time and space complexity when they help explain the implementation.
- Avoid committing generated executables, object files, logs, or editor-specific temporary files.
- Update the relevant folder README when adding, renaming, or removing a source file.

## Before committing

Run these commands from the repository root in a Bash environment with GCC installed:

```bash
bash scripts/compile_all.sh
bash scripts/test_examples.sh
```

The compile sweep uses C11, common warning flags, and `-Werror`; all examples should compile without warnings. The smoke tests check representative outputs and selected edge cases, but they are not a substitute for reviewing the algorithm's correctness.

When changing behavior, update or add a focused test. Do not describe an implementation as production-ready unless its correctness, memory safety, and platform assumptions have been evaluated accordingly.

## Style

- Follow the repository's `.editorconfig` settings.
- Use clear names and comments that explain reasoning rather than restating each line.
- Validate input at public function boundaries when practical.
- Check allocation results before dereferencing allocated memory.
- Keep documentation accurate about fixed limits, supported character sets, and other assumptions.
