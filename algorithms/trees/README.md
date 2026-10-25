# Tree Algorithms

This folder contains algorithms that operate on trees, including decomposition, ancestor queries, traversal, and dynamic programming.

## Implementations

| File | Focus |
|---|---|
| [centroid_decomposition.c](centroid_decomposition.c) | Centroid decomposition |
| [hld_segment_tree.c](hld_segment_tree.c) | Heavy-light decomposition with a segment tree |
| [lca_binary_lifting.c](lca_binary_lifting.c) | Lowest common ancestor using binary lifting |
| [lca_euler_rmq.c](lca_euler_rmq.c) | Lowest common ancestor using an Euler tour and range-minimum query |
| [tree_diameter_bfs.c](tree_diameter_bfs.c) | Tree diameter using breadth-first search |
| [tree_metrics.c](tree_metrics.c) | Tree metrics and measurements |
| [tree_traversals.c](tree_traversals.c) | Tree traversal methods |
| [tree_vertex_cover_dp.c](tree_vertex_cover_dp.c) | Tree vertex cover using dynamic programming |

## Compile an example

From the repository root, compile a standalone source file with GCC:

```bash
gcc algorithms/trees/tree_traversals.c -std=c11 -Wall -Wextra -Wpedantic -o tree_traversals
```

Run it on Linux/macOS with `./tree_traversals`, or on Windows with `tree_traversals.exe`.

This index describes the intended topic of each example; inspect each source file for its specific input assumptions and limitations.
