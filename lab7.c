#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Child{
    int key;
    int checked;
    struct Child * next_child;
} Child;

typedef struct Parent{
    int key;
    int numberchild;
    int checked;
    Child * first_child;
    Child * last_child;
} Parent;

typedef struct Graph{
    Parent * vexs;
} Graph;

Graph create_graph(int n) {
    Graph G;
    G.vexs = (Parent * )malloc(sizeof(Parent) * n);

    for (int i = 0; i < n; i++) {
        G.vexs[i].key = i;
        G.vexs[i].first_child = NULL;
        G.vexs[i].last_child = NULL;
        G.vexs[i].numberchild = 0;
        G.vexs[i].checked = 0;
    }
    return G;
}

Child create_child(int key) {
    Child new_child;
    new_child.key = key;
    new_child.checked = 0;
    new_child.next_child = NULL;
    return new_child;
}

void add_child(Graph G, Child * new_child, int from) {
    G.vexs[from].numberchild++;
    if (G.vexs[from].first_child == NULL) {
        G.vexs[from].first_child = new_child;
        G.vexs[from].last_child = new_child;
    } else {
        Child * temp = G.vexs[from].last_child;
        temp->next_child = new_child;
        G.vexs[from].last_child = new_child;
    }
}

void clear(Parent * vexs, Child * edges) {
    free(vexs);
    free(edges);
}

void dfs(Graph G, int * top_sorted, int * tail, Parent * vertex, bool * cycle) {
    if (vertex->checked == 2) {
        return;
    }
    vertex->checked = 1;
    for (int i = vertex->numberchild-1; i >= 0; i--) {
        Child * temp = vertex->first_child;
        for (int j = 0; j < i; j++) {
            temp = temp->next_child;
        }

        if (G.vexs[temp->key].checked == 1) {
            *cycle = true;
            return;
        }

        if (temp->checked == 0) {
            dfs(G, top_sorted, tail, &G.vexs[temp->key], cycle);
        }
    }

    vertex->checked = 2;
    top_sorted[*tail] = vertex->key;
    (*tail)++;
}

int main(){
    int n, m;
    if (scanf("%d", &n) != 1) {
        printf("bad number of lines");
        return 0;
    }

    if (scanf("%d", &m) != 1) {
        printf("bad number of lines");
        return 0;
    }

    if ((n < 0) || (n > 2000)) {
        printf ("bad number of vertices");
        return 0;
    }

    if ((m < 0) || (m > n*(n+1)/2)) {
        printf ("bad number of edges");
        return 0;
    }

    int top_sorted[n];
    int tail = 0;
    Graph G = create_graph(n);
    bool cycle = false;

    Child * edges = malloc(n * sizeof(Child));
    for (int i = 0; i < n; i++) {
        edges[i] = create_child(i);
    }

    for (int i = 0; i < m; i++){
        int from, to;
        if (scanf("%d %d", &from, &to) != 2){
            printf("bad number of lines");
            clear(G.vexs, edges);
            return 0;
        } else if ((from < 1) || (from > n) || (to < 1) || (to > n)) {
            printf("bad vertex");
            clear(G.vexs, edges);
            return 0;
        } else {
            from--;
            to--;
            add_child(G, &edges[to], from);
        }
    }
    for (int i = 0; i < n; i++) {
        dfs(G, top_sorted, &tail, &G.vexs[i], &cycle);
    }
    if (cycle == false) {
        for (int i = n-1; i >= 0; i--) {
            printf("%d ", top_sorted[i]+1);
        }
    } else {
        printf("impossible to sort");
    }
    clear(G.vexs, edges);
    return 0;
}
