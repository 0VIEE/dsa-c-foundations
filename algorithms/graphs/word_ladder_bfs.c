#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_WORDS 100
#define WORD_LEN 10

struct QueueNode {
    int word_idx;
    int depth;
    struct QueueNode *link;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

struct Queue* create_queue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(struct Queue *q, int word_idx, int depth) {
    struct QueueNode *p = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    p->word_idx = word_idx;
    p->depth = depth;
    p->link = NULL;

    if (q->rear == NULL) {
        q->front = p;
        q->rear = p;
        return;
    }
    q->rear->link = p;
    q->rear = p;
}

bool is_empty(struct Queue *q) {
    return q->front == NULL;
}

struct QueueNode dequeue(struct Queue *q) {
    struct QueueNode item = {-1, 0, NULL};
    if (is_empty(q)) return item;

    struct QueueNode *p = q->front;
    item = *p;
    q->front = q->front->link;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(p);
    return item;
}

// Check if two words differ by exactly one character
bool differs_by_one(const char *w1, const char *w2, int len) {
    int diff_count = 0;
    for (int i = 0; i < len; i++) {
        if (w1[i] != w2[i]) {
            diff_count++;
            if (diff_count > 1) return false;
        }
    }
    return diff_count == 1;
}

int ladder_length(const char *beginWord, const char *endWord, char wordList[][WORD_LEN], int wordCount) {
    int len = strlen(beginWord);
    int end_idx = -1;

    for (int i = 0; i < wordCount; i++) {
        if (strcmp(wordList[i], endWord) == 0) {
            end_idx = i;
            break;
        }
    }
    if (end_idx == -1) return 0; // Target word not in dictionary

    bool visited[MAX_WORDS] = {false};
    struct Queue *q = create_queue();

    // Push initial transitions from beginWord
    for (int i = 0; i < wordCount; i++) {
        if (differs_by_one(beginWord, wordList[i], len)) {
            if (i == end_idx) {
                free(q);
                return 2; // Direct one-step conversion (begin -> end)
            }
            enqueue(q, i, 2);
            visited[i] = true;
        }
    }

    int result = 0;
    while (!is_empty(q)) {
        struct QueueNode curr = dequeue(q);

        if (curr.word_idx == end_idx) {
            result = curr.depth;
            break;
        }

        for (int i = 0; i < wordCount; i++) {
            if (!visited[i] && differs_by_one(wordList[curr.word_idx], wordList[i], len)) {
                visited[i] = true;
                enqueue(q, i, curr.depth + 1);
            }
        }
    }

    // Cleanup queue
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);

    return result;
}

int main() {
    /*
        Begin Word: "hit"
        End Word:   "cog"
        Word List:  {"hot", "dot", "dog", "lot", "log", "cog"}
        Shortest Path: "hit" -> "hot" -> "dot" -> "dog" -> "cog" (Length = 5)
    */
    char beginWord[] = "hit";
    char endWord[] = "cog";
    char wordList[][WORD_LEN] = {"hot", "dot", "dog", "lot", "log", "cog"};
    int wordCount = sizeof(wordList) / sizeof(wordList[0]);

    int shortest_len = ladder_length(beginWord, endWord, wordList, wordCount);

    printf("=== Word Ladder Shortest Path (BFS) ===\n");
    printf("Begin Word: %s\n", beginWord);
    printf("End Word:   %s\n", endWord);
    printf("Shortest Transformation Length: %d\n", shortest_len); // Expected: 5

    return 0;
}