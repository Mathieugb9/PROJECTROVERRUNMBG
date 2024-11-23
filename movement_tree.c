#include <stdlib.h>
#include <stdio.h>
#include "movement_tree.h"

TreeNode* create_node(int node_cost, int movement_type, TreeNode* parent_node) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->cost = node_cost;
    new_node->move = movement_type;
    new_node->num_children = 0;
    new_node->children = NULL;
    new_node->parent = parent_node;
    return new_node;
}


void add_child(TreeNode* parent_node, TreeNode* child_node) {
    if (parent_node == NULL || child_node == NULL) return;

    parent_node->num_children++;
    parent_node->children = (TreeNode**)realloc(parent_node->children,
                                                parent_node->num_children * sizeof(TreeNode*));
    if (parent_node->children == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    parent_node->children[parent_node->num_children - 1] = child_node;
}


void free_tree(TreeNode* root_node) {
    if (root_node == NULL) return;

    for (int i = 0; i < root_node->num_children; i++) {
        free_tree(root_node->children[i]);
    }
    free(root_node->children);
    free(root_node);
}
