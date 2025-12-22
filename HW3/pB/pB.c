#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#else
#include <unistd.h>
#endif

// Fast integer reader using fread
static const int BUFSIZE = 1 << 20;
static char buf[1 << 20];
static int buf_len = 0, buf_pos = 0;

static void refill() {
    buf_len = fread(buf, 1, BUFSIZE, stdin);
    buf_pos = 0;
    if (buf_len == 0) buf[0] = '\0';
}

static inline int next_int() {
    int x = 0, s = 1;
    if (buf_pos >= buf_len) refill();
    while (buf_pos < buf_len && (buf[buf_pos] < '0' && buf[buf_pos] != '-')) {
        buf_pos++;
        if (buf_pos >= buf_len) refill();
    }
    if (buf_pos < buf_len && buf[buf_pos] == '-') { s = -1; buf_pos++; }
    while (buf_pos < buf_len && buf[buf_pos] >= '0') {
        x = x * 10 + (buf[buf_pos++] - '0');
        if (buf_pos >= buf_len) refill();
    }
    return x * s;
}

int main(void) {
    // Detect interactive stdin (tty). If interactive, use scanf to avoid waiting for EOF.
    int interactive;
#ifdef _WIN32
    interactive = _isatty(0);
#else
    interactive = isatty(0);
#endif
    int N, D;
    if (interactive) {
        if (scanf("%d %d", &N, &D) != 2) return 0;
    } else {
        // Use stdin fast reader
        refill();
        N = next_int();
        D = next_int();
    }
    if (N <= 0) {
        // If no tasks, trivially can assemble
        printf("0\n");
        return 0;
    }

    // allocate adjacency list arrays
    // head: size N, initialized to -1
    int *head = (int*)malloc((size_t)N * sizeof(int));
    if (!head) { perror("malloc"); return 1; }
    for (int i = 0; i < N; ++i) head[i] = -1;

    int *to = (int*)malloc((size_t)D * sizeof(int));
    int *next = (int*)malloc((size_t)D * sizeof(int));
    int *indeg = (int*)malloc((size_t)N * sizeof(int));
    int *queue = (int*)malloc((size_t)N * sizeof(int));
    if ((!to && D>0) || (!next && D>0) || !indeg || !queue) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < N; ++i) indeg[i] = 0;

    // read edges and build adjacency
    for (int e = 0; e < D; ++e) {
        int u, v;
        if (interactive) {
            if (scanf("%d %d", &u, &v) != 2) { u = 0; v = 0; }
        } else {
            u = next_int();
            v = next_int();
        }
        // assume 0 <= u,v < N
        to[e] = v;
        next[e] = head[u];
        head[u] = e;
        indeg[v]++;
    }

    // Kahn's algorithm
    int qh = 0, qt = 0;
    for (int i = 0; i < N; ++i) if (indeg[i] == 0) queue[qt++] = i;

    int visited = 0;
    while (qh < qt) {
        int u = queue[qh++];
        visited++;
        for (int ei = head[u]; ei != -1; ei = next[ei]) {
            int v = to[ei];
            indeg[v]--;
            if (indeg[v] == 0) queue[qt++] = v;
        }
    }

    if (visited == N) printf("0\n");
    else printf("1\n");

    free(head);
    free(to);
    free(next);
    free(indeg);
    free(queue);
    return 0;
}
