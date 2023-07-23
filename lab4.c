#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct stack{
    int data[10000];
    int size;
} stack;

int sizeofstr(char str[]) {
    int size = 0;
    while(*str) {
    ++size;
    ++str;
    }
    return size;
}

void push(stack *S, int x) {
    S->data[S->size] = x;
    S->size++;
}

int pop(stack *S) {
    if (S->size <= 0) {
        printf("syntax error");
        exit(0);
    }
    S->size--;
    return S->data[S->size];
}

int top(stack *S) {
    if (S->size <= 0) {
        printf("syntax error");
        exit(0);
    }
    return S->data[S->size - 1];
}

bool IsOperator(char c) {
    if ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '(') || (c == ')')) {
        return true;
    }
    return false;
}

bool IsDigit(char c) {
    if ((c >= '0') && (c <= '9')) {
        return true;
    }
    return false;
}

int GetPriority(char c) {
    if ((c == '(') || (c == ')')) {
        return 0;
    }
    else if ((c == '+') || (c == '-')) {
        return 1;
    }
    else {
        return 2;
    }
}

void postfix(char * expression, int size, char * result){
    int size1 = 0;
    stack * operstack = (stack*)malloc(sizeof(stack));
    operstack->size = 0;
    for (int i = 0; i < size; ++i) {
        if (IsDigit(expression[i])) {
            while ((IsOperator(expression[i])) == false){
                result[size1] = expression[i];
                ++size1;
                ++i;
                if (i == size) break;
            }
            result[size1] = ' ';
            ++size1;
            --i;
        }
        if (IsOperator(expression[i])) {
            if (expression[i] == '(') {
                push(operstack, (int) expression[i]);
            } else if (expression[i] == ')'){
                if ((i == 0) || (expression[i-1] == '(')){
                    printf("syntax error");
                    exit(0);
                }
                char c = (char) pop(operstack);
                while (c != '(') {
                    result[size1] = c;
                    result[size1 + 1] = ' ';
                    size1 += 2;
                    c = (char)pop(operstack);
                }
            } else {
                if (operstack->size > 0 && GetPriority(expression[i]) <= GetPriority((char)top(operstack))) {
                    result[size1] = (char)pop(operstack);
                    result[size1 + 1] = ' ';
                    size1 += 2;
                }
                push(operstack, (int) expression[i]);
            }
        }
    }
    while (operstack->size > 0){
        result[size1] = (char)pop(operstack);
        result[size1+1] = ' ';
        size1 += 2;
    }
    result[size1] = '\0';
    free(operstack);
}

int count(char * expression){
    int result = 0;
    stack * st = (stack*)malloc(sizeof(stack));
    st->size = 0;
    int sizeofstring = sizeofstr(expression)-1;
    for (int i = 0; i < sizeofstring; i++) {
        if (IsDigit(expression[i])) {
            int a = 0;
            while ((IsOperator(expression[i]) == false) && (expression[i] != ' ')) {
                a *= 10;
                a += (int) (expression[i] - '0');
                ++i;
                if (i == sizeofstring) {
                    break;
                }
            }
            push(st, a);
            i--;
        } else if (IsOperator(expression[i]) == true) {
            int a = pop(st);
            int b = pop(st);
            if (expression[i] == '+') {
                result = b + a;
            }
            if (expression[i] == '-') {
                result = b - a;
            }
            if (expression[i] == '*') {
                result = b * a;
            }
            if (expression[i] == '/') {
                if (a == 0) {
                    printf("division by zero");
                    exit(0);
                }
                result = b / a;
            }
            push(st, result);
        }
    }
    int t = top(st);
    free(st);
    return t;
}


int main() {

    char * expression = (char *)malloc(10000);
    char a;
    int i = 0;
    a = getchar();
    while ((a != '\n') && (a != '\0')) {
        expression[i] = a;
        if ((IsDigit(expression[i]) == false) && (IsOperator(expression[i]) == false)) {
            printf("syntax error");
            free(expression);
            return 0;
        }
        ++i;
        a = getchar();
    }
    int size = i;

    char * PostfixExpression = (char *)malloc(10000);
    postfix(expression, size, PostfixExpression);
    printf("%d", count(PostfixExpression));
    free(PostfixExpression);
    free(expression);
    return 0;
}
