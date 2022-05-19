#ifndef LAB4_B_TREE_H
#define LAB4_B_TREE_H

typedef struct Node{
    unsigned int key;
    char *value;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int colour; // 0 = Black, 1 = Red
} Node;

typedef struct Tree{
    Node *root;
} Tree;

extern Node *EPointer;

int insert(Tree *tree, unsigned int key, char *value);
int delete(Tree *tree, int key);
void printTree(Node *ptr, int level);
void bypass(Node *ptr, int size);
Node *find(Node *ptr, int key);
Node **findNearest(Tree *tree, int key);


#endif //LAB4_B_TREE_H
