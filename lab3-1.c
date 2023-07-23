#include <stdio.h>
#include <malloc.h>

void Swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void QuickSort(int arr[], int left, int right) {
    if (left < right) {
        int k = left;
        int j = right;
        int pivot = arr[(left + right) / 2];
        while (k <= j) {
            while (arr[k] < pivot) {
                ++k;
            }
            while (arr[j] > pivot) {
                --j;
            }
            if (k >= j) {
                break;
            }
            Swap(&arr[k], &arr[j]);
            ++k;
            --j;
        }
        QuickSort(arr, left, j);
        QuickSort(arr, j+1, right);
    }
}

int main() {
    int n;
    if (scanf("%d", &n)) {}
    int *arr = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &arr[i])) {}
    }
    QuickSort(arr, 0, n-1);
    for (int i  =0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    free(arr);
    return 0;
}
