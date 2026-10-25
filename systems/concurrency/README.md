# Concurrency Experiments

## Implementations

- [cache_line_padding.c](cache_line_padding.c) — cache-line padding and false-sharing experiment
- [hazard_pointers.c](hazard_pointers.c) — hazard-pointer concept demonstration
- [lockfree_queue.c](lockfree_queue.c) — lock-free queue experiment
- [mpmc_bounded_queue.c](mpmc_bounded_queue.c) — bounded multi-producer, multi-consumer queue
- [spsc_ring_buffer.c](spsc_ring_buffer.c) — single-producer, single-consumer ring buffer
- [treiber_stack.c](treiber_stack.c) — Treiber stack experiment

Concurrency code can be sensitive to memory ordering, reclamation, and platform details. These filenames describe the topic, not a guarantee of production-level correctness or lock-free performance.
