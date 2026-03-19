#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int M, N;
    if (scanf("%d %d", &M, &N) != 2) return 0;

    // allocate grid: M rows, N cols
    size_t cells = (size_t)M * (size_t)N;
    char *grid = (char*)malloc(cells + 1);
    if (!grid) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // store positions of uppercase letters A-Z (each at most 2 positions)
    int letter_pos[26][2];
    int letter_cnt[26] = {0};
    for (int i = 0; i < 26; ++i) { letter_pos[i][0] = letter_pos[i][1] = -1; }

    int start = -1, exitp = -1;
    char *rowbuf = (char*)malloc((size_t)N + 8);
    if (!rowbuf) { free(grid); fprintf(stderr, "Memory allocation failed\n"); return 1; }

    for (int r = 0; r < M; ++r) {
        if (scanf("%s", rowbuf) != 1) { free(grid); free(rowbuf); return 0; }
        for (int c = 0; c < N; ++c) {
            char ch = rowbuf[c];
            size_t idx = (size_t)r * N + c;
            grid[idx] = ch;
            if (ch == 'B') start = (int)idx;
            else if (ch == 'E') exitp = (int)idx;
            else if (ch >= 'A' && ch <= 'Z') {
                int li = ch - 'A';
                if (letter_cnt[li] < 2) {
                    letter_pos[li][letter_cnt[li]] = (int)idx;
                    letter_cnt[li]++;
                }
            }
        }
    }
    free(rowbuf);

    if (start == -1 || exitp == -1) { printf("-1\n"); free(grid); return 0; }
    if (start == exitp) { printf("0\n"); free(grid); return 0; }

    // BFS
    int *dist = (int*)malloc(cells * sizeof(int));
    int *queue = (int*)malloc(cells * sizeof(int));
    if (!dist || !queue) { fprintf(stderr, "Memory allocation failed\n"); free(grid); free(dist); free(queue); return 1; }
    for (size_t i = 0; i < cells; ++i) dist[i] = -1;

    size_t qh = 0, qt = 0;
    queue[qt++] = start;
    dist[start] = 0;

    const int dr[4] = {-1,1,0,0};
    const int dc[4] = {0,0,-1,1};

    while (qh < qt) {
        int u = queue[qh++];
        int du = dist[u];
        if (u == exitp) {
            printf("%d\n", du);
            free(grid); free(dist); free(queue);
            return 0;
        }
        int r = u / N;
        int c = u % N;
        // 4-neighbors
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr < 0 || nr >= M || nc < 0 || nc >= N) continue;
            int v = nr * N + nc;
            char ch = grid[v];
            if (ch == '#') continue;
            if (dist[v] == -1) {
                dist[v] = du + 1;
                queue[qt++] = v;
            }
        }
        // teleport if on A-Z (excluding B and E). Teleport costs 1.
        char cur = grid[u];
        if (cur >= 'A' && cur <= 'Z') {
            int li = cur - 'A';
            if (letter_cnt[li] > 0) {
                for (int j = 0; j < letter_cnt[li]; ++j) {
                    int v = letter_pos[li][j];
                    if (v == -1 || v == u) continue;
                    if (dist[v] == -1) {
                        dist[v] = du + 1;
                        queue[qt++] = v;
                    }
                }
                // clear list so we don't process these teleports again
                letter_cnt[li] = 0;
            }
        }
    }

    printf("-1\n");
    free(grid); free(dist); free(queue);
    return 0;
}
