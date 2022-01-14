#include <stdio.h>
#include <stdlib.h>
#include "traverse.h"


int main(int argc, char **argv)
{
    // ノードを10個ほど定義
    Node n1  = { .value = 1, .left = NULL, .right = NULL};
    Node n2  = { .value = 2, .left = NULL, .right = NULL };
    Node n3  = { .value = 3, .left = NULL, .right = NULL };
    Node n4  = { .value = 4, .left = NULL, .right = NULL };
    Node n5  = { .value = 5, .left = NULL, .right = NULL };
    Node n6  = { .value = 6, .left = NULL, .right = NULL };
    Node n7  = { .value = 7, .left = NULL, .right = NULL };
    Node n8  = { .value = 8, .left = NULL, .right = NULL };
    Node n9  = { .value = 9, .left = NULL, .right = NULL };
    Node n10 = { .value =10, .left = NULL, .right = NULL };
    
    // 木を繋いでみる。木の形はページ参照
    
    
    // 走査する
    traverse(&n1);

    return EXIT_SUCCESS;
}
