#include <malloc.h>
#include <stdio.h>

struct Heap {
    int* arr;        // 指向 heap 元素的指標
    int size;        // heap 當前元素的數量
    int capacity;    // heap 的最大容量
};

typedef struct Heap heap;

heap* createHeap(int capacity);
void insert(heap* h, int data);
void maxHeapify(heap* h, int index);
void insertHelper(heap* h, int index);
void printHeap(heap* h);
void battleAndMerge(heap* h);
void changeBlock(heap* h, int idx, int delta);

// 創建 heap
heap* createHeap(int capacity)
{
    heap* h = (heap*)malloc(sizeof(heap));

    h->size = 0;
    h->capacity = capacity;

    h->arr = (int*)malloc(capacity * sizeof(int));

    return h;
}

// bottom-up
void insertHelper(heap* h, int index)
{
    int parent = (index - 1) / 2;
    if (h->arr[parent] < h->arr[index]) {
        int temp = h->arr[parent];
        h->arr[parent] = h->arr[index];
        h->arr[index] = temp;

        insertHelper(h, parent);
    }
}

// top-down
void maxHeapify(heap* h, int index)
{
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int max = index;

    if (left < h->size && h->arr[left] > h->arr[max]) {
        max = left;
    }
    if (right < h->size && h->arr[right] > h->arr[max]) {
        max = right;
    }

    if (max != index) {
        int temp = h->arr[max];
        h->arr[max] = h->arr[index];
        h->arr[index] = temp;
        maxHeapify(h, max);
    }
}

// Add a Block
void insert(heap* h, int data)
{
    if (h->size < h->capacity) {
        h->arr[h->size] = data;
        insertHelper(h, h->size);
        h->size++;
    }
}

int extractMax(heap* h)
{
    int deleteItem;

    deleteItem = h->arr[0];

    h->arr[0] = h->arr[h->size - 1];
    h->size--; 

    maxHeapify(h, 0);

    return deleteItem;
}

// Battle and Merge
void battleAndMerge(heap* h)
{
    if (h->size >= 2) {
        int largest = extractMax(h);
        int second_largest = extractMax(h);

        int new_block = largest - second_largest;

        insert(h, new_block);
    }
}

// Change a Block
void changeBlock(heap* h, int idx, int delta)
{
    h->arr[idx] += delta;
    if (delta > 0) {
        insertHelper(h, idx);
    } else {
        maxHeapify(h, idx);
    }
}

// Print heap 當前內容
void printHeap(heap* h)
{
    for (int i = 0; i < h->size; i++) {
        printf("%d ", h->arr[i]);
    }
    printf("\n");
}

int main()
{
    int N;
    scanf("%d", &N);
    heap* hp = createHeap(N);

    for (int i = 0; i < N; i++) {
        int op;
        scanf("%d", &op);

        if (op == 0) {
            int x;
            scanf("%d", &x);
            insert(hp, x);
        } else if (op == 1) {
            battleAndMerge(hp);
        } else if (op == 2) {
            int idx, delta;
            scanf("%d %d", &idx, &delta);
            changeBlock(hp, idx, delta);
        }
    }

    printHeap(hp);

    return 0;
}
