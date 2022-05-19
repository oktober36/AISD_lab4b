#include <time.h>
#include "dialog.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"
#include "dialog.h"

const char *dialogMsgs[] = {"0. Quit", "1. Add", "2. Bypass", "3. Find", "4. Delete", "5. Show", "6. Timing"};
const char *findMsgs[] = {"0. Back", "1. By key", "2. Nearest"};
const char *addMsgs[] = {"0. Back", "1. Key", "2. File"};

void (*dialogFptr[])(Tree *) = {NULL, DAdd, DBypass, DFind, DDelete, DShow, DTiming};

int getInt() {
    int out, flag;
    char *error;
    error = "";
    do {
        flag = 1;
        puts(error);
        error = "Please enter positive number";
        if (!scanf("%d%*[ ]", &out))
            flag = 0;
        if (getchar() != '\n')
            flag = 0;
        if (out < 0)
            flag = 0;
        fflush(stdin);
    } while (!flag);
    return out;
}

char *getStr() {
    char *s, *buf = (char *) calloc(sizeof(char), 256);
    int flag;
    char *error;
    error = "";
    do {
        flag = 1;
        puts(error);
        error = "Please enter string without spaces";
        if (!scanf("%s%*[ ]", buf))
            flag = 0;
        if (getchar() != '\n')
            flag = 0;
        fflush(stdin);
    } while (!flag);
    s = (char *) calloc(sizeof(char), strlen(buf) + 1);
    strcpy(s, buf);
    free(buf);
    buf = NULL;
    return s;
}

int getOption(int optionNum, char *msg, char *errMsg, char **options) {
    int rc, i;
    char *error = "";
    do {
        puts(error);
        error = errMsg;
        for (i = 0; i < optionNum; ++i)
            puts(options[i]);
        puts(msg);
        rc = getInt();
    } while (rc < 0 || rc >= optionNum);
    return rc;
}

void dialog(Tree *tree) {
    int rc;
    while ((rc = getOption(7, "Select the command",
                           "Wrong command number, repeat", (char **) dialogMsgs))) {
        dialogFptr[rc](tree);
        if (rc == 6)
            return;
    }
}

void DAdd(Tree *tree) {
    int option;
    option = getOption(3, "Select the add1 option",
                       "Wrong option number, repeat", (char **) addMsgs);
    if (!option)
        return;
    if (option == 1) {
        int out, key;
        char *value;
        puts("Enter key");
        key = getInt();
        puts("Enter value");
        value = getStr();
        out = insert(tree, key, value);
        if (out == -1)
            puts("Memory Error");
        if (out == 0)
            puts("Duplicate key");
        free(value);
    } else {
        FILE *file;
        char *fileName, *value;
        int key;

        puts("Enter filename");
        fileName = getStr();
        file = fopen(fileName, "r");
        if (!file){
            puts("No such file");
            return;
        }

        value = calloc(256, sizeof (char ));
        while (fscanf(file, "%d%*[ ]", &key) != EOF && fscanf(file, "%s%*[ ]", value) != EOF)
            if (key >=0)
            insert(tree, key, value);
        free(value);
        fclose(file);
    }
}

void DFind(Tree *tree) {
    int option;
    int key;

    option = getOption(3, "Select the search option",
                       "Wrong option number, repeat", (char **) findMsgs);
    if (!option)
        return;
    puts("Enter key");
    key = getInt();
    if (option == 1) {
        Node *node;
        node = find(tree->root, key);
        if (!node)
            puts("Not found");
        else
            printf("(%d : %s)\n", key, node->value);
    } else if (option == 2) {
        Node **out = findNearest(tree, key);
        if (!out)
            puts("Not found");
        else
            for (int i = 0; out[i]; i++)
                printf("(%d : %s)\n", out[i]->key, out[i]->value);
    }
}

void DBypass(Tree *tree){
    int size;
    puts("Enter size");
    size = getInt();
    bypass(tree->root, size);
}

void DDelete(Tree *tree) {
    int key;

    puts("Enter the key");
    key = getInt();
    if (!delete(tree, key))
        puts("Not found");
}

void DShow(Tree *tree) {
    puts("---Your tree---");
    printTree(tree->root, 0);
}

void DTiming(Tree *tree) {
    puts("Step of nodes number: 1000; Search number: 10'000'000");
    srand(time(NULL));
    for (int i = 0; i < 10001; i++) {
        int key;
        char *value = "1";
        key = rand() % 10001;
        insert(tree, key, value);
        if (i % 1000 == 0) {
            clock_t oldTime = clock();
            for (int j = 0; j < 10000000; j++) {
                key = rand() % 10001;
                find(tree->root, key);
            }
            printf("%.3f\n", (float) (clock() - oldTime) / CLOCKS_PER_SEC);
        }
    }
}
