#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MOD 1000000007
#define MAX_N 5005

typedef struct TrieNode {
    struct TrieNode *children[26];
    int count; 
} TrieNode;

TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->count = 0;
    for (int i = 0; i < 26; i++) node->children[i] = NULL;
    return node;
}

void insert(TrieNode* root, char* word) {
    int len = strlen(word);
    TrieNode* curr = root;
    for (int i = len - 1; i >= 0; i--) { 
        int idx = word[i] - 'a';
        if (curr->children[idx] == NULL) {
            curr->children[idx] = createNode();
        }
        curr = curr->children[idx];
    }
    curr->count++; 
}

int main() {
    char target[MAX_N];
    if (scanf("%s", target) == EOF) return 0;

    int k;
    scanf("%d", &k);
    
    TrieNode* root = createNode();
    char word[1000005];
    for (int i = 0; i < k; i++) {
        scanf("%s", word);
        insert(root, word); 
    }

    int n = strlen(target);
    long long dp[MAX_N] = {0};
    dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        TrieNode* curr = root;
        for (int j = i - 1; j >= 0; j--) {
            int idx = target[j] - 'a';
            if (curr->children[idx] == NULL) break;
            
            curr = curr->children[idx];
            if (curr->count > 0) {
                dp[i] = (dp[i] + (dp[j] * curr->count)) % MOD;
            }
        }
    }

    printf("%lld\n", dp[n]);

    return 0;
}