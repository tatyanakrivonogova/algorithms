#include <stdio.h>
#include <string.h>
#include <math.h>
const int minnumbersystem = 2;
const int maxnumbersystem = 16;
// Raising the number to positive integer power
long long int intpower(int number, int degree) {
    if (degree == 0) {
        return 1;
    } else {
        long long int result = 1;
        for (int i = 0; i < degree; ++i) {
            result*=number;
        }
        return result;
    }
}
// Raising the number to negative integer power
long double doublepower(int number, int degree) {
    if (degree == 0) {
        return 1;
    }  else {
        long double result = 1;
        for (int i = 0; i > degree; --i) {
            result /= number;
        }
        return result;
    }
}
// Get the character code
int digit(char a) {
    if ((a >= '0') && (a <= '9')) {
        return a - '0';
    } else if ((a >= 'A') && (a <= 'F')) {
    return a-'A' + 10;
    } else if ((a >= 'a') && (a <= 'f')) {
    return a-'a' + 10;
    } else {
        return -1;
    }
}
// Find the length of string
int size(char str[]) {
    int size = 0;
    while(*str) {
    ++size;
    ++str;
    }
    return size;
}
// Count the number of symbols
int countofsymbols(char x[], char symbol) {
    int count = 0;
    for (int i = 0; i < size(x); i++) {
        if (x[i] == symbol) {
            count++;
        }
    }
    return count;
}
// Checking the input data
int checkinputdata(char x[], int b1, int b2) {
    for (int i = 0; i < size(x); i++) {
        if (((digit(x[i]) >= b1 || digit(x[i]) == -1) && (x[i] != '.')) || countofsymbols(x, '.') > 1) {
            return 0;
        }
    }
    if ((countofsymbols(x, '0') + countofsymbols(x, '.')) == size(x) && countofsymbols(x, '.') != 0) {
        return 0;
    }
    if (b1 < minnumbersystem || b1 > maxnumbersystem || b2 < minnumbersystem || b2 > maxnumbersystem) {
        return 0;
    }
    return 1;
}
// Get a symbol by code
int symbol(int a) {
    if ((a >= 0) && (a <= 9)) {
        return '0' + a;
    } else if ((a > 9) && (a < maxnumbersystem)) {
    return a + 'a' - 10;
    } else {
        return -1;
    }
}
// Find the point position
int find_pos(char x[]) {
    int pos = -1;
    for (int i = 0; x[i] != '\0'; i++) {
        if (x[i] == '.' || x[i] == ',') {
            pos = i;
        }
    }
    return pos;
}
// Converting the integer part to the decimal number system
long long int celtodec(int pos, char x[], int b1) {
    if (pos == -1) {
        long long int wholedec = 0;
        int i = 0;
        for (int j = size(x) - 1; j >= 0; j--) {
            wholedec += intpower(b1, i) * (digit(x[j]));
            i++;
        }
        return wholedec;
    } else {
        char cel[14] = {' '};
        strncpy (cel, x, pos);
        long long int celdec = 0;
        int i = 0;
        for (int j = size(cel) - 1; j >= 0; j--) {
            celdec += intpower(b1, i) * (digit(cel[j]));
            i++;
        }
        return celdec;
    }
}
// Converting the fractional part to the decimal number system
long double drobtodec(int pos, char x[], int b1) {
    long double drobdec = 0;
    int j = -1;
    for (int i = 0; i < size(x); ++i) {
    }
    for (int i = pos+1; i < size(x); i++) {
        drobdec += doublepower(b1, j) * (digit(x[i]));
        j--;
    }
    return drobdec;
}
// Converting the integer part to the desired number system
void decceltob2(long long int celdec, int b2) {
    char cel[100] = {' '};
    int i = 0;
    if (celdec == 0) {
        cel[0] = '0';
    }
    while (celdec > 0) {
        cel[i] = symbol(celdec%b2);
        i++;
        celdec /= b2;
    }
    for (int j = 0; j < size(cel)/2; j++) {
        int temp = cel[j];
        cel[j] = cel[size(cel)-j-1];
        cel[size(cel)-j-1] = temp;
    }
    printf("%s", cel);
}
// Converting the fractional part to the desired number system
void decdrobtob2(long double drobdec, int b2) {
    char drob[100] = {' '};
    int i = 0;
    int count = 0;
    while (count < 12) {
        drob[i] = symbol(trunc(drobdec*b2));
        i++;
        drobdec = drobdec*b2 - trunc(drobdec*b2);
        count++;
    }
    printf(".%s\n", drob);
}
int main(void) {
    char x[14] = {' '};
    int b1, b2;
    if (scanf("%d %d ", &b1, &b2) != 1) {}
    if (scanf("%13s", x)) {}
    if (checkinputdata(x, b1, b2) == 0) {
        printf("bad input");
    } else if (checkinputdata(x, b1, b2) == 1) {
        const int pos = find_pos(x);
        if (pos == -1) {
            decceltob2(celtodec(pos, x, b1), b2);
        } else {
            decceltob2(celtodec(pos, x, b1), b2);
            decdrobtob2(drobtodec(pos, x, b1), b2);
        }
    }
    return 0;
}
