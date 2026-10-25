# Tests and Build Checks

Run the repository checks from the project root in a Bash environment with GCC installed:

```bash
bash scripts/compile_all.sh
bash scripts/test_examples.sh
```

- `compile_all.sh` compiles every C source file separately using C11, `-Wall -Wextra -Wpedantic -Werror`, POSIX declarations, pthread support, and the math library. Any compilation warning or failure fails the check.
- `test_examples.sh` compiles and runs representative examples and checks selected output lines against known results.
- `test_aho_corasick_array.c` and `test_aho_corasick_trie.c` exercise invalid input, empty and overlong patterns, duplicate handling, repeated builds, and capacity behavior.
- `test_kmp_search.c` checks invalid inputs and overlapping matches.
- `test_trie_autocomplete.c` checks invalid words and prefixes, maximum word length, and duplicate insertion.
- `test_fenwick_tree.c` checks invalid sizes and indices, range boundaries, and the zero-index update case.

## Smoke-test coverage

| Example | Built-in sample | Expected result |
|---|---|---|
| Fixed-array Aho–Corasick | Patterns `he`, `she`, `his`, `hers`; text `ushers` | `she` and `he` end at index 3; `hers` ends at index 5 |
| Pointer-trie Aho–Corasick | Same patterns; text `ahishers` | `his` starts at index 1; `she` at 3; `he` and `hers` at 4 |
| BST insertion/search | Insert `50, 30, 70, 20, 40, 60, 80` | Inorder traversal: `20 30 40 50 60 70 80`; search finds 40 and does not find 99 |
| BST deletion | Delete 20, then 50 from the sample BST | Final traversal: `30 40 60 70 80` |
| Dinic maximum flow | The directed capacity network in `algorithms/graphs/dinic_max_flow.c` | Maximum flow: `20` |
| KMP search | Text `ababcabcabababd`; patterns `ababd` and `ab` | Match indices `10`; then `0, 2, 5, 8, 10, 12` |
| Prim's MST (heap) | Adjacency-list sample graph | Total MST weight: `19` |
| Prim's MST (matrix) | Adjacency-matrix sample graph | Total MST weight: `17` |
| 0/1 knapsack | Three items, capacity `50` | Maximum value: `220` |
| Fenwick tree | Prefix and range queries, followed by an update | Prefix `15`, range `14`, updated range `20`, updated prefix `21` |
| Trie autocomplete | Built-in dictionary and several prefixes | Suggestions include `category`; unknown prefix `xyz` has no suggestions |

These checks are a useful baseline, not complete correctness proofs. They do not replace broad property-based testing, sanitizer runs, memory-safety checks, or concurrency stress tests. Many examples still use fixed sample data, and the systems experiments need additional review before reuse in real applications.
