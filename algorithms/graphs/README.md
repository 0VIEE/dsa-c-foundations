# Graph Algorithms

This folder collects graph representations, traversal methods, shortest-path algorithms, spanning trees, connectivity tools, matching, and flow algorithms.

## Implementations

| File | Focus |
|---|---|
| [bellman_ford.c](bellman_ford.c) | Bellman–Ford shortest paths |
| [dijkstra_min_heap.c](dijkstra_min_heap.c) | Dijkstra shortest paths using a min-heap |
| [floyd_warshall.c](floyd_warshall.c) | All-pairs shortest paths |
| [graph_adj_matrix.c](graph_adj_matrix.c) | Graph representation using an adjacency matrix |
| [graph_bfs.c](graph_bfs.c) | Breadth-first search |
| [graph_cycle_detect.c](graph_cycle_detect.c) | Graph cycle detection |
| [graph_topo_sort.c](graph_topo_sort.c) | Topological sorting |
| [word_ladder_bfs.c](word_ladder_bfs.c) | Word Ladder search using BFS |
| [prim_mst.c](prim_mst.c) | Prim's minimum spanning tree implementation using an adjacency list and min-heap |
| [prims_mst.c](prims_mst.c) | Prim's minimum spanning tree implementation using an adjacency matrix |
| [dsu_optimized.c](dsu_optimized.c) | Disjoint Set Union (Union-Find) |
| [tarjan_articulation_points.c](tarjan_articulation_points.c) | Articulation points |
| [tarjan_bridges.c](tarjan_bridges.c) | Bridges in a graph |
| [hopcroft_karp.c](hopcroft_karp.c) | Bipartite matching with Hopcroft–Karp |
| [dinic_max_flow.c](dinic_max_flow.c) | Maximum flow using Dinic's algorithm |

## Compile an example

From the repository root, compile a standalone source file with GCC:

```bash
gcc algorithms/graphs/graph_bfs.c -std=c11 -Wall -Wextra -Wpedantic -o graph_bfs
```

Run it on Linux/macOS with `./graph_bfs`, or on Windows with `graph_bfs.exe`.

Examples may use different input assumptions or compiler requirements. Check the source file before choosing compiler options; this index does not imply that every implementation has been fully tested.
