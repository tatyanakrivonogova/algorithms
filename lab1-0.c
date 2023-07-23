#include <stdio.h>

//Find the length of string
int SizeOfString(int str[]) {
    int size = 0;
    while(*str) {
        ++size;
        ++str;
    }
    return size;
}

//Get new batch of characters
int ReadingData(int * string, int SizeOfShift, int SizeOfPattern) {
    for (int i = 0; i < SizeOfPattern-SizeOfShift; ++i) {
        string[i] = string[i+SizeOfShift];
    }
    for (int i = SizeOfPattern-SizeOfShift; i < SizeOfPattern; ++i) {
        int b;
        b = getchar();
        if (b == EOF) {
            return -1;
        }
        string[i] = b;
    }
    return 0;
}

void ReadingOfPattern(int * pattern) {
    int j = 0;
    int a;
    a = getchar();
    while (a != (int) '\n') {
        pattern[j] = a;
        ++j;
        a = getchar();
    }
}

// Filling in the shift array
void FillingShiftArray(int * ShiftArray, int * pattern, int SizeOfPattern) {
    for (int i = 0; i < 256; ++i) {
        ShiftArray[i] = SizeOfPattern;
    }


    for (int i = SizeOfPattern - 2; i >= 0; --i) {
        if ((SizeOfPattern - 1 - i) < (ShiftArray[pattern[i]])) {
            ShiftArray[pattern[i]] = SizeOfPattern - 1 - i;
        }
    }
}

int main() {
    int pattern[20] = {' '};
    ReadingOfPattern(pattern);
    int SizeOfPattern = SizeOfString(pattern);
    int ShiftArray[256];
    FillingShiftArray(ShiftArray, pattern, SizeOfPattern);
    int string[20] = {' '};
    if (ReadingData(string, SizeOfPattern, SizeOfPattern) == -1) {
    } else {
        int i = SizeOfPattern - 1;
        while (1) {
            int j = SizeOfPattern - 1;
            int k = i;
            while (j >= 0) {
                printf("%d ", k+1);
                if (pattern[j] != string[j]) {
                    break;
                } else {
                    --k;
                    --j;
                }
            }
            i += ShiftArray[string[SizeOfPattern-1]];
            if ((ReadingData(string, ShiftArray[string[SizeOfPattern-1]], SizeOfPattern)) == -1) {
                return 0;
            }
        }
    }
    return 0;
}
