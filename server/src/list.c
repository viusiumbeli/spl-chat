#include <wchar.h>
#include <malloc.h>
#include "list.h"

void add_node(node_t *node, int value) {
    while (node->next != NULL) {
        node = node->next;
    }

    node->next = malloc(sizeof(node_t));
    if (node->next != NULL) {
        node = node->next;
        node->val = value;
        node->next = NULL;
    }

}

void remove_node(node_t *node, int value) {
    node_t *previous;
    while (node->next != NULL) {
        previous = node;
        node = node->next;
        if (node->val == value) {
            previous->next = node->next;
        }
    }
}

void print_list(node_t *node) {
    printf("{connections}\n");
    while (node->next != NULL) {
        node = node->next;
        printf("%i\n", node->val);
    }
    printf("{connections}\n");
}