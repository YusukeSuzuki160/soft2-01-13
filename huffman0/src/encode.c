#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "encode.h"

#define NSYMBOLS 256

static int symbol_count[NSYMBOLS];
// 以下このソースで有効なstatic関数のプロトタイプ宣言

// ファイルを読み込み、static配列の値を更新する関数
static void count_symbols(const char *filename);

// symbol_count をリセットする関数
static void reset_count(void);

// 与えられた引数でNode構造体を作成し、そのアドレスを返す関数
static Node *create_node(int symbol, int count, Node *left, Node *right);

// Node構造体へのポインタが並んだ配列から、最小カウントを持つ構造体をポップしてくる関数
// n は 配列の実効的な長さを格納する変数を指している（popするたびに更新される）
static Node *pop_min(int *n, Node *nodep[]);

// ハフマン木を構成する関数
static Node *build_tree(void);

static int get_char_code(char c)
{ // count_symbol用に、文字コードを10進数の整数に変換
    if (c == ',')
    {
        return get_char_code('Z') + 1;
    }
    if (c == '.')
    {
        return get_char_code('Z') + 2;
    }
    if (c == '\'')
    {
        return get_char_code('Z') + 3;
    }
    if (c == ' ')
    {
        return get_char_code('Z') + 4;
    }
    if (c == '\n')
    {
        return get_char_code('Z') + 5;
    }
    if (c == '\t')
    {
        return get_char_code('Z') + 6;
    }
    else if (c == '\r')
    {
        return get_char_code('Z') + 7;
    }
    else if (c >= 'a' && c <= 'z')
    {
        return (int)(c - 'a');
    }
    else
    {
        return (int)(get_char_code('z') + 1 + c - 'A');
    }
}

static void count_symbols(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open %s\n", filename);
        exit(1);
    }
    char *buf = (char *)malloc(sizeof(char));
    while (fread(buf, 1, 1, fp) != 0)
    {
        symbol_count[get_char_code(*buf)]++;
    }
    fclose(fp);
    free(buf);
}

static void reset_count(void)
{
    for (int i = 0; i < NSYMBOLS; i++)
        symbol_count[i] = 0;
}

static Node *create_node(int symbol, int count, Node *left, Node *right)
{
    Node *ret = (Node *)malloc(sizeof(Node));
    *ret = (Node){.symbol = symbol, .count = count, .left = left, .right = right};
    return ret;
}

static Node *pop_min(int *n, Node *nodep[])
{
    // Find the node with the smallest count
    // カウントが最小のノードを見つけてくる
    int argmin = 0;
    for (int i = 0; i < *n; i++)
    {
        if (nodep[i]->count < nodep[argmin]->count)
        {
            argmin = i;
        }
    }

    Node *node_min = nodep[argmin];

    // Remove the node pointer from nodep[]
    // 見つかったノード以降の配列を前につめていく
    for (int i = argmin; i < (*n) - 1; i++)
    {
        nodep[i] = nodep[i + 1];
    }
    // 合計ノード数を一つ減らす
    (*n)--;

    return &(*node_min);
}

static Node *build_tree(void)
{
    int n = 0;
    Node *nodep[NSYMBOLS];

    for (int i = 0; i < NSYMBOLS; i++)
    {
        // カウントの存在しなかったシンボルには何もしない
        if (symbol_count[i] == 0)
            continue;

        nodep[n++] = create_node(i, symbol_count[i], NULL, NULL);
    }

    const int dummy = -1; // ダミー用のsymbol を用意しておく
    while (n >= 2)
    {
        Node *node1 = pop_min(&n, nodep);
        Node *node2 = pop_min(&n, nodep);
        Node *new = (Node *)malloc(sizeof(Node));
        new->symbol = dummy;
        new->count = node1->count + node2->count;
        new->right = node1;
        new->left = node2;
        nodep[n] = new;
        n++;
    }
    // なぜ以下のコードで木を返したことになるか少し考えてみよう
    return (n == 0) ? NULL : nodep[0];
}

// Perform depth-first traversal of the tree
// 深さ優先で木を走査する
void traverse_tree(const int depth, Node *np)
{
    if (np == NULL || np->left == NULL)
        return;
    Node *np1 = np->right;
    Node *np2 = np->left;
    if (depth == 0)
    {
        np1->code[0] = '0';
        np2->code[0] = '1';
        np1->code[1] = 0;
        np2->code[1] = 0;
    }
    else
    {
        for (int i = 0; i < depth; i++)
        {
            np1->code[i] = np->code[i];
            np2->code[i] = np->code[i];
        }
        np1->code[depth] = '0';
        np2->code[depth] = '1';
        np1->code[depth + 1] = 0;
        np2->code[depth + 1] = 0;
    }
    traverse_tree(depth + 1, np1);
    traverse_tree(depth + 1, np2);
}

static void print_code(int code)
{
    if (code == -1)
    {
        printf(".");
    }
    else if (code == get_char_code(','))
    {
        printf("comma");
    }
    else if (code == get_char_code('.'))
    {
        printf("period");
    }
    else if (code == get_char_code('\''))
    {
        printf("quotation");
    }
    else if (code == get_char_code(' '))
    {
        printf("space");
    }
    else if (code == get_char_code('\n'))
    {
        printf("LF");
    }
    else if (code == get_char_code('\t'))
    {
        printf("tab");
    }
    else if (code == get_char_code('\r'))
    {
        printf("CR");
    }
    else if (code <= 'z' - 'a')
    {
        printf("%c", 'a' + code);
    }
    else
    {
        printf("%c", 'A' + code - 'z' - 1);
    }
}

void print_node(const Node *np, int flag)
{
    static int flags[100];
    static int line = 0;
    if (flag == -1)
    {
        for (int i = 0; i < 100; i++)
        {
            flags[i] = -1;
        }
    }
    for (int i = 0; i < 100; i++)
    {
        if (flags[i] == 0)
        {
            printf(" ");
        }
        else if (flags[i] == 1)
        {
            printf("|");
        }
        else if (flags[i] == -1)
        {
            break;
        }
    }
    if (flag == -1)
    {
        printf("|--");
    }
    else
    {
        printf("--");
    }

    if (np->right != NULL)
    {
        line += 3;
    }
    int now_line = line;
    print_code(np->symbol);
    if (np->right == NULL)
    {
        printf(": %s\n", np->code);
        return;
    }
    printf("\n");
    for (int i = 0; i < line; i++)
    {
        if (flags[i] == -1)
        {
            flags[i] = 0;
        }
    }

    flags[line] = 1;

    print_node(np->right, 0);
    line = now_line;
    print_node(np->left, 0);
    if (np != NULL)
    {
        line = now_line;
        flags[line] = -1;
        int m = line - 1;
        while (flags[m] == 0)
        {
            flags[m] = -1;
            m--;
        }
    }
}
// この関数は外部 (main) で使用される (staticがついていない)
Node *encode(const char *filename)
{
    reset_count();
    count_symbols(filename);
    Node *root = build_tree();
    printf("build tree Ok!\n");
    if (root == NULL)
    {
        fprintf(stderr, "A tree has not been constructed.\n");
    }
    fflush(stdout);
    return root;
}
