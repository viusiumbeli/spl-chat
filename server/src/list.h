#ifndef SPL_CHAT_LIST_H
#define SPL_CHAT_LIST_H


struct node {
    int val;
    struct node *next;
};

void add_node(struct node  *node, int value);

void remove_node(struct node *node, int value);

void print_list(struct node *node);

#endif //SPL_CHAT_LIST_H
