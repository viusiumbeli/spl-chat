#ifndef SPL_CHAT_LIST_H
#define SPL_CHAT_LIST_H


typedef struct node {
    int val;
    struct node *next;
} node_t;

void add_node(node_t *node, int value);

void print_list(node_t *node);

#endif //SPL_CHAT_LIST_H
