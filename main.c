#include <stdio.h>
#include "map.h"
#include "movement_tree.h"
void interactive_test() { // temporary test function to test the tree node creation
    printf("\n--- Interactive Movement Tree Test ---\n");

    TreeNode* root = NULL;
    while (1) {
        printf("\nChoose an action:\n");
        printf("1. Create root node\n");
        printf("2. Add child to a node\n");
        printf("3. Find minimum-cost leaf\n");
        printf("4. Trace path from a leaf to root\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 5) {
            printf("Exiting interactive test.\n");
            break;
        }

        switch (choice) {
            case 1: {
                if (root != NULL) {
                    printf("Root already exists! You can only create one root.\n");
                } else {
                    int cost, move;
                    printf("Enter root cost: ");
                    scanf("%d", &cost);
                    printf("Enter root move (e.g., 0 for MOVE_FORWARD): ");
                    scanf("%d", &move);
                    root = create_node(cost, move, NULL);
                    printf("Root node created with cost %d and move %d.\n", cost, move);
                }
                break;
            }
            case 2: {
                if (root == NULL) {
                    printf("You need to create a root node first!\n");
                } else {
                    int parent_cost, cost, move;
                    printf("Enter parent node cost to attach the child: ");
                    scanf("%d", &parent_cost);

                    TreeNode* parent = NULL;
                    TreeNode* stack[100];
                    int top = 0;
                    stack[top++] = root;

                    while (top > 0) {
                        TreeNode* node = stack[--top];
                        if (node->cost == parent_cost) {
                            parent = node;
                            break;
                        }
                        for (int i = 0; i < node->num_children; i++) {
                            stack[top++] = node->children[i];
                        }
                    }

                    if (parent == NULL) {
                        printf("Parent node with cost %d not found!\n", parent_cost);
                    } else {
                        printf("Enter child cost: ");
                        scanf("%d", &cost);
                        printf("Enter child move (e.g., 0 for MOVE_FORWARD): ");
                        scanf("%d", &move);
                        TreeNode* child = create_node(cost, move, parent);
                        add_child(parent, child);
                        printf("Child node added with cost %d and move %d.\n", cost, move);
                    }
                }
                break;
            }
            case 3: {
                if (root == NULL) {
                    printf("You need to create a root node first!\n");
                } else {
                    TreeNode* min_leaf = find_minimum_leaf(root);
                    if (min_leaf != NULL) {
                        printf("Minimum leaf found with cost %d and move %d.\n", min_leaf->cost, min_leaf->move);
                    } else {
                        printf("No minimum leaf found.\n");
                    }
                }
                break;
            }
            case 4: {
                if (root == NULL) {
                    printf("You need to create a root node first!\n");
                } else {
                    int leaf_cost;
                    printf("Enter leaf node cost to trace the path: ");
                    scanf("%d", &leaf_cost);

                    TreeNode* leaf = NULL;
                    TreeNode* stack[100];
                    int top = 0;
                    stack[top++] = root;

                    while (top > 0) {
                        TreeNode* node = stack[--top];
                        if (node->cost == leaf_cost) {
                            leaf = node;
                            break;
                        }
                        for (int i = 0; i < node->num_children; i++) {
                            stack[top++] = node->children[i];
                        }
                    }

                    if (leaf == NULL) {
                        printf("Leaf node with cost %d not found!\n", leaf_cost);
                    } else {
                        int path[100];
                        int path_length = 0;
                        trace_path(leaf, path, &path_length);
                        printf("Path from leaf to root: ");
                        for (int i = 0; i < path_length; i++) {
                            printf("%d ", path[i]);
                        }
                        printf("\n");
                    }
                }
                break;
            }
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
    }
}


int main() {
    t_map map;

    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);
    interactive_test();
    return 0;


}






