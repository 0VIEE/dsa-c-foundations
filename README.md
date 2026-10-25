# DSA C Foundations

[![C compile and smoke tests](https://github.com/0VIEE/dsa-c-foundations/actions/workflows/compile-c.yml/badge.svg)](https://github.com/0VIEE/dsa-c-foundations/actions/workflows/compile-c.yml)
![Language](https://img.shields.io/badge/language-C11-00599C?logo=c&logoColor=white)

A learning-focused collection of C implementations covering data structures, algorithms, and systems programming. The goal is to understand how each technique works by implementing it, checking its behavior, and documenting its assumptions.

## What you'll find

- **Algorithms** — graph traversal and shortest paths, spanning trees, tree algorithms, dynamic programming, string matching, and range-query problems.
- **Data structures** — linked lists, stacks, queues, trees, heaps, Fenwick trees, segment trees, bit vectors, and tries.
- **Systems experiments** — memory allocation, alignment, concurrent queues and stacks, hazard pointers, consistent hashing, and timing wheels.

## Repository map

### Algorithms
- [Graphs](algorithms/graphs/README.md) — BFS, shortest paths, spanning trees, connectivity, matching, and maximum flow.
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

## Quick start

Install GCC and compile an individual standalone example from the repository root:

```bash
gcc algorithms/graphs/graph_bfs.c -std=c11 -Wall -Wextra -Wpedantic -o graph_bfs
```

Run it on Linux/macOS with `./graph_bfs`, or on Windows with `graph_bfs.exe`.

Most programs use built-in demonstration data rather than reading command-line input. Read the source and its topic README for input assumptions, fixed limits, and implementation details before adapting an example.

## Build and test

Run the repository-wide compile check and targeted smoke tests in a Bash environment with GCC installed:

```bash
bash scripts/compile_all.sh
bash scripts/test_examples.sh
```

The compile check builds every C source file with C11 and common warnings treated as errors. The smoke tests verify expected output from representative algorithms and exercise invalid-input and boundary cases in the Aho–Corasick, KMP, and trie examples. GitHub Actions runs these checks on pushes and pull requests.

See [tests/README.md](tests/README.md) for coverage and limitations.

## Project status

This is an active learning repository, not a claim that every implementation is production-ready. Automated checks confirm that all examples compile without warnings and that selected sample cases behave as expected; they do not prove every algorithm correct. Memory-management and concurrency experiments in particular require additional edge-case, memory-safety, and stress testing before reuse in real systems.

Exact duplicate files have been removed, distinct implementations have descriptive names, and each topic folder has its own index. See [CONTRIBUTING.md](CONTRIBUTING.md) for conventions when adding or improving examples.
