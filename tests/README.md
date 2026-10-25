# Tests and Build Checks

Run the repository checks from the project root:

```bash
bash scripts/compile_all.sh
bash scripts/test_examples.sh
```

- `compile_all.sh` compiles every tracked C example separately with GCC, C11, common warnings, POSIX declarations, pthread support, and the math library. It reports warnings and fails if any source does not compile and link as a standalone program.
- `test_examples.sh` runs targeted smoke tests for the two Aho–Corasick demonstrations and the BST insertion/search/deletion examples, and the Dinic maximum-flow example. It checks key matches, traversal output, and the expected max-flow result.

These checks are a starting point, not complete correctness proofs. They do not replace edge-case tests, memory-safety checks, or stress testing—particularly for the concurrency and allocator experiments.
