#include <stdlib.h>
#include <stdio.h>
#include "movement_tree.h"
#include <limits.h>

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

TreeNode* find_minimum_leaf(TreeNode* node) {
    if (node == NULL) return NULL;


    if (node->num_children == 0) {
        return node;
    }

    TreeNode* min_leaf = NULL;
    int min_cost = INT_MAX;

    for (int i = 0; i < node->num_children; i++) {
        TreeNode* leaf = find_minimum_leaf(node->children[i]);
        if (leaf != NULL && leaf->cost < min_cost) {
            min_cost = leaf->cost;
            min_leaf = leaf;
        }
    }

    return min_leaf;
}


void trace_path(TreeNode* leaf_node, int* movement_path, int* path_length) {
    if (leaf_node == NULL || movement_path == NULL || path_length == NULL) return;

    *path_length = 0;
    TreeNode* current = leaf_node;

    while (current->parent != NULL) {
        movement_path[(*path_length)++] = current->move;
        current = current->parent;
    }


    for (int i = 0; i < *path_length / 2; i++) {
        int temp = movement_path[i];
        movement_path[i] = movement_path[*path_length - i - 1];
        movement_path[*path_length - i - 1] = temp;
    }
}