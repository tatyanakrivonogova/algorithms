#include <stdio.h>
#include <string.h>

// Raising a number to a power
int power(int number, int degree) {
    if (degree == 0) {
        return 1;
    } else {
        int result = 1;
        for (int i = 0; i < degree; ++i) {
            result*=number;
        }
        return result;
    }
}

// Hash search of a string
int hash(unsigned char s[], int size) {
    int result = 0;
    int p = 1;
    for (int i = 0; i < size; ++i) {
        result += ((int) s[i] % 3) * p;
        p *= 3;
    }
    return result;
}

int main() {

    FILE * fin = fopen("in.txt", "r");
    FILE * fout = fopen("out.txt", "w");

    // Find the number of symbols in the file
    fseek(fin, 0, SEEK_END);
    long long fsize = ftell(fin)-2;
    fseek(fin, 0, SEEK_SET);

    // Reading of pattern
    unsigned char pattern[17] = {' '};
    int SizeOfPattern = 0;
    for (int i = 0; i < 17; ++i) {
        if (fscanf(fin, "%c", &pattern[i])) {}
        if (pattern[i] == '\n') {
            pattern[i] = '\0';
            SizeOfPattern = i;
            break;
        }
    }

    int HashOfPattern = hash(pattern, SizeOfPattern);
    fprintf(fout, "%d", HashOfPattern);

    // Reading of string
    unsigned char string[17];
    for (int i = 0; i < SizeOfPattern; ++i) {
        if (fscanf(fin, "%c", &string[i]) == EOF) {
            fclose(fin);
            fclose(fout);
            return 0;
        }
    }
    string[SizeOfPattern] = '\0';

    // Checking the beginning of string
    int index = 0;
    int HashOfString = hash(string, SizeOfPattern);
    if (HashOfPattern == HashOfString) {
        for (int i = index; i < SizeOfPattern; ++i) {
            fprintf(fout, " %d", i+1);
            if (pattern[i] != string[i]) {
                break;
            }
        }
    }

    int pow = power(3, SizeOfPattern-1);

    // Creating the buffer
    unsigned char buffer[1000];
    if (fread(buffer, sizeof(unsigned char), 1000, fin)) {}
    int k = 0;

    while (1) {
        ++index;
        HashOfString -= ((int) string[0] % 3);
        HashOfString /= 3;
        for (int i = 0; i < SizeOfPattern-1; ++i) {
            string[i] = string[i+1];
        }
        if (k >= 1000) {
            memset(buffer, 0, sizeof(buffer));
            if (fread(buffer, sizeof(unsigned char), 1000, fin)) {}
            k = 0;
        }
        if (index+SizeOfPattern >= fsize) {
            fclose(fin);
            fclose(fout);
            return 0;
        } else {
            string[SizeOfPattern-1] = buffer[k];
            ++k;
        }
        HashOfString += ((int) string[SizeOfPattern-1] % 3) * pow;
        if (HashOfPattern == HashOfString) {
            for (int i = 0; i < SizeOfPattern; ++i) {
                fprintf(fout, " %d", index+1);
                if (pattern[i] != string[i]) {
                    break;
                }
                index++;
            }
        }
    }
    return 0;
}
