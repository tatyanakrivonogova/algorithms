#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#define INT_MAX 2147483647
#define NO_VALUE -1
#define OVER_INT_MAX -2

void FindMinSign(int * min_sign, int * min_vert, int * sign, bool * done, int n){
    *min_sign = NO_VALUE;
    *min_vert = NO_VALUE;
    for (int i = 0; i < n; i++) {
        if (done[i] == false) {
            if ((*min_sign == NO_VALUE) && (sign[i] != NO_VALUE)){
                *min_sign = sign[i];
                *min_vert = i;
            } else if ((sign[i] != NO_VALUE) && (sign[i] < *min_sign) && (sign[i] != OVER_INT_MAX)) {
                *min_sign = sign[i];
                *min_vert = i;
            }
        }
    }
}

int CheckOverflow(int ** matrix, int * sign, int n, int finish) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if ((matrix[i][finish] != NO_VALUE) && (((sign[i] == INT_MAX) && (matrix[i][finish] > 0)) || ((sign[i] > INT_MAX) && (matrix[i][finish] == INT_MAX)))) {
            count++;
        }
        if (count == 2) {
            break;
        }
    }
    if (count == 2) {
        return 0;
    } else {
        return 1;
    }
}

bool ExceedIntMax(int ** matrix, int * sign, int min_vert, int j) {
    return ((matrix[min_vert][j] == INT_MAX) && (sign[min_vert] > 0)) || ((sign[min_vert] == INT_MAX) && (matrix[min_vert][j] > 0)) || (sign[min_vert] == OVER_INT_MAX);
}

void Clear(int ** matrix, int * sign, bool * done, int * routes, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(sign);
    free(done);
    free(routes);
}

int main() {
    FILE* fin = fopen ("in.txt", "r");

    int n, m, start, finish;
    if (fscanf(fin, "%d", &n) == EOF) {
        printf("bad number of lines");
        fclose(fin);
        return 0;
    }

    if (fscanf(fin, "%d", &start) == EOF) {
        printf("bad number of lines");
        fclose(fin);
        return 0;
    }

    if (fscanf(fin, "%d", &finish) == EOF) {
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

    if ((start < 1) || (start > n)) {
        printf("bad vertex");
        fclose(fin);
        return 0;
    }

    if ((finish < 1) || (finish > n)) {
        printf("bad vertex");
        fclose(fin);
        return 0;
    }

    if ((m < 0) || (m > n*(n-1)/2)) {
        printf ("bad number of edges");
        fclose(fin);
        return 0;
    }
    start--;
    finish--;

    bool * done;
    done = malloc(n*sizeof(bool));

    int * sign;
    sign = malloc(n*sizeof(int));

    int * routes;
    routes = malloc(n*sizeof(int));

    int ** matrix = malloc(n*sizeof(*matrix));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = NO_VALUE;
            }
        }
        routes[i] = NO_VALUE;
        sign[i] = NO_VALUE;
        done[i] = false;
    }

    for (int i = 0; i < m; i++) {
        int from, to, weight;
        if (fscanf(fin, "%d %d %d", &from, &to, &weight) != 3){
            printf("bad number of lines");
            fclose(fin);
            Clear(matrix, sign, done, routes, n);
            return 0;
        } else if ((from < 1) || (from > n) || (to < 1) || (to > n)) {
            printf("bad vertex");
            fclose(fin);
            Clear(matrix, sign, done, routes, n);
            return 0;
        } else if ((weight < 0) || (weight > INT_MAX)) {
            printf("bad length");
            fclose(fin);
            Clear(matrix, sign, done, routes, n);
            return 0;
        } else {
            matrix[from-1][to-1] = weight;
            matrix[to-1][from-1] = weight;
        }
    }
    sign[start] = 0;
    for (int curr_vertex = 0; curr_vertex < n; curr_vertex++) {
        int min_sign;
        int min_vert;
        FindMinSign(&min_sign, &min_vert, sign, done, n);
        if (min_vert != NO_VALUE){
            for (int j = 0; j < n; j++) {
                if (done[j] == 0) {
                    if ((matrix[min_vert][j] != NO_VALUE) && ((matrix[min_vert][j] < sign[j]) || (sign[j] == NO_VALUE) || (sign[j] == OVER_INT_MAX))){
                        if (ExceedIntMax(matrix, sign, min_vert, j)){
                            sign[j] = OVER_INT_MAX;
                            if (min_vert != j) {
                                routes[j] = min_vert;
                            }
                        } else {
                            sign[j] = matrix[min_vert][j] + sign[min_vert];
                            if (min_vert != j) {
                                routes[j] = min_vert;
                            }
                        }
                    }
                }
            }
            done[min_vert] = 1;
        }
    }


    for (int i = 0; i < n; i++) {
        if (sign[i] == NO_VALUE) {
            printf("oo ");
        } else if (sign[i] == OVER_INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%d ", sign[i]);
        }
    }
    printf("\n");

    if ((m == 0) && (start != finish)) {
        printf("no path");
        fclose(fin);
        Clear(matrix, sign, done, routes, n);
        return 0;
    }

    if ((sign[finish] == OVER_INT_MAX) && (CheckOverflow(matrix, sign, n, finish) == 0)) {
        printf("overflow");
        fclose(fin);
        Clear(matrix, sign, done, routes, n);
        return 0;
    }
    printf("%d ", finish+1);
    while (finish != start) {
        printf("%d ", routes[finish]+1);
        finish = routes[finish];
    }

    fclose(fin);
    Clear(matrix, sign, done, routes, n);
    return 0;
}
