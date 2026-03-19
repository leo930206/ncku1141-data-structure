#include "hash.h"
#include <stdlib.h>

#define TABLE_SIZE 10007

typedef struct Node {
	Data data;
	struct Node *next;
} Node;

static Node *table[TABLE_SIZE];

static int hash(int key) {
	int h = key % TABLE_SIZE;
	if (h < 0) h += TABLE_SIZE;
	return h;
}

void insert(Data *data) {
	if (!data) return;
	int idx = hash(data->key);
	Node *cur = table[idx];
	while (cur) {
		if (cur->data.key == data->key) {
			cur->data.value = data->value;
			free(data);
			return;
		}
		cur = cur->next;
	}
	Node *n = (Node*)malloc(sizeof(Node));
	if (!n) { free(data); return; }
	n->data.key = data->key;
	n->data.value = data->value;
	n->next = table[idx];
	table[idx] = n;
	free(data);
}

void remove(int key) {
	int idx = hash(key);
	Node *cur = table[idx];
	Node *prev = NULL;
	while (cur) {
		if (cur->data.key == key) {
			if (prev) prev->next = cur->next;
			else table[idx] = cur->next;
			free(cur);
			return;
		}
		prev = cur;
		cur = cur->next;
	}
}

int search(int key) {
	int idx = hash(key);
	Node *cur = table[idx];
	while (cur) {
		if (cur->data.key == key) return cur->data.value;
		cur = cur->next;
	}
	return -1;
}

