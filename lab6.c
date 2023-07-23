#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    int h;
    struct node * left;
    struct node * right;
} node;


int height(node *tree){
    if (tree == NULL) {
        return 0;
    }
    return tree->h;
}

void updateheight(node *tree){
    if (height(tree->left) > height(tree->right)) {
        tree->h = height(tree->left)+1;
    } else {
        tree->h = height(tree->right)+1;
    }
}

int balancefactor(node *tree){
    return height(tree->right) - height(tree->left);
}

node * rightrotate(node *tree){
    node *temp = tree->left;
    tree->left = temp->right;
    temp->right = tree;
    updateheight(tree);
    updateheight(temp);
    return temp;
}

node * leftrotate(node *tree){
    node *temp = tree->right;
    tree->right = temp->left;
    temp->left = tree;
    updateheight(tree);
    updateheight(temp);
    return temp;
}

node * balance(node *tree){
    updateheight(tree);
    if (balancefactor(tree) == 2) {
        if (balancefactor(tree->right) < 0) {
            tree->right = rightrotate(tree->right);
        }
        return leftrotate(tree);
    }
    if (balancefactor(tree) == -2) {
        if (balancefactor(tree->left) > 0) {
            tree->left = leftrotate(tree->left);
        }
        return rightrotate(tree);
    }
    return tree;
}

node * add(node * tree, int x, node * free) {
    if (tree == NULL) {
        tree = free;
        tree->key = x;
        tree->h = 1;
        tree->left = tree->right = NULL;
        return tree;
    }
    if (x < tree->key) {
        tree->left = add(tree->left, x, free);
    }
    if (x > tree->key) {
        tree->right = add(tree->right, x, free);
    }
    if (x == tree->key) {
        if (height(tree->right) < height(tree->left)){
            tree->right = add(tree->right, x, free);
        } else {
            tree->left = add(tree->left, x, free);
        }
    }
    return balance(tree);
}


int main() {
    int n;
    if (scanf("%d", &n)){}
    if (n == 0) {
        printf("0");
        return 0;
    }
    node * memory = malloc(sizeof(node) * n);
    node * tree = NULL;
    int current;
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &current)){}
        tree = add(tree, current, &memory[i]);
    }
    printf("%d", height(tree));
    free(memory);
    return 0;
}
