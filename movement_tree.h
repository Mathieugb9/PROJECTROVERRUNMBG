#ifndef MOVEMENT_TREE_H
#define MOVEMENT_TREE_H

#include "map.h"
#include "loc.h"
#include "moves.h"

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
t_localisation apply_move(t_localisation loc, t_move move, t_map* map);
t_move* generate_random_moves(int N);
TreeNode* construct_phase_tree(t_localisation start_loc, t_map* map);
void build_tree(TreeNode* node, t_localisation loc, t_map* map, t_move* moves, int depth, int max_depth);
void display_tree(TreeNode* node, int level);

#endif // MOVEMENT_TREE_H
