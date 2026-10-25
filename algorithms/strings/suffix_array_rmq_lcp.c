#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

struct Suffix {
    int index;
    int rank[2];
};

int compare_suffixes(const void *a, const void *b) {
    struct Suffix *s1 = (struct Suffix*)a;
    struct Suffix *s2 = (struct Suffix*)b;
    if (s1->rank[0] != s2->rank[0]) {
        return s1->rank[0] - s2->rank[0];
    }
    return s1->rank[1] - s2->rank[1];
}

// Build Suffix Array using O(N log^2 N) Prefix Doubling
void build_suffix_array(const char *txt, int n, int sa[]) {
    struct Suffix *suffixes = (struct Suffix*)malloc(n * sizeof(struct Suffix));

    for (int i = 0; i < n; i++) {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i] - 'a';
        suffixes[i].rank[1] = (i + 1 < n) ? (txt[i + 1] - 'a') : -1;
    }

    qsort(suffixes, n, sizeof(struct Suffix), compare_suffixes);

    int *ind = (int*)malloc(n * sizeof(int));
    for (int k = 4; k < 2 * n; k *= 2) {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++) {
            if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i - 1].rank[1]) {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            } else {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; i++) {
            int next_idx = suffixes[i].index + k / 2;
            suffixes[i].rank[1] = (next_idx < n) ? suffixes[ind[next_idx]].rank[0] : -1;
        }

        qsort(suffixes, n, sizeof(struct Suffix), compare_suffixes);
    }

    for (int i = 0; i < n; i++) {
        sa[i] = suffixes[i].index;
    }

    free(suffixes);
    free(ind);
}

// Build Kasai's LCP Array in O(N)
void build_lcp_array(const char *txt, int n, int sa[], int lcp[], int inv_sa[]) {
    for (int i = 0; i < n; i++) {
        inv_sa[sa[i]] = i;
    }

    int h = 0;
    lcp[0] = 0;

    for (int i = 0; i < n; i++) {
        if (inv_sa[i] > 0) {
            int j = sa[inv_sa[i] - 1];
            while (i + h < n && j + h < n && txt[i + h] == txt[j + h]) {
                h++;
            }
            lcp[inv_sa[i]] = h;
            if (h > 0) h--;
        }
    }
}

// Segment Tree for Range Minimum Query (RMQ)
int seg_tree[4 * MAX_LEN];

int min_val(int a, int b) {
    return (a < b) ? a : b;
}

void build_rmq_tree(int node, int start, int end, int lcp[]) {
    if (start == end) {
        seg_tree[node] = lcp[start];
        return;
    }
    int mid = (start + end) / 2;
    build_rmq_tree(2 * node, start, mid, lcp);
    build_rmq_tree(2 * node + 1, mid + 1, end, lcp);
    seg_tree[node] = min_val(seg_tree[2 * node], seg_tree[2 * node + 1]);
}

int query_rmq(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 1e9;
    if (l <= start && end <= r) return seg_tree[node];

    int mid = (start + end) / 2;
    return min_val(query_rmq(2 * node, start, mid, l, r),
                   query_rmq(2 * node + 1, mid + 1, end, l, r));
}

// Query LCP between any two suffixes i and j in O(log N)
int query_arbitrary_lcp(int i, int j, int n, int inv_sa[]) {
    if (i == j) return n - i; // Same suffix

    int pos_i = inv_sa[i];
    int pos_j = inv_sa[j];

    if (pos_i > pos_j) {
        int temp = pos_i;
        pos_i = pos_j;
        pos_j = temp;
    }

    // Query range in LCP array: [pos_i + 1, pos_j]
    return query_rmq(1, 0, n - 1, pos_i + 1, pos_j);
}

int main() {
    const char *text = "banana";
    int n = strlen(text);

    int sa[MAX_LEN];
    int lcp[MAX_LEN];
    int inv_sa[MAX_LEN];

    build_suffix_array(text, n, sa);
    build_lcp_array(text, n, sa, lcp, inv_sa);
    build_rmq_tree(1, 0, n - 1, lcp);

    printf("=== Arbitrary Suffix LCP via Suffix Array + RMQ ===\n");
    printf("Base String: \"%s\"\n\n", text);

    // Suffix 1: "anana" (index 1) vs Suffix 3: "ana" (index 3) -> Common prefix "ana" (len 3)
    printf("LCP(suffix[1]=\"%s\", suffix[3]=\"%s\") = %d (Expected: 3)\n",
           text + 1, text + 3, query_arbitrary_lcp(1, 3, n, inv_sa));

    // Suffix 0: "banana" (index 0) vs Suffix 2: "nana" (index 2) -> Common prefix "" (len 0)
    printf("LCP(suffix[0]=\"%s\", suffix[2]=\"%s\") = %d (Expected: 0)\n",
           text + 0, text + 2, query_arbitrary_lcp(0, 2, n, inv_sa));

    // Suffix 2: "nana" (index 2) vs Suffix 4: "na" (index 4) -> Common prefix "na" (len 2)
    printf("LCP(suffix[2]=\"%s\", suffix[4]=\"%s\") = %d (Expected: 2)\n",
           text + 2, text + 4, query_arbitrary_lcp(2, 4, n, inv_sa));

    return 0;
}