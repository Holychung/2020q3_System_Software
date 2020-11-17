#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int **parent;
    int max_level;
    int n;
} TreeAncestor;

TreeAncestor *treeAncestorCreate(int n, int *parent, int parentSize)
{
    TreeAncestor *obj = malloc(sizeof(TreeAncestor));
    obj->n = n;
    obj->parent = malloc(n * sizeof(int *));

    int max_level = 32 - __builtin_clz(n);
    for (int i = 0; i < n; i++) {
        obj->parent[i] = malloc(max_level * sizeof(int));
        memset(obj->parent[i], -1, max_level * sizeof(int));
    }
    for (int i = 0; i < parentSize; i++)
        obj->parent[i][0] = parent[i];

    for (int j = 1; j < max_level; j++) {
        int quit = 1;
        for (int i = 0; i < parentSize; i++) {
            obj->parent[i][j] = obj->parent[i][j - 1] == -1
                                    ? -1
                                    : obj->parent[obj->parent[i][j - 1]][j - 1];
            if (obj->parent[i][j] != -1) quit = 0;
        }
        if (quit) break;
    }
    obj->max_level = max_level;
    return obj;
}

// int treeAncestorGetKthAncestor(TreeAncestor *obj, int node, int k)
// {
//     int max_level = obj->max_level;
//     for (int i = 0; i < max_level && node != -1; ++i)
//         if (k & (1 << i))
//             node = obj->parent[node][i];
//     return node;
// }

int treeAncestorGetKthAncestor(TreeAncestor *obj, int node, int k)
{
    while (k && node != -1) {
        node = obj->parent[node][__builtin_ctz(k)];
        k ^= k & -k;
    }
    return node;
}

void treeAncestorFree(TreeAncestor *obj)
{
    for (int i = 0; i < obj->n; i++)
        free(obj->parent[i]);
    free(obj->parent);
    free(obj);
}

int main() {
    int parent[7];
    parent[0] = -1;
    for (int i = 1; i < 7; i++)
        parent[i] = i-1;
    // parent[] = -1 0 1 2 4 5 6
    int size = 7;
    TreeAncestor *obj = treeAncestorCreate(size, parent, size);
    printf("%d\n", treeAncestorGetKthAncestor(obj, 5, 4));
    // 1
    printf("%d\n", treeAncestorGetKthAncestor(obj, 5, 3));
    // 2
    printf("%d\n", treeAncestorGetKthAncestor(obj, 5, 256));
    // expected -1 but get 5 
    
    treeAncestorFree(obj);
    return 0;
}