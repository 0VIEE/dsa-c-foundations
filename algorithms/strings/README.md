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

## Aho–Corasick examples

Both implementations now validate their inputs and check allocation or capacity limits where applicable. They accept non-empty lowercase English patterns and lowercase text only, with a maximum pattern length of 31 characters.

- The fixed-array implementation has a limit of 128 trie states and treats repeated pattern strings as one pattern.
- The pointer-trie implementation allocates nodes dynamically and can report multiple pattern IDs for the same string.

The examples remain educational demonstrations. They do not provide a Unicode-aware or case-insensitive search API, and the pointer-based example relies on callers to map pattern IDs consistently to the pattern array supplied to search. Edge-case tests cover invalid characters, empty and overlong patterns, duplicate patterns, repeated automaton construction, and the fixed-array capacity limit.

## Compile an example

From the repository root:

```bash
gcc algorithms/strings/kmp_search.c -std=c11 -Wall -Wextra -Wpedantic -o kmp_search
```

Run `./kmp_search` on Linux/macOS or `kmp_search.exe` on Windows.
