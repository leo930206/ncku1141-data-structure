#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Union-Find結構體
int* parent = NULL;
int* size = NULL;

// 查找操作，帶路徑壓縮
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// 合併操作，帶按大小合併
void union_sets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    
    if (rootX != rootY) {
        // 合併兩個聯盟，將小樹接到大樹下面
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }
}

// 計算當前的聯盟數量
int count_components(int n) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (parent[i] == i) {
            count++;
        }
    }
    return count;
}

// 初始化 Union-Find 結構
void init_union_find(int n) {
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        size[i] = 1;
    }
}

int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);
    // 分配內存
    parent = (int*)malloc((n + 1) * sizeof(int));
    size = (int*)malloc((n + 1) * sizeof(int));
    int (*edges)[2] = (m>0) ? malloc(m * sizeof(int[2])) : NULL;
    int* collapse = (q>0) ? malloc(q * sizeof(int)) : NULL;

    if (!parent || !size || (m>0 && !edges) || (q>0 && !collapse)) {
        printf("Memory allocation failed!\n");
        free(parent);
        free(size);
        free(edges);
        free(collapse);
        return 1;
    }

    for (int i = 0; i < m; i++) scanf("%d %d", &edges[i][0], &edges[i][1]);
    for (int i = 0; i < q; i++) scanf("%d", &collapse[i]);

    // 標記被崩塌的橋
    char *removed = (char*)calloc(m, 1);
    if (q > 0) {
        for (int i = 0; i < q; i++) {
            int idx = collapse[i] - 1;
            if (idx >= 0 && idx < m) removed[idx] = 1;
        }
    }

    // 初始化 Union-Find
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        size[i] = 1;
    }

    int components = n;
    // 連接未崩塌的橋
    for (int i = 0; i < m; i++) {
        if (removed[i]) continue;
        int u = edges[i][0], v = edges[i][1];
        int ru = find(u), rv = find(v);
        if (ru != rv) {
            union_sets(ru, rv);
            components--;
        }
    }

    int *ans = (int*)malloc((q + 1) * sizeof(int));
    if (!ans) {
        free(parent); free(size); free(edges); free(collapse); free(removed);
        printf("Memory allocation failed!\n");
        return 1;
    }

    ans[q] = components;
    for (int i = q - 1; i >= 0; i--) {
        int eidx = collapse[i] - 1;
        if (eidx < 0 || eidx >= m) { ans[i] = components; continue; }
        int u = edges[eidx][0], v = edges[eidx][1];
        int ru = find(u), rv = find(v);
        if (ru != rv) {
            union_sets(ru, rv);
            components--;
        }
        ans[i] = components;
    }

    for (int i = 0; i <= q; i++) printf("%d\n", ans[i]);

    free(ans);
    free(removed);
    free(parent);
    free(size);
    free(edges);
    free(collapse);
    return 0;
}