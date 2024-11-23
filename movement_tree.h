#ifndef MOVEMENT_TREE_H
#define MOVEMENT_TREE_H

typedef struct TreeNode {
    int cost;
    int move;
    struct TreeNode** children;
    int num_children;
    struct TreeNode* parent;
} TreeNode;

TreeNode* create_node(int node_cost, int movement_type, TreeNode* parent_node);
void add_child(TreeNode* parent_node, TreeNode* child_node);
void free_tree(TreeNode* root_node);
TreeNode* find_minimum_leaf(TreeNode* root_node);
void trace_path(TreeNode* leaf_node, int* movement_path, int* path_length);

#endif // MOVEMENT_TREE_H
