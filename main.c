#include <stdio.h>
#include <stdlib.h>
#include "movement_tree.h"
#include "map.h"
#include "loc.h"
#include "moves.h"

void print_localisation(t_localisation loc) {
    printf("Position: (%d, %d), Orientation: %d, Moves allowed: %d\n",
           loc.pos.x, loc.pos.y, loc.ori, loc.num_moves_allowed);
}

void print_map(t_map* map) {
    printf("Map:\n");
    for (int i = 0; i < map->y_max; i++) {
        for (int j = 0; j < map->x_max; j++) {
            printf("%d ", map->soils[i][j]);
        }
        printf("\n");
    }

    printf("Costs:\n");
    for (int i = 0; i < map->y_max; i++) {
        for (int j = 0; j < map->x_max; j++) {
            printf("%-5d ", map->costs[i][j]);
        }
        printf("\n");
    }
}

void simulate_full_path(t_map* map, t_localisation start_loc) {
    printf("\n--- Starting Full Path Simulation ---\n");
    t_localisation current_loc = start_loc;

    int phase = 1;
    int total_cost = 0;

    while (1) {
        printf("\n--- Phase %d ---\n", phase);
        printf("Current State:\n");
        print_localisation(current_loc);

        if (map->costs[current_loc.pos.y][current_loc.pos.x] == 0) {
            printf("MARC has reached the base station!\n");
            break;
        }

        TreeNode* root = construct_phase_tree(current_loc, map);
        TreeNode* min_leaf = find_minimum_leaf(root);

        if (min_leaf == NULL) {
            printf("No valid path found in phase %d. Simulation terminated.\n", phase);
            free_tree(root);
            break;
        }

        total_cost += min_leaf->cost;

        int movement_path[100];
        int path_length = 0;
        trace_path(min_leaf, movement_path, &path_length);

        printf("Optimal path for this phase:\n");
        for (int i = 0; i < path_length; i++) {
            printf("%s ", getMoveAsString(movement_path[i]));
        }
        printf("\n");

        for (int i = 0; i < path_length; i++) {
            current_loc = apply_move(current_loc, movement_path[i], map);

            if (current_loc.pos.x == -1 || current_loc.pos.y == -1) {
                printf("MARC went out of bounds or hit a crevasse. Simulation terminated.\n");
                free_tree(root);
                return;
            }
        }

        free_tree(root);
        phase++;
    }

    printf("\nSimulation complete: Total Cost = %d, Phases = %d.\n", total_cost, phase);
}

int main() {
    printf("\n--- MARC Rover Simulation ---\n");

    t_map map;

#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map loaded with dimensions: %d x %d\n", map.y_max, map.x_max);
    print_map(&map);

    t_localisation start_loc = loc_init(0, 0, NORTH);
    printf("\nStarting MARC at initial position:\n");
    print_localisation(start_loc);

    while (1) {
        printf("\n--- Options ---\n");
        printf("1: Simulate MARC movements interactively\n");
        printf("2: Simulate full path to base station\n");
        printf("3: Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\nInteractive simulation is not implemented in this version.\n");
        } else if (choice == 2) {
            simulate_full_path(&map, start_loc);
        } else if (choice == 3) {
            printf("Exiting simulation.\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
