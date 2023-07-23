#include <stdio.h>

void Swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void AddElement(int heap[], int index, int element) {
    heap[index] = element;
    while ((index > 1) && (heap[index] > heap[index / 2])) {
        Swap(&heap[index], &heap[index / 2]);
        index /= 2;
    }
}

void RemoveElement(int heap[], int n) {
    heap[1] = heap[n];
    int index = 1;
    while (index*2 <= n) {
        int max = index * 2;
        if ((index * 2 + 1 <= n) && (heap[index * 2 + 1] > heap[max])) {
            max = index * 2 + 1;
        }
        if (heap[index] < heap[max]) {
            Swap(&heap[index], &heap[max]);
        }
        index = max;
    }
}

int main() {
    int n;
    if (scanf("%d", &n)) {}
    int heap[n+1];
    heap[0] = 0;
    int element;
    for (int i = 1; i <= n; ++i) {
        if (scanf("%d", &element)) {}
        AddElement(heap, i, element);
    }
    for (int i = n; i > 0; --i) {
        int temp = heap[1];
        RemoveElement(heap, i);
        heap[i] = temp;
    }
    for (int i = 1; i <= n; ++i) {
        printf("%d ", heap[i]);
    }
    printf("\n");
    return 0;
}
