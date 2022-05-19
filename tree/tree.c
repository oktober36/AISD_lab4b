#include "tree.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

Node EList = {
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    0};

Node *EPointer = &EList;

void leftRotate(Tree *tree, Node *x){
    Node *y = x->right;
    Node *p = x->parent;
    x->right = y->left;
    if (y->left != EPointer)
        y->left->parent = x;
    y->parent = p;
    if (p == EPointer)
        tree->root = y;
    else {
        if (p->left == x)
            p->left = y;
        else
            p->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(Tree *tree, Node *x) {
    Node *y = x->left;
    Node *p = x->parent;
    x->left = y->right;
    if (y->right != EPointer)
        y->right->parent = x;
    y->parent = p;
    if (p == EPointer)
        tree->root = y;
    else {
        if (p->left == x)
            p->left = y;
        else
            p->right = y;
    }
    y->right = x;
    x->parent = y;
}

Node *crateNode(unsigned int key, char *value){
    Node* out = malloc(sizeof (Node));
    if (!out)
        return NULL;
    out->key = key;
    out->value = strdup(value);
    if (!out->value){
        free(out);
        return NULL;
    }
    out->left = EPointer;
    out->right = EPointer;
    out->colour = 1;
    return out;
}

void insertFixup(Tree *tree, Node *x){
    Node *pp, *p1, *p2;
    while(x->parent->colour){
        p1 = x->parent;
        pp = p1->parent;
        if (p1 == pp->left){
            p2 = pp->right;
            if (p2->colour){
                p1->colour = 0;
                p2->colour = 0;
                pp->colour = 1;
                x = pp;
                continue;
            } else if (x == p1->right) {
                x = p1;
                leftRotate(tree, x);
                p1 = x->parent;
            }
            p1->colour = 0;
            pp->colour = 1;
            rightRotate(tree, pp);
        } else {
            p2 = pp->left;
            if (p2->colour){
                p1->colour = 0;
                p2->colour = 0;
                pp->colour = 1;
                x = pp;
                continue;
            } else if (x == p1->left) {
                x = p1;
                rightRotate(tree, x);
                p1 = x->parent;
            }
            p1->colour = 0;
            pp->colour = 1;
            leftRotate(tree, pp);
        }
    }
    tree->root->colour = 0;
}

int insert(Tree *tree, unsigned int key, char *value){
    Node *parent, **ptr;
    Node *node = crateNode(key, value);

    ptr = &(tree->root);
    parent = EPointer;
    while (*ptr != EPointer) {
        parent = *ptr;
        if (key < (*ptr)->key)
            ptr = &((*ptr)->left);
        else if (key > (*ptr) -> key)
            ptr = &((*ptr)->right);
        else
            return 0;
    }
    if (node == NULL)
        return -1;
    node->parent = parent;
    *ptr = node;
    insertFixup(tree, node);
    return 1;
}

void printTree(Node *ptr, int level){
    int i = level;
    char colour;
    if (ptr != EPointer){
        printTree(ptr->right, level + 2);
        while (i-- > 0)
            printf("  ");
        colour = ptr->colour? 'R' : 'B';
        printf("%d %c\n", ptr->key, colour);
        printTree(ptr->left, level + 2);
    }
}

Node *find(Node *ptr, int k)
{
    while (ptr != EPointer){
        if (ptr->key == k)
            return ptr;
        ptr = k < ptr->key ? ptr->left : ptr->right;
    }
    return NULL;
}

Node *findMin(Node *ptr)
{
    if (ptr == EPointer)
        return NULL;
    while (ptr->left != EPointer)
        ptr = ptr->left;
    return ptr;
}

void deleteFixup(Tree *tree, Node *x) {
    if (tree->root == EPointer)
        return;
    while (x != tree->root && !x->colour) {
        Node *p = x->parent;
        if (x == p->left) {
            Node *w = p->right;
            if (w->colour) {
                w->colour = 0;
                p->colour = 1;
                leftRotate(tree, p);
                w = p->right;
            }
            if (!w->left->colour && !w->right->colour) {
                w->colour = 1;
                x = p;
            } else {
                if (!w->right->colour) {
                    w->colour = 1;
                    w->left->colour = 0;
                    rightRotate(tree, w);
                    w = p->right;
                }
                w->colour = p->colour;
                p->colour = 0;
                w->right->colour = 0;
                leftRotate(tree, p);
                x = tree->root;
            }
        } else {
            Node *w = p->left;
            if (w->colour) {
                w->colour = 0;
                p->colour = 1;
                rightRotate(tree, p);
                w = p->left;
            }
            if (!w->right->colour && !w->left->colour) {
                w->colour = 1;
                x = p;
            } else {
                if (!w->left->colour) {
                    w->colour = 1;
                    w->right->colour = 0;
                    leftRotate(tree, w);
                    w = p->left;
                }
                w->colour = p->colour;
                p->colour = 0;
                w->left->colour = 0;
                rightRotate(tree, p);
                x = tree->root;
            }
        }
    }
    x->colour = 0;
}

int delete(Tree *tree, int k)
{
    Node **pRoot = &(tree->root);
    Node *y, *par, *ptr, *x = find(*pRoot, k);


    if (x == NULL)
        return 0;
    if (x->left == EPointer || x->right == EPointer)
        y = x;
    else
        y = findMin(x->right);
    if (y->left != EPointer)
        ptr = y->left;
    else
        ptr = y->right;
    par = y->parent;
    if (ptr != EPointer)
        ptr->parent = par;
    if (par == EPointer)
        *pRoot = ptr;
    else
    if (par->left == y)
        par->left = ptr;
    else
        par->right = ptr;
    if (y != x)
        x->key = y->key;
    deleteFixup(tree, x);
    free(y->value);
    free(y);
    return 1;
}

void bypass(Node *ptr, int size){
    if (ptr == EPointer)
        return;
    bypass(ptr->right, size);
    if ((ptr->key == 0 && size == 1) || floor(log10( (double)ptr->key)) + 1 == (double) size)
        printf("(%d : %s)\n", ptr->key, ptr->value);
    bypass(ptr->left, size);
}

Node **findNearest(Tree *tree, int k){
    Node *buf, *ptr = tree->root;
    int min, module, total = 0;

    buf = tree->root;
    if (buf == EPointer)
        return NULL;
    Node **out = calloc(4, sizeof(Node *));
    while (buf != EPointer){
        ptr = buf;
        if (ptr->key == k)
            break;
        buf = k < ptr->key ? ptr->left : ptr->right;
    }
    out[0] = ptr;
    out[1] = ptr->parent;
    out[2] = ptr->right;
    out[3] = ptr->left;
    min = -1;
    for (int i = 0; i < 4; i++) {
        if (out[i] == EPointer)
            continue;
        module = abs((int) out[i]->key - k);
        if (min == -1){
            min = module;
            out[0] = out[i];
            continue;
        }
        if (module == min){
            total = 1;
            out[1] = out[i];
            break;
        }
        if (module < min && module > 0){
            out[0] = out[i];
            min = module;
        }
    }
    out[total+1] = NULL;
    return out;
}




