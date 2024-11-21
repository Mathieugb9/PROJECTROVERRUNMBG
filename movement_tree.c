#include <stdio.h>
#include <stdlib.h>
#include "movement_tree.h"
#include "moves.h" // test n
#include "map.h"

TreeNode* create_node(int node_cost, int movement_type, TreeNode* parent_node) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->cost = node_cost;
    new_node->move = movement_type;
    new_node->children = NULL;
    new_node->num_children = 0;
    new_node->parent = parent_node;
    return new_node;
}

void add_child(TreeNode* parent_node, TreeNode* child_node) {
    if (parent_node == NULL || child_node == NULL) {
        return;
    }
    parent_node->num_children++;
    parent_node->children = realloc(parent_node->children, sizeof(TreeNode*) * parent_node->num_children);
    if (parent_node->children == NULL) {
        exit(EXIT_FAILURE);
    }
    parent_node->children[parent_node->num_children - 1] = child_node;
}

TreeNode* find_minimum_leaf(TreeNode* root_node) {
    if (root_node == NULL) {
        return NULL;
    }
    if (root_node->num_children == 0) {
        return root_node;
    }
    TreeNode* minimum_leaf = NULL;
    int minimum_cost = __INT_MAX__;
    for (int i = 0; i < root_node->num_children; i++) {
        TreeNode* leaf_node = find_minimum_leaf(root_node->children[i]);
        if (leaf_node->cost < minimum_cost) {
            minimum_cost = leaf_node->cost;
            minimum_leaf = leaf_node;
        }
    }
    return minimum_leaf;
}

void trace_path(TreeNode* leaf_node, int* movement_path, int* path_length) {
    if (leaf_node == NULL || movement_path == NULL || path_length == NULL) {
        return;
    }
    *path_length = 0;
    while (leaf_node != NULL) {
        movement_path[(*path_length)++] = leaf_node->move;
        leaf_node = leaf_node->parent;
    }
    for (int i = 0; i < (*path_length) / 2; i++) {
        int temp = movement_path[i];
        movement_path[i] = movement_path[*path_length - i - 1];
        movement_path[*path_length - i - 1] = temp;
    }
}
