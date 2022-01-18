#include <stdio.h>
#include "traverse.h"

void traverse(const Node *n)
{
    if (n == NULL)
        return;

    printf("value=%d\n", n->value);
    traverse(n->left);
    traverse(n->right);
}
