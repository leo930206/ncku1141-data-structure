// hw.c
#include "mst.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int edge_cmp(const void *a, const void *b) {
    const Edge *ea = *(const Edge **)a;
    const Edge *eb = *(const Edge **)b;
    if (ea->w < eb->w) return -1;
    if (ea->w > eb->w) return 1;
    return 0;
}

static int find_root(int *parent, int x) {
    int r = x;
    while (parent[r] != r) r = parent[r];
    while (parent[x] != r) { int p = parent[x]; parent[x] = r; x = p; }
    return r;
}

void generate_mst(Node *start) {
    if (start == NULL) return;

    Node **stack = (Node **)malloc(sizeof(Node *) * MAX_NODES);
    if (!stack) return;
    int stack_sz = 0;

    Edge **elist = (Edge **)malloc(sizeof(Edge *) * MAX_EDGES);
    if (!elist) { free(stack); return; }
    int ecount = 0;

    /* BFS/DFS to discover all reachable nodes and collect unique edges */
    start->var = (void *)1;
    stack[stack_sz++] = start;

    for (int i = 0; i < stack_sz; ++i) {
        Node *n = stack[i];
        for (int j = 0; j < n->edge_count; ++j) {
            Edge *e = n->edges[j];
            if (e->var == NULL) {
                e->var = (void *)1; /* mark edge seen */
                elist[ecount++] = e;
            }
            Node *nei = (e->u == n) ? e->v : e->u;
            if (nei->var == NULL) {
                nei->var = (void *)1; /* mark node seen */
                stack[stack_sz++] = nei;
            }
        }
    }

    if (ecount == 0 || stack_sz == 0) {
        /* nothing to do */
        /* cleanup var flags */
        for (int i = 0; i < stack_sz; ++i) stack[i]->var = NULL;
        for (int i = 0; i < ecount; ++i) elist[i]->var = NULL;
        free(stack);
        free(elist);
        return;
    }

    /* Kruskal: sort edges by weight */
    qsort(elist, ecount, sizeof(Edge *), edge_cmp);

    /* Prepare union-find over node ids. Node ids are < MAX_NODES per header */
    int *parent = (int *)malloc(sizeof(int) * MAX_NODES);
    if (!parent) {
        for (int i = 0; i < stack_sz; ++i) stack[i]->var = NULL;
        for (int i = 0; i < ecount; ++i) elist[i]->var = NULL;
        free(stack); free(elist);
        return;
    }
    for (int i = 0; i < MAX_NODES; ++i) parent[i] = -1;
    for (int i = 0; i < stack_sz; ++i) {
        int id = stack[i]->id;
        parent[id] = id;
    }

    /* Optional rank/size array */
    int *rank = (int *)malloc(sizeof(int) * MAX_NODES);
    if (!rank) { free(parent); for (int i = 0; i < stack_sz; ++i) stack[i]->var = NULL; for (int i = 0; i < ecount; ++i) elist[i]->var = NULL; free(stack); free(elist); return; }
    for (int i = 0; i < MAX_NODES; ++i) rank[i] = 0;

    for (int i = 0; i < ecount; ++i) {
        Edge *e = elist[i];
        int a = e->u->id;
        int b = e->v->id;
        if (parent[a] == -1 || parent[b] == -1) continue; /* skip nodes outside discovered set */
        int ra = find_root(parent, a);
        int rb = find_root(parent, b);
        if (ra != rb) {
            /* union by rank */
            if (rank[ra] < rank[rb]) parent[ra] = rb;
            else if (rank[ra] > rank[rb]) parent[rb] = ra;
            else { parent[rb] = ra; rank[ra]++; }
            e->keep = 1;
        }
    }

    /* cleanup: reset var fields so main or judge won't see modified var pointers */
    for (int i = 0; i < stack_sz; ++i) stack[i]->var = NULL;
    for (int i = 0; i < ecount; ++i) elist[i]->var = NULL;

    free(parent);
    free(rank);
    free(stack);
    free(elist);
}
