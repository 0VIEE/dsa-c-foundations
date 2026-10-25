# Tests and Build Checks

Run the repository checks from the project root:

```bash
bash scripts/compile_all.sh
bash scripts/test_examples.sh
```

- `compile_all.sh` compiles every C source file separately with GCC, C11, common warnings, POSIX declarations, pthread support, and the math library. It reports warnings and fails if any source does not compile and link as a standalone program.
- `test_examples.sh` runs targeted smoke tests for the two Aho–Corasick demonstrations, BST insertion/search/deletion, and Dinic maximum flow. It checks key matches, traversal output, and the expected maximum-flow result.

## Sample cases and expected results

These programs currently use built-in sample data rather than reading input from standard input.

| Example | Built-in sample | Expected result |
|---|---|---|
| Fixed-array Aho–Corasick | Patterns `he`, `she`, `his`, `hers`; text `ushers` | `she` and `he` end at index 3; `hers` ends at index 5 |
| Pointer-trie Aho–Corasick | Same patterns; text `ahishers` | `his` starts at index 1; `she` at 3; `he` and `hers` at 4 |
| BST insertion/search | Insert `50, 30, 70, 20, 40, 60, 80` | Inorder traversal: `20 30 40 50 60 70 80`; search finds 40 and does not find 99 |
| BST deletion | Delete 20, then 50 from the sample BST | Traversals remain sorted; final traversal: `30 40 60 70 80` |
| Dinic maximum flow | The directed capacity network defined in `algorithms/graphs/dinic_max_flow.c` | Maximum flow: `20` |

The checks are a starting point, not complete correctness proofs. They do not replace edge-case tests, memory-safety checks, or stress testing—particularly for the concurrency and allocator experiments.
