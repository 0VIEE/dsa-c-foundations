#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_VIRTUAL_NODES 16

struct VirtualNode {
    uint32_t hash_value;
    char server_id[16];
};

struct ConsistentHashRing {
    struct VirtualNode ring[MAX_VIRTUAL_NODES];
    size_t node_count;
};

// Simple FNV-1a 32-bit hash function
uint32_t fnv1a_hash(const char *key) {
    uint32_t hash = 2166136261u;
    while (*key) {
        hash ^= (uint8_t)*key++;
        hash *= 16777619u;
    }
    return hash;
}

struct ConsistentHashRing* create_hash_ring() {
    struct ConsistentHashRing *p = (struct ConsistentHashRing*)malloc(sizeof(struct ConsistentHashRing));
    p->node_count = 0;
    return p;
}

// Add physical server with virtual nodes to hash ring
void hash_ring_add_server(struct ConsistentHashRing *p, const char *server_id, int vnode_replicas) {
    for (int i = 0; i < vnode_replicas; i++) {
        if (p->node_count >= MAX_VIRTUAL_NODES) break;

        char vnode_key[32];
        snprintf(vnode_key, sizeof(vnode_key), "%s#%d", server_id, i);

        uint32_t hash = fnv1a_hash(vnode_key);
        p->ring[p->node_count].hash_value = hash;
        strncpy(p->ring[p->node_count].server_id, server_id, 15);
        p->ring[p->node_count].server_id[15] = '\0';
        p->node_count++;

        printf("  [Ring Add] Added Virtual Node '%s' -> Hash: 0x%08X\n", vnode_key, hash);
    }

    // Simple bubble sort to keep ring sorted by hash value for binary lookup
    for (size_t i = 0; i < p->node_count - 1; i++) {
        for (size_t j = 0; j < p->node_count - i - 1; j++) {
            if (p->ring[j].hash_value > p->ring[j + 1].hash_value) {
                struct VirtualNode temp = p->ring[j];
                p->ring[j] = p->ring[j + 1];
                p->ring[j + 1] = temp;
            }
        }
    }
}

// Map key to first server clockwise on hash ring
const char* hash_ring_get_server(struct ConsistentHashRing *p, const char *key) {
    if (p->node_count == 0) return NULL;

    uint32_t key_hash = fnv1a_hash(key);
    for (size_t i = 0; i < p->node_count; i++) {
        if (key_hash <= p->ring[i].hash_value) {
            return p->ring[i].server_id;
        }
    }
    // Wrap around to first virtual node on ring
    return p->ring[0].server_id;
}

void free_hash_ring(struct ConsistentHashRing *p) {
    if (p != NULL) free(p);
}

int main() {
    printf("=== Consistent Hashing Engine with Virtual Nodes (ANSI C) ===\n");
    struct ConsistentHashRing *ring = create_hash_ring();

    printf("Populating Hash Ring with Physical Servers & Replicas:\n");
    hash_ring_add_server(ring, "Server-Alpha", 2);
    hash_ring_add_server(ring, "Server-Beta",  2);

    printf("\nRouting Application Request Keys to Ring Nodes:\n");
    const char *keys[] = {"user_session_101", "cart_data_882", "cache_key_994"};
    for (int i = 0; i < 3; i++) {
        uint32_t k_hash = fnv1a_hash(keys[i]);
        const char *target = hash_ring_get_server(ring, keys[i]);
        printf("  -> Key '%s' (Hash: 0x%08X) mapped clockwise to: %s\n", keys[i], k_hash, target);
    }

    free_hash_ring(ring);
    printf("[PASS] Consistent Hashing execution completed cleanly.\n");
    return 0;
}