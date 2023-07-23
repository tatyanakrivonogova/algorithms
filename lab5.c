#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INF 2147483647
#define size_of_buffer 10000
#define number_of_symbols 256

int LengthOfString(unsigned char str[]) {
    int size = 0;
    while(*str) {
        ++size;
        ++str;
    }
    return size;
}


void CopyString(unsigned char * a, unsigned char * b) {
    for (int i = 0; i < LengthOfString(b); i++) {
        a[i] = b[i];
    }
}


typedef struct Node{
    unsigned char symbol;
    unsigned char * code;
    int freq;
    bool leaf;
    struct Node * left;
    struct Node * right;
} Node;


int Power(int base, int degree) {
    int res = 1;
    for (int i = 0; i < degree; i++) {
        res *= base;
    }
    return res;
}


void CharToBin (unsigned char c, unsigned char * bin_code) {
    int k = 128;
    for (int i = 0; i < 8; i++) {
        if ((c&k) == 0) {
            bin_code[i] = '0';
        } else {
            bin_code[i] = '1';
        }
        k>>=1;
    }
    bin_code[8] = '\0';
}


unsigned char BinToChar (const unsigned char * bin) {
    int res = 0;
    for (int i = 0; i < 8; i++) {
        if (bin[i] == '1') {
            res += (int) Power(2, 7-i);
        }
    }
    return (unsigned char)res;
}


int PrintBin (unsigned char * binary_text, unsigned char * bin_zip, FILE* out) {
    int size = LengthOfString(binary_text);
    int index_bin = 0;
    int index_zip = 0;
    int k = 0;

    while (index_bin < size) {
        unsigned char * bin8 = (unsigned char *)calloc(9, sizeof(unsigned char));
        for (int i = 0; i < 8; i++) {
            if (index_bin >= size) {
                index_bin++;
                k++;
            } else {
                bin8[i] = binary_text[index_bin];
                index_bin++;
            }
        }
        bin_zip[index_zip] = BinToChar(bin8);
        free(bin8);
        index_zip++;
    }
    fwrite(bin_zip, sizeof(unsigned char), index_zip, out);
    return k;
}


Node * BuildZipTree(Node * nodes, int num_of_symbols) {
    Node * min1;
    Node * min2;
    if (nodes[0].freq < nodes[1].freq) {
        min1 = &nodes[0];
        min2 = &nodes[1];
    } else {
        min1 = &nodes[1];
        min2 = &nodes[0];
    }

    for (int i = 2; i < num_of_symbols; i++) {
        if ((nodes[i].freq <= min1->freq) && (nodes[i].freq <= min2->freq)) {
            min2 = min1;
            min1 = &nodes[i];
        } else if (nodes[i].freq <= min2->freq) {
            min2 = &nodes[i];
        }
    }
    if (min2->freq == INF) {
        return min1;
    }

    nodes[num_of_symbols].left = min1;
    nodes[num_of_symbols].right = min2;
    nodes[num_of_symbols].leaf = false;
    nodes[num_of_symbols].freq = min1->freq + min2->freq;
    nodes[num_of_symbols].symbol = min1->symbol;
    num_of_symbols++;

    min1->freq = INF;
    min2->freq = INF;
    return BuildZipTree(nodes, num_of_symbols);
}


void GetCode(Node * currentnode, unsigned char currentcode[], int index) {
    if (currentnode->leaf == true){
        if (index == 0) {
            unsigned char t[3] = "1\0";
            CopyString(currentnode->code, t);
            return;
        }
        currentcode[index] = '\0';
        CopyString(currentnode->code, currentcode);
        return;
    }

    currentcode[index] = '0';
    GetCode(currentnode->left, currentcode, index+1);
    currentcode[index] = '1';
    GetCode(currentnode->right, currentcode, index+1);
}


void EncodeTree(Node * current_node, unsigned char * encoded_tree, int *current_size) {
    if (current_node->leaf == true) {
        encoded_tree[*current_size] = '0';
        (*current_size)++;
        unsigned char bin_code[9];
        CharToBin(current_node->symbol, &bin_code[0]);
        for (int i = 0; i < 8; i++) {
            encoded_tree[*current_size+i] = bin_code[i];
        }
        *current_size += 8;
        return;
    }

    encoded_tree[*current_size] = '1';
    (*current_size)++;
    if (current_node->left->leaf == 1) {
        encoded_tree[*current_size] = '0';
        (*current_size)++;
        unsigned char bin_code[9];
        CharToBin(current_node->left->symbol, &bin_code[0]);
        for (int i = 0; i < 8; i++) {
            encoded_tree[*current_size + i] = bin_code[i];
        }
        *current_size += 8;
    }
    else {
        EncodeTree(current_node->left, encoded_tree, current_size);
    }

    if (current_node->right->leaf) {
        encoded_tree[*current_size] = '0';
        (*current_size)++;
        unsigned char bin_code[9];
        CharToBin(current_node->right->symbol, &bin_code[0]);
        for (int i = 0; i < 8; i++) {
            encoded_tree[*current_size + i] = bin_code[i];
        }
        *current_size += 8;
    }
    else {
        EncodeTree(current_node->right, encoded_tree, current_size);
    }
}


void Zip(FILE * in, FILE * out) {
    Node * nodes = (Node *)malloc(sizeof(Node)*512);
    for (int i = 0; i < number_of_symbols; i++) {
        nodes[i].symbol = (unsigned char) i;
        nodes[i].code = (unsigned char *)calloc(100, sizeof(char));
        nodes[i].freq = 0;
        nodes[i].leaf = false;
        nodes[i].left = NULL;
        nodes[i].right = NULL;
    }

    unsigned char * text = (unsigned char *)malloc(sizeof(unsigned char)*size_of_buffer);
    int size_of_text = 0;
    while (true) {
        int input = fread(text, sizeof(unsigned char), size_of_buffer, in);
        if (input == 0) {
            break;
        }

        for (int i = 0; i < input; i++) {
            nodes[(int)text[i]].freq++;
            nodes[(int)text[i]].leaf = true;
            size_of_text++;
        }
    }
    if (size_of_text == 0) {
        free(text);
        exit(0);
    }

    for (int i = 0; i < number_of_symbols; i++) {
        if (nodes[i].freq == 0) {
            nodes[i].freq = INF;
        }
    }

    int num_of_symbols = number_of_symbols;
    Node * root = BuildZipTree(&nodes[0], num_of_symbols);

    unsigned char code[100];
    GetCode(root, code, 0);

    unsigned char * bin_zip = (unsigned char*)calloc((8*(size_of_buffer + 1)) + 1, sizeof(char));
    int codesize = 0;
    EncodeTree(root, bin_zip, &codesize);

    fseek(in, 0, SEEK_SET);
    size_of_text = fread(text, sizeof(unsigned char), size_of_buffer, in);

    for (int i = 1; i < size_of_text; i++) {
        for (int j = 0; j < LengthOfString(nodes[text[i]].code); j++) {
            bin_zip[codesize] = nodes[text[i]].code[j];
            bin_zip[codesize + 1] = '\0';
            codesize++;
            if (codesize == 8*size_of_buffer) {
                PrintBin(bin_zip, bin_zip, out);
                codesize = 0;
            }
        }
        if (i == size_of_text - 1) {
            size_of_text = fread(text, sizeof(unsigned char), size_of_buffer, in);
            i = -1;
        }
    }

    unsigned char * zipped_text = (unsigned char *)malloc(sizeof(unsigned char)*size_of_buffer);
    unsigned char last[1];
    last[0] = (unsigned char)(PrintBin(bin_zip, zipped_text, out) + '0');
    fwrite(last, sizeof(unsigned char), 1, out);

    free(text);
    free(bin_zip);
    free(zipped_text);
    for (int i = 0; i < num_of_symbols; i++) {
        free(nodes[i].code);
    }
    free(nodes);
}


void BuildUnzipTree(unsigned char * encoded_tree, Node * nodes, Node * current_node, int * index, int * free_index) {
    if (encoded_tree[*index] == '0') {
        current_node->leaf = true;
        (*index)++;
        unsigned char bin[9];
        for (int i = 0; i < 8; i++) {
            bin[i] = encoded_tree[*index];
            (*index)++;
        }
        current_node->symbol = BinToChar(bin);
    } else {
        current_node->leaf = false;
        (*index)++;
        current_node->left = &nodes[*free_index];
        (*free_index)++;
        BuildUnzipTree(encoded_tree, nodes, current_node->left, index, free_index);
        current_node->right = &nodes[*free_index];
        (*free_index)++;
        BuildUnzipTree(encoded_tree, nodes, current_node->right, index, free_index);
    }
}


void DecodeText (FILE * out, FILE * in, unsigned char * bin, int * index, Node * root, int size, int last_byte, unsigned char * zipped_text) {
    Node * current_node = root;
    unsigned char * output = (unsigned char *)malloc(sizeof(unsigned char)*(8*size_of_buffer));
    int index_out = 0;

    while (*index < size) {
        if (((size - (*index)) == last_byte) && (size < 8*(size_of_buffer-1))) {
            fwrite(output, sizeof(unsigned char), index_out, out);
            break;
        }

        if (root->leaf == true) {
            output[index_out] = root->symbol;
            output[index_out + 1] = '\0';
            index_out++;
        }

        else {
            if (bin[*index] == '1') {
                current_node = current_node->right;
            } else {
                current_node = current_node->left;
            }

            if (current_node->leaf) {
                output[index_out] = current_node->symbol;
                index_out++;
                current_node = root;
            }
        }

        if (*index == size-1) {
            fwrite(output, sizeof(unsigned char), index_out, out);
            int fsize = (int) fread(zipped_text, sizeof(unsigned char), (int)(size_of_buffer-1), in);
            if (fsize == 0) {
                break;
            }
            unsigned char bin_code[9];
            CharToBin((unsigned char)(last_byte + '0'), &bin_code[0]);
            for (int j = 0; j < 9; j++) {
                bin[j] = bin_code[j];
            }
            size = 8;

            for (int i = 0; i < fsize - 1; i++) {
                CharToBin(zipped_text[i], &bin_code[0]);
                for (int j = 0; j < 9; j++) {
                    bin[size + j] = bin_code[j];
                }
                size += 8;
            }
            last_byte = zipped_text[fsize - 1] - '0';

            *index = -1;
            index_out = 0;
        }
        (*index)++;
    }
    free(output);
}


void Unzip (FILE* in, FILE* out) {
    unsigned char* zipped_text = (unsigned char *)malloc(sizeof(unsigned char)*size_of_buffer);
    unsigned char* bin_zip = (unsigned char *)malloc(sizeof(unsigned char)*(8*size_of_buffer));
    int unzip_size = 0;
    int last_byte = 0;

    int fsize = (int) fread(zipped_text, sizeof(unsigned char), size_of_buffer, in);
    for (int i = 0; i < fsize-1; i++) {
        unsigned char bin_code[9];
        CharToBin(zipped_text[i], &bin_code[0]);
        for (int j = 0; j < 9; j++) {
            bin_zip[unzip_size + j] = bin_code[j];
        }
        unzip_size += 8;
    }
    last_byte = zipped_text[fsize - 1] - '0';

    if (unzip_size == 0) {
        exit(0);
    }

    int index = 0;
    Node* nodes = (Node *)malloc(sizeof(Node)*512);

    int free_index = 1;
    BuildUnzipTree(bin_zip, &nodes[0], &nodes[0], &index, &free_index);
    DecodeText(out, in, bin_zip, &index, &nodes[0], unzip_size, last_byte, zipped_text);

    free(bin_zip);
    free(zipped_text);
    free(nodes);
}


int main() {
    FILE *in = fopen("in.txt", "rb");
    FILE *out = fopen("out.txt", "wb");

    unsigned char first[1];
    if (fread(first, sizeof(unsigned char), 1, in)){};
    if (first[0] == 'c') {
        Zip(in, out);
    }
    if (first[0] == 'd') {
        Unzip(in, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
