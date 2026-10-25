# String Algorithms

Implementations exploring pattern matching, suffix-based data structures, and substring problems.

## Implementations

- [aho_corasick.c](aho_corasick.c)
- [aho_corasick1.c](aho_corasick1.c)
- [aho_corasick12.c](aho_corasick12.c)
- [aho_corasick123.c](aho_corasick123.c)
- [kmp_search.c](kmp_search.c) — Knuth–Morris–Pratt pattern search
- [sam_distinct_substrings.c](sam_distinct_substrings.c) — distinct substrings using a suffix automaton
- [sam_queries.c](sam_queries.c) — suffix-automaton query experiment
- [suffix_array_rmq_lcp.c](suffix_array_rmq_lcp.c) — suffix array, LCP, and range-minimum query
- [suffix_automaton.c](suffix_automaton.c) — suffix automaton

The four Aho–Corasick files are intentionally retained as separate variants. They have not been consolidated because their differences and correctness have not yet been reviewed.

## Compile an example

From the repository root:

```bash
gcc algorithms/strings/kmp_search.c -std=c11 -Wall -Wextra -Wpedantic -o kmp_search
```

Run `./kmp_search` on Linux/macOS or `kmp_search.exe` on Windows.
