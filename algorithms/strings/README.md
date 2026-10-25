# String Algorithms

Implementations exploring pattern matching, suffix-based data structures, and substring problems.

## Implementations

- [aho_corasick.c](aho_corasick.c) — fixed-array automaton with dictionary links
- [aho_corasick_trie.c](aho_corasick_trie.c) — pointer-based trie with failure links
- [kmp_search.c](kmp_search.c) — Knuth–Morris–Pratt pattern search
- [sam_distinct_substrings.c](sam_distinct_substrings.c) — distinct substrings using a suffix automaton
- [sam_queries.c](sam_queries.c) — suffix-automaton query experiment
- [suffix_array_rmq_lcp.c](suffix_array_rmq_lcp.c) — suffix array, LCP, and range-minimum query
- [suffix_automaton.c](suffix_automaton.c) — suffix automaton

The former `aho_corasick12.c` and `aho_corasick123.c` files were exact copies of the pointer-based example, so the redundant copies were removed. Their previous versions remain in Git history.

## Current limitations

Both Aho–Corasick examples are educational demonstrations, not general-purpose text-search libraries. They currently assume lowercase English letters (`a`–`z`) and do not fully validate input or capacity limits. The pointer-based version also stores a single pattern ID per terminal node, so inserting the same pattern more than once overwrites its earlier ID. These edge cases should be addressed before using either example with arbitrary input.

## Compile an example

From the repository root:

```bash
gcc algorithms/strings/kmp_search.c -std=c11 -Wall -Wextra -Wpedantic -o kmp_search
```

Run `./kmp_search` on Linux/macOS or `kmp_search.exe` on Windows.
