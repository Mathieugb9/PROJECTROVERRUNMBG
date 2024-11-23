#include <stdio.h>
#include "map.h"
#include "movement_tree.h"

void display_tree(TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) printf("  ");
    printf("Level %d: Cost = %d, Move = %d\n", level, node->cost, node->move);

    for (int i = 0; i < node->num_children; i++) {
        display_tree(node->children[i], level + 1);
    }
}

int main() {
    t_map map;

#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }

    displayMap(map);

    TreeNode* root = create_node(9, -1, NULL);

    TreeNode* child1 = create_node(8, 0, root);
    TreeNode* child2 = create_node(7, 1, root);
    TreeNode* child3 = create_node(6, 2, root);
    add_child(root, child1);
    add_child(root, child2);
    add_child(root, child3);

    TreeNode* child1_1 = create_node(7, 3, child1);
    TreeNode* child1_2 = create_node(6, 4, child1);
    add_child(child1, child1_1);
    add_child(child1, child1_2);

    TreeNode* child2_1 = create_node(6, 5, child2);
    TreeNode* child2_2 = create_node(5, 6, child2);
    add_child(child2, child2_1);
    add_child(child2, child2_2);

    printf("Tree structure:\n");
    display_tree(root, 0);

    free_tree(root);
    return 0;
}
