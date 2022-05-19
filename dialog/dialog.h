#ifndef LAB4_B_DIALOG_H
#define LAB4_B_DIALOG_H
#include "../tree/tree.h"

int getInt();
char *getStr();
int getOption(int optionNum, char *msg, char *errMsg, char **options);

void dialog();
void DAdd(Tree *tree);
void DFind(Tree *tree);
void DDelete(Tree *tree);
void DShow(Tree *tree);
void DBypass(Tree *tree);
void DTiming(Tree *tree);

#endif //LAB4_B_DIALOG_H
