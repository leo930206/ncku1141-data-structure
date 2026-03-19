#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int key;
    struct Node *left, *right;
};

struct Node* root = NULL;

struct Node* insertNode(struct Node* node, int value);
struct Node* deleteNode(struct Node* root, int value);
struct Node* minValueNode(struct Node* node);
void levelOrder(struct Node* root);

// main function
int main() {
    while (1) {
        char op[7] = {};
        int x;

        scanf("%s", op);

        if (strcmp(op, "insert") == 0) {
            scanf("%d", &x);
            root = insertNode(root, x);
        }
        else if (strcmp(op, "delete") == 0) {
            scanf("%d", &x);
            root = deleteNode(root, x);
        }
        else if (strcmp(op, "exit") == 0) {
            levelOrder(root);
            return 0;
        }
    }
}

// insert x in node 
struct Node* insertNode(struct Node* node, int value) {
    if (node == NULL) {
        node = (struct Node*)malloc(sizeof(struct Node));
        node->key = value;
        node->left = node->right = NULL;
    }
    else if (value < node->key) {
        node->left = insertNode(node->left, value);
    }
    else if (value > node->key) {
        node->right = insertNode(node->right, value);
    }
    return node;
}

// delete x in node  
struct Node* deleteNode(struct Node* root, int value) {
    if (root == NULL) return root;

    if (value < root->key) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->key) {
        root->right = deleteNode(root->right, value);
    } else {
        // 當 root 為要刪除的 node 時
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // 當 node 有兩個子 node 時
        struct Node* temp = minValueNode(root->right);  // 找到右子樹中最小的 node
        root->key = temp->key;  // 用最小 node 的 key value 替換當前 node
        root->right = deleteNode(root->right, temp->key);  // 刪除右子樹中最小的 node
    }
    return root;
}

// 找到最小值 node 
struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// level order 遍歷
void levelOrder(struct Node* root) {
    if (root == NULL) return;

    struct Node* queue[3000];  // 題目: Number of commands < 3000
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        struct Node* current = queue[front++];  // 取出 front 的元素
        printf("%d ", current->key);

        if (current->left != NULL) {
            queue[rear++] = current->left;  // 加入左子節點
        }

        if (current->right != NULL) {
            queue[rear++] = current->right;  // 加入右子節點
        }
    }
    printf("\n");
}
