#pragma once

typedef struct node Node;

struct node
{
    int symbol;
    int count;
    char code[100];
    Node *left;
    Node *right;
};

// ファイルをエンコードし木のrootへのポインタを返す
Node *encode(const char *filename);
// Treeを走査して表示する
void traverse_tree(const int depth, Node *root);

void print_node(const Node *np, int flag);
