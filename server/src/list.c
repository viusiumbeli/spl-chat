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

void print_list(node_t *node) {
    printf("{list}\n");
    while (node->next != NULL) {
        node = node->next;
        printf("%i\n", node->val);
    }
    printf("{list}\n");
}