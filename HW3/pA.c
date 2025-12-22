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
    if (scanf("%d %d %d", &n, &m, &q) != 3) return 0;

    // 分配內存
    parent = (int*)malloc((n + 1) * sizeof(int));
    size = (int*)malloc((n + 1) * sizeof(int));
    int (*edges)[2] = malloc(m * sizeof(int[2]));
    int* collapse = malloc(q * sizeof(int));

    if (!parent || !size || (m>0 && !edges) || (q>0 && !collapse)) {
        printf("Memory allocation failed!\n");
        free(parent);
        free(size);
        free(edges);
        free(collapse);
        return 1;
    }

    // 讀取橋樑
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &edges[i][0], &edges[i][1]);
    }

    // 讀取崩塌的橋樑
    for (int i = 0; i < q; i++) {
        scanf("%d", &collapse[i]);
    }

    // 處理並輸出每個階段的聯盟數量
    for (int i = 0; i <= q; i++) {
        // 初始化 Union-Find 結構
        init_union_find(n);

        // 連接當前階段的橋樑
        for (int j = 0; j < m; j++) {
            bool is_collapsed = false;
            // 檢查當前橋是否已經崩塌
            for (int k = 0; k < i && k < q; k++) {
                if (j == collapse[k] - 1) {
                    is_collapsed = true;
                    break;
                }
            }
            if (!is_collapsed) {
                union_sets(edges[j][0], edges[j][1]);
            }
        }

        // 輸出當前階段的聯盟數量
        printf("%d\n", count_components(n));
    }

    // 釋放內存
    free(parent);
    free(size);
    free(edges);
    free(collapse);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Union-Find結構體
int* parent = NULL;
int* size = NULL;
int (*edges)[2] = NULL;
int* collapse = NULL;
int* restored = NULL;

// 查找操作，帶路徑壓縮
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #define MAX_N 200000
    #define MAX_M 200000

    // Union-Find結構體
    int* parent = NULL;
    int* size = NULL;
    // 從前往後處理每個崩塌的橋
    for (int i = 0; i < q; i++) {
        // 重置所有數據結構
        for (int j = 1; j <= n; j++) {
            parent[j] = j;
            size[j] = 1;
        }

        // 連接除了已崩塌的橋之外的所有橋
        for (int j = 0; j < m; j++) {
            bool is_collapsed = false;
            // 檢查當前橋是否在已崩塌的橋之列
            for (int k = 0; k <= i; k++) {
                if (j == collapse[k] - 1) {
                    is_collapsed = true;
                    break;
                }
            }
            if (!is_collapsed) {
                union_sets(edges[j][0], edges[j][1]);
            }
        }

        // 計算當前的聯盟數量
        components = 0;
        for (int j = 1; j <= n; j++) {
            if (parent[j] == j) {
                components++;
            }
        }
        // 分配內存
        parent = (int*)malloc((n + 1) * sizeof(int));
        size = (int*)malloc((n + 1) * sizeof(int));
        int (*edges)[2] = malloc(m * sizeof(int[2]));
        int* collapse = malloc(q * sizeof(int));

        if (!parent || !size || !edges || !collapse) {
            printf("Memory allocation failed!\n");
            free(parent);
            free(size);
            free(edges);
            free(collapse);
            return 1;
        }
    // 記錄所有橋的狀態，開始時我們先將所有橋樑連接
        // 讀取橋樑
        for (int i = 0; i < m; i++) {
            scanf("%d %d", &edges[i][0], &edges[i][1]);
        }

        // 讀取崩塌的橋樑
        for (int i = 0; i < q; i++) {
            scanf("%d", &collapse[i]);
        }
        free(edges);
        // 初始狀態：所有橋都存在
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    // 初始情況下，連接所有橋樑
        for (int i = 0; i < m; i++) {
            union_sets(edges[i][0], edges[i][1]);
        }
    int* results = (int*)malloc((q + 1) * sizeof(int));
        // 計算並輸出初始狀態的聯盟數量
        int components = 0;
        for (int i = 1; i <= n; i++) {
            if (parent[i] == i) {
                components++;
            }
        }
        printf("%d\n", components);
        free(size);
        // 按順序處理崩塌的橋
        for (int i = 0; i < q; i++) {
            // 重置所有數據結構
            for (int j = 1; j <= n; j++) {
                parent[j] = j;
                size[j] = 1;
            }
    }
            // 重新連接未崩塌的橋
            for (int j = 0; j < m; j++) {
                bool is_collapsed = false;
                for (int k = 0; k <= i; k++) {
                    if (j == collapse[k] - 1) {
                        is_collapsed = true;
                        break;
                    }
                }
                if (!is_collapsed) {
                    union_sets(edges[j][0], edges[j][1]);
                }
            }
    }
            // 計算並輸出當前的聯盟數量
            components = 0;
            for (int j = 1; j <= n; j++) {
                if (parent[j] == j) {
                    components++;
                }
            }
            printf("%d\n", components);
        }
    // 逐一斷開橋樑並計算結果
        // 釋放內存
        free(parent);
        free(size);
        free(edges);
        free(collapse);
        }
        
        // 計算連通分量數量
        int components = 0;
        for (int j = 1; j <= n; j++) {
            if (parent[j] == j) {
                components++;
            }
        }
        results[i] = components;
    }

    // 計算初始的聯盟數量
    int initial_alliances = 0;
    for (int i = 1; i <= n; i++) {
        if (parent[i] == i) {
            initial_alliances++;
        }
    }

    // 按順序輸出結果
    for (int i = q; i >= 0; i--) {
        printf("%d\n", results[i]);
    }
    
    // 釋放記憶體
    free(results);

    // 釋放動態分配的內存
    free(parent);
    free(size);

    return 0;
}
