#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#define INT_MAX 2147483647

typedef struct Edge{
    int from;
    int to;
} Edge;

int FindMinWeightEdge(int n, int * minweight, bool * used) {
    int minedge = -1;
    for (int j = 0; j < n; j++) {
        if (!used[j]){
            if ((minedge == -1) && (minweight[j] != -1)) {
                minedge = j;
            } else if ((minweight[j] != -1) && (minweight[j] < minweight[minedge])) {
                minedge = j;
            }
        }
    }
    return minedge;
}

void UpdateMinWeight(int v, int n, int ** matrix, int * minweight, int * edgeto) {
    for (int to = 0; to < n; to++) {
        if ((minweight[to] == -1) && (matrix[v][to] != -1)) {
            minweight[to] = matrix[v][to];
            edgeto[to] = v;
        } else if (matrix[v][to] < minweight[to]) {
            minweight[to] = matrix[v][to];
            edgeto[to] = v;
        }
    }
}

void AddEdge(int v, int * edgeto, Edge ** mst, int index, Edge * free) {
    Edge * newedge = free;
    newedge->from = v+1;
    newedge->to = edgeto[v]+1;
    mst[index] = newedge;
}

int CreateMst(int ** matrix, int * minweight, bool * used, int * edgeto, Edge ** mst, Edge * memory, int n) {
    int index = 0;
    minweight[0] = 0;

    for (int i = 0; i < n; i++) {
        int v = FindMinWeightEdge(n, minweight, used);
        if (v == -1) {
            return -1;
        }
        used[v] = true;
        if (edgeto[v] != -1) {
            if (v < edgeto[v]) {
                AddEdge(v, edgeto, mst, index, &memory[index]);
                index++;
            } else {
                AddEdge(v, edgeto, mst, index, &memory[index]);
                index++;
            }
        }
        UpdateMinWeight(v, n, matrix, minweight, edgeto);
    }
    return index;
}

void clear(int ** matrix, int * minweight, int * edgeto, bool * used, Edge ** mst, Edge * memory, int n) {
    for(int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(mst);
    free(matrix);
    free(minweight);
    free(edgeto);
    free(used);
    free(memory);
}

int main() {
    FILE* fin = fopen ("in.txt", "r");

    int n, m;
    if (fscanf(fin, "%d", &n) == EOF) {
        printf("bad number of lines");
        fclose(fin);
        return 0;
    }

    if (fscanf(fin, "%d", &m) == EOF) {
        printf("bad number of lines");
        fclose(fin);
        return 0;
    }

    if ((n < 0) || (n > 5000)) {
        printf("bad number of vertices");
        fclose(fin);
        return 0;
    }

    if ((m < 0) || (m > n*(n+1)/2)) {
        printf ("bad number of edges");
        fclose(fin);
        return 0;
    }

    if ((n == 0) || ((m == 0) && (n != 1))) {
        printf("no spanning tree");
        fclose(fin);
        exit(0);
    }
    bool * used;
    used = malloc(n*sizeof(bool));

    int * edgeto;
    edgeto = malloc(n*sizeof(int));

    int * minweight;
    minweight = malloc(n*sizeof(int));

    int ** matrix = malloc(n*sizeof(*matrix));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            matrix[i][j] = -1;
        }
        minweight[i] = -1;
        edgeto[i] = -1;
        used[i] = false;
    }

    Edge ** mst = malloc((n-1) * sizeof(Edge));
    Edge * memory = malloc((n-1) * sizeof(Edge));

    for (int i = 0; i < m; i++) {
        int from, to, weight;
        if (fscanf(fin, "%d %d %d", &from, &to, &weight) != 3){
            printf("bad number of lines");
            clear(matrix, minweight, edgeto, used, mst, memory, n);
            fclose(fin);
            return 0;
        } else if ((from < 1) || (from > n) || (to < 1) || (to > n)) {
            printf("bad vertex");
            clear(matrix, minweight, edgeto, used, mst, memory, n);
            fclose(fin);
            return 0;
        } else if ((weight < 0) || (weight > INT_MAX)) {
            printf("bad length");
            clear(matrix, minweight, edgeto, used, mst, memory, n);
            fclose(fin);
            return 0;
        } else {
            matrix[from-1][to-1] = weight;
            matrix[to-1][from-1] = weight;
        }
    }

    int index = CreateMst(matrix, minweight, used, edgeto, mst, memory, n);
    if (index == -1) {
        printf("no spanning tree");
        clear(matrix, minweight, edgeto, used, mst, memory, n);
        fclose(fin);
        return 0;
    }

    for (int i = 0; i < index; i++) {
        printf("%d %d\n", mst[i]->from, mst[i]->to);
    }

    clear(matrix, minweight, edgeto, used, mst, memory, n);
    fclose(fin);
    return 0;
}
