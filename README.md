# DSA C Foundations

A learning-focused collection of C implementations and experiments covering data structures, algorithms, and systems programming.

The goal is to build understanding through implementation, experimentation, debugging, and careful review of how each approach works.

## Repository Map

### Algorithms
- [Graphs](algorithms/graphs/README.md) — BFS, shortest paths, spanning trees, connectivity, matching, and network flow.
- [Trees](algorithms/trees/README.md) — traversal, decomposition, ancestor queries, and tree dynamic programming.
- [Dynamic programming](algorithms/dynamic-programming/README.md) — knapsack, coin change, subsequences, and optimization problems.
- [String algorithms](algorithms/strings/README.md) — pattern matching, suffix structures, and substring queries.
- [Range-query algorithms](algorithms/range-queries/README.md) — inversion counting with a Fenwick tree.

### Data structures
- [Linear structures](data-structures/linear/README.md) — linked lists, stacks, and queues.
- [Trees](data-structures/trees/README.md) — binary trees and binary search trees.
- [Priority queues](data-structures/heaps/README.md) — priority-queue example.
- [Range-query structures](data-structures/range-queries/README.md) — Fenwick trees, segment trees, and bit vectors.
- [Tries](data-structures/tries/README.md) — prefix-tree autocomplete.

### Systems programming
- [Memory management](systems/memory/README.md) — alignment, arenas, and allocator experiments.
- [Concurrency](systems/concurrency/README.md) — queues, stacks, hazard pointers, and cache-line behavior.
- [Advanced experiments](systems/advanced/README.md) — AST analysis, consistent hashing, and timing wheels.

Each topic index links to its source files and provides compile guidance where useful.

## Building and Running

If a source file is a standalone program, compile it from the repository root with GCC:

```bash
gcc path/to/example.c -std=c11 -Wall -Wextra -Wpedantic -o example
```

For example:

```bash
gcc algorithms/graphs/graph_bfs.c -std=c11 -Wall -Wextra -Wpedantic -o graph_bfs
```

Run the program with `./graph_bfs` on Linux/macOS or `graph_bfs.exe` on Windows.

These commands are starting points, not a guarantee that every file builds with identical options. Check each source file for required inputs, dependencies, compiler settings, and limitations.

## Project Status and Quality

This is a learning repository, not a claim that every implementation is production-ready. Completeness, correctness testing, and performance characteristics vary, especially for memory-management and concurrency experiments.

Exact duplicate source files have been removed after comparing their contents; implementations that differ remain separate. Testing, edge-case coverage, and reproducible build checks are still ongoing. The string-algorithm index calls out known input and capacity limitations in the Aho–Corasick demonstrations.

---

*An ongoing C programming and computer science learning project.*
