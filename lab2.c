#include <stdio.h>
#include <stdlib.h>
//Find the length of string
int sizeofstring(char str[]) {
    int size = 0;
    while(*str) {
    ++size;
    ++str;
    }
    return size;
}
//Check the string for the correctness of the input
int checkstring(char p[]) {
    int digits[100];
    for (int i = 0; i < 100; ++i) {
        digits[i] = 0;
    }
    for (int i = 0; i < sizeofstring(p); ++i) {
        if (((p[i] - '0') >= 0) && ((p[i] - '0') <= 9)) {
            digits[p[i]-'0']++;
            if ((digits[p[i]-'0'] > 1) && (p[i]-'0' >= 0) && (p[i] - '0' <= 9)) {
                return -1;
            }
        } else {
            return -1;
        }
    }
    return 1;
}
//Converting a character to a number
int digit(char a) {
    return a - '0';
}
//Converting char array to int
void arraytoint(char p[], int size) {
    for (int i = 0; i < size; ++i) {
            p[i] = digit(p[i]);
        }
}
//search for the maximum index that is less than the following
int findmaxindex(char p[], int size) {
    for (int i = size-2; i >= 0; --i) {
        if (p[i] < p[i+1]) {
            return i;
        }
    }
    return -1;
}
//search for the index of element larger than the one found
int findbiggerelement(char p[], int element, int size) {
    for (int i = size-1; i >= 0; --i) {
        if (p[i] > element) {
            return i;
        }
    }
    return -1;
}

void permutation(char p[], int size) {
    int index = findmaxindex(p, size);
    if (index == -1) {
    } else {
        int element = p[index];
        int indexofbiggerelement = findbiggerelement(p, element, size);
        int temp = p[index];
        p[index] = p[indexofbiggerelement];
        p[indexofbiggerelement] = temp;
        char rev[11] = {' '};
        for (int i = 0; i <= index; ++i) {
            rev[i] = p[i];
        }
        int i = index+1;
        int j = size-1;
        while ((i < size) && (j >= index)) {
            rev[i] = p[j];
            ++i;
            --j;
        }
        for (int i = 0; i < size; ++i) {
            printf("%d", rev[i]);
        }
        printf("\n");
        for (int i = 0; i < size; ++i) {
            p[i] = rev[i];
        }
    }
}

int main(void) {
    char p[200] = {' '};
    int j = 0;
    char a;
    if (scanf("%c", &a)) {}
    while (a != '\n') {
        p[j] = a;
        ++j;
        if (scanf("%c", &a)) {}
    }
    int n;
    if (scanf("%d", &n)) {}
    if ((checkstring(p) == -1) || (n < 0)) {
        printf("bad input");
    } else {
        int size = sizeofstring(p);
        arraytoint(p, size);
        for (int k = n; k > 0; --k) {
            permutation(p, size);
        }
    }
    return 0;
}
