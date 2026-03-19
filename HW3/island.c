#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000
#define MAX_M 200000

// Union-Find結構體
int parent[MAX_N + 1];
int size[MAX_N + 1];

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

int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);

    // 初始化父節點和大小
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        size[i] = 1;
    }

    int edges[MAX_M][2];
    // 讀取橋樑
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &edges[i][0], &edges[i][1]);
    }

    int collapse[MAX_M];  // 存放崩塌橋樑的ID
    // 讀取崩塌的橋樑
    for (int i = 0; i < q; i++) {
        scanf("%d", &collapse[i]);
    }

    // 初始情況下，根據所有橋樑進行合併
    for (int i = 0; i < m; i++) {
        union_sets(edges[i][0], edges[i][1]);
    }

    // 計算初始的聯盟數量
    int initial_alliances = 0;
    for (int i = 1; i <= n; i++) {
        if (parent[i] == i) {
            initial_alliances++;
        }
    }

    // 輸出初始狀況下的聯盟數量
    printf("%d\n", initial_alliances);

    // 處理每次橋樑崩塌，並計算新的聯盟數量
    for (int i = 0; i < q; i++) {
        // 崩塌的橋樑ID是從1開始，所以要減去1
        int bridge = collapse[i] - 1;
        // 重置並刪除該橋
        parent[edges[bridge][0]] = edges[bridge][0];
        parent[edges[bridge][1]] = edges[bridge][1];
        size[edges[bridge][0]] = 1;
        size[edges[bridge][1]] = 1;

        // 重新連接所有橋，忽略已崩塌的橋
        for (int j = 0; j < m; j++) {
            if (j != bridge) {
                union_sets(edges[j][0], edges[j][1]);
            }
        }

        // 計算崩塌後的聯盟數量
        int current_alliances = 0;
        for (int i = 1; i <= n; i++) {
            if (parent[i] == i) {
                current_alliances++;
            }
        }

        // 輸出當前的聯盟數量
        printf("%d\n", current_alliances);
    }

    return 0;
}
