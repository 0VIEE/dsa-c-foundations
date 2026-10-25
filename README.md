# DSA C Foundations

A learning-focused collection of C implementations and experiments covering data structures, algorithms, and systems programming.

This repository documents hands-on work with algorithm design, data structure implementation, memory management, and lower-level programming concepts.

## Repository Structure

Source files are being organized by topic while preserving their existing implementations.

- **[Graph algorithms](algorithms/graphs/README.md):** Traversal, shortest paths, spanning trees, matching, connectivity, and flow.
- **[Tree algorithms](algorithms/trees/README.md):** Tree traversal, decomposition, ancestor queries, and tree dynamic programming.
- **Root-level examples:** Additional string algorithms, dynamic programming exercises, data structures, memory-management experiments, and concurrency examples are still being classified.

Each topic folder includes an index with links to its implementations and a basic compile example.

## Building and Running

If a source file is a standalone program, you can compile it from the repository root with GCC:

```bash
gcc path/to/example.c -std=c11 -Wall -Wextra -Wpedantic -o example
```

For example:

```bash
gcc algorithms/graphs/graph_bfs.c -std=c11 -Wall -Wextra -Wpedantic -o graph_bfs
```

Run the compiled program with `./graph_bfs` on Linux/macOS or `graph_bfs.exe` on Windows.

The command above is a starting point, not a guarantee that every file compiles with the same options. Check each source file for required inputs, dependencies, and compiler settings.

## Development Approach

The goal is to learn through implementation, experimentation, debugging, and testing. Some programs are exercises or experimental prototypes; completeness, correctness testing, and performance characteristics vary. Advanced or concurrency-related examples should not be assumed to be production-ready.

Similarly named files are being retained until their differences and purposes can be reviewed, rather than deleting them based on filenames alone.

## Next Improvements

- Organize the remaining root-level source files into clear topic folders.
- Add example inputs, expected outputs, and per-program instructions where useful.
- Improve correctness tests and edge-case coverage.
- Review similarly named variants and document why each is kept.

---

*An ongoing C programming and computer science learning project.*
