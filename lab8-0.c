#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#define INT_MAX 2147483647

typedef struct Edge{
    int from;
    int to;
    int weight;
} Edge;

void Swap(Edge * a, Edge * b) {
    Edge temp = *a;
    *a = *b;
    *b = temp;

}

void QuickSort(Edge * edges, int left, int right) {
    if (left < right) {
        int k = left;
        int j = right;
        Edge pivot = edges[(left + right) / 2];
        while (k <= j) {
            while (edges[k].weight < pivot.weight) {
                ++k;
            }
            while (edges[j].weight > pivot.weight) {
                --j;
            }
            if (k >= j) {
                break;
            }
            Swap(&edges[k], &edges[j]);
            ++k;
            --j;
        }
        QuickSort(edges, left, j);
        QuickSort(edges, j+1, right);
    }
}

void make_set(int * parent, int * rank, int vertex) {
    parent[vertex] = vertex;
    rank[vertex] = 0;
}

int find_set(int * parent, int vertex) {
    if (vertex == parent[vertex]) {
        return vertex;
    }
    return parent[vertex] = find_set(parent, parent[vertex]);
}

void union_sets(int * parent, int * rank, int v1, int v2){
    int a = find_set(&parent[0], v1);
    int b = find_set(&parent[0], v2);
    if (a == b) {
        return;
    }
    if (rank[a] >= rank[b]) {
        parent[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }
    } else {
        parent[a] = b;
        if (rank[a] == rank[b]) {
            rank[b]++;
        }
    }

}

void clear(Edge * edges, int * parent, int * rank, int * mst) {
    free(edges);
    free(parent);
    free(rank);
    free(mst);
}

int main() {
    int n, m;
    if (scanf("%d", &n) == EOF) {
        printf("bad number of lines");
        return 0;
    }

    if (scanf("%d", &m) == EOF) {
        printf("bad number of lines");
        return 0;
    }

    if ((n < 0) || (n > 5000)) {
        printf("bad number of vertices");
        return 0;
    }

    if ((m < 0) || (m > n*(n+1)/2)) {
        printf ("bad number of edges");
        return 0;
    }

    if ((n == 0) || ((m == 0) && (n != 1))) {
        printf("no spanning tree");
        exit(0);
    }

    int * parent = malloc(n * sizeof(int));
    int * rank = malloc(n * sizeof(int));
    int * mst = malloc(m * sizeof(int));
    Edge * edges = malloc(m * sizeof(Edge));
    int index = 0;

    for (int i = 0; i < m; i++) {
        if (scanf("%d %d %d", &edges[i].from, &edges[i].to, &edges[i].weight) != 3){
            printf("bad number of lines");
            clear(&edges[0], &parent[0], &rank[0], &mst[0]);
            return 0;
        } else if ((edges[i].from < 1) || (edges[i].from > n) || (edges[i].to < 1) || (edges[i].to > n)) {
            printf("bad vertex");
            clear(&edges[0], &parent[0], &rank[0], &mst[0]);
            return 0;
        } else if ((edges[i].weight < 0) || (edges[i].weight > INT_MAX)) {
            printf("bad length");
            clear(&edges[0], &parent[0], &rank[0], &mst[0]);
            return 0;
        } else {
            edges[i].from--;
            edges[i].to--;
        }
    }

    if (m < n-1) {
        printf("no spanning tree");
        clear(&edges[0], &parent[0], &rank[0], &mst[0]);
        return 0;
    }

    QuickSort(&edges[0], 0, m-1);

    for (int i = 0; i < n; i++) {
        make_set(&parent[0], &rank[0], i);
    }

    for (int i = 0; i < m; i++) {
        if (find_set(&parent[0], edges[i].from) != find_set(&parent[0], edges[i].to)){
            union_sets(&parent[0], &rank[0], edges[i].from, edges[i].to);
            mst[index] = i;
            index++;
        }
    }

    if (index != n-1) {
        printf("no spanning tree");
        clear(&edges[0], &parent[0], &rank[0], &mst[0]);
        return 0;
    }

    for (int i = 0; i < index; i++) {
        printf("%d %d\n", edges[mst[i]].from+1, edges[mst[i]].to+1);
    }
    clear(&edges[0], &parent[0], &rank[0], &mst[0]);
    return 0;
}
