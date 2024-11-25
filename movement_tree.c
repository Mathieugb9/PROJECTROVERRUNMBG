#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "movement_tree.h"
#include "map.h"
#include "loc.h"
#include "moves.h"
#include "Complexity.h"

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

t_localisation apply_move(t_localisation loc, t_move move, t_map* map) {
    if (loc.num_moves_allowed <= 0) {
        loc.pos.x = -1;
        loc.pos.y = -1;
        return loc;
    }

    if (move >= T_LEFT && move <= U_TURN) {
        loc.ori = rotate(loc.ori, move);
        loc.num_moves_allowed--;
        return loc;
    }

    t_localisation new_loc = translate(loc, move);

    if (!isValidLocalisation(new_loc.pos, map->x_max, map->y_max)) {
        new_loc.pos.x = -1;
        new_loc.pos.y = -1;
        return new_loc;
    }

    int soil_type = map->soils[new_loc.pos.y][new_loc.pos.x];

    if (soil_type == CREVASSE) {
        new_loc.pos.x = -1;
        new_loc.pos.y = -1;
        return new_loc;
    }

    if (soil_type == REG) {
        new_loc.num_moves_allowed = 4;
    } else {
        new_loc.num_moves_allowed = loc.num_moves_allowed - 1;
    }

    return new_loc;
}
void build_tree(TreeNode* node, t_localisation loc, t_map* map, t_move* moves, int depth, int max_depth) {
    if (depth >= max_depth) {
        return;
    }

    for (int i = 0; i < 9; i++) {
        t_localisation new_loc = apply_move(loc, moves[i], map);

        if (new_loc.pos.x == -1 && new_loc.pos.y == -1) {
            continue;
        }

        increment_operations(1);
        int cost = map->costs[new_loc.pos.y][new_loc.pos.x];
        TreeNode* child = create_node(cost, moves[i], node);
        add_child(node, child);

        build_tree(child, new_loc, map, moves, depth + 1, max_depth);
    }
}
TreeNode* construct_phase_tree(t_localisation start_loc, t_map* map) {
    t_move* moves = getRandomMoves(9);
    TreeNode* root = create_node(map->costs[start_loc.pos.y][start_loc.pos.x], -1, NULL);
    build_tree(root, start_loc, map, moves, 0, 5);
    free(moves);
    return root;
}


void display_optimal_route(TreeNode* root) {
    if (root == NULL) return;

    TreeNode* min_leaf = find_minimum_leaf(root);
    if (min_leaf == NULL) {
        printf("No valid path found.\n");
        return;
    }

    int movement_path[100];
    int path_length = 0;
    trace_path(min_leaf, movement_path, &path_length);

    printf("Optimal Route Found:\n");
    printf("Total Cost: %d\n", min_leaf->cost);
    printf("Moves: ");
    for (int i = 0; i < path_length; i++) {
        printf("%s ", getMoveAsString(movement_path[i]));
    }
    printf("\n");
}
