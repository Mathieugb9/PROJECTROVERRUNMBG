#include <stdio.h>
#include <stdlib.h>
#include "movement_tree.h"
#include "map.h"
#include "loc.h"
#include "moves.h"


t_position find_base_station(t_map* map) {
    t_position base_position;
    base_position.x = -1;
    base_position.y = -1;

    for (int y = 0; y < map->y_max; y++) {
        for (int x = 0; x < map->x_max; x++) {
            if (map->soils[y][x] == BASE_STATION) {
                base_position.x = x;
                base_position.y = y;
                return base_position;
            }
        }
    }


    fprintf(stderr, "Error: Base station not found on the map.\n");
    exit(EXIT_FAILURE);
}


int trim_path_to_base(t_map* map, t_position base, t_localisation start_loc, int* movement_path, int path_length) {
    t_localisation current_loc = start_loc;

    for (int i = 0; i < path_length; i++) {
        current_loc = apply_move(current_loc, movement_path[i], map);


        if (current_loc.pos.x == base.x && current_loc.pos.y == base.y) {
            return i + 1;
        }
    }

    return path_length;
}

void print_localisation_with_base(t_localisation loc, t_position base) {
    printf("MARC Position: (%d, %d), Orientation: %d, Moves allowed: %d | Base Position: (%d, %d)\n",
           loc.pos.x, loc.pos.y, loc.ori, loc.num_moves_allowed, base.x, base.y);
}

void display_map_with_costs(t_map* map) {
    printf("\n--- Map with Costs ---\n");
    for (int y = 0; y < map->y_max; y++) {
        for (int x = 0; x < map->x_max; x++) {
            printf("[%d] ", map->costs[y][x]);
        }
        printf("\n");
    }
}

void simulate_full_path(t_map* map, t_localisation start_loc, t_position base, int verbose) {
    printf("\n--- Starting MARC Path Simulation ---\n");

    t_localisation current_loc = start_loc;
    int phase = 1;
    int total_cost = 0;
    int movement_path[1000];
    int total_moves = 0;

    while (1) {

        if (current_loc.pos.x == base.x && current_loc.pos.y == base.y) {
            printf("\nMARC has reached the base station!\n");
            break;
        }

        TreeNode* root = construct_phase_tree(current_loc, map);
        TreeNode* min_leaf = find_minimum_leaf(root);

        if (min_leaf == NULL) {
            printf("\nNo valid path found in phase %d. Simulation terminated.\n", phase);
            free_tree(root);
            return;
        }

        int phase_moves[100];
        int path_length = 0;
        trace_path(min_leaf, phase_moves, &path_length);


        path_length = trim_path_to_base(map, base, current_loc, phase_moves, path_length);

        printf("\n--- Phase %d ---\n", phase);
        printf("Optimal Path: ");
        for (int i = 0; i < path_length; i++) {
            printf("%s ", getMoveAsString(phase_moves[i]));
        }
        printf("\n");

        for (int i = 0; i < path_length; i++) {
            movement_path[total_moves++] = phase_moves[i];
            current_loc = apply_move(current_loc, phase_moves[i], map);


            total_cost += map->costs[current_loc.pos.y][current_loc.pos.x];

            if (verbose) {
                printf("Move %d: %s -> Position: (%d, %d), Step Cost: %d, Total Cost: %d\n",
                       i + 1, getMoveAsString(phase_moves[i]),
                       current_loc.pos.x, current_loc.pos.y,
                       map->costs[current_loc.pos.y][current_loc.pos.x], total_cost);
            }


            if (current_loc.pos.x == base.x && current_loc.pos.y == base.y) {
                free_tree(root);
                return;
            }
        }

        free_tree(root);
        phase++;
    }

    printf("\n--- Simulation Complete ---\n");
    printf("Total Cost: %d\n", total_cost);
    printf("Total Moves: %d\n", total_moves);
    printf("Path Taken: ");
    for (int i = 0; i < total_moves; i++) {
        printf("%s ", getMoveAsString(movement_path[i]));
    }
    printf("\n");

    display_map_with_costs(map);
}

int main() {
    printf("\n--- MARC Rover Simulation ---\n");

    t_map map;

#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/training.map");
#endif

    printf("Map loaded with dimensions: %d x %d\n", map.y_max, map.x_max);

    t_position base_position = find_base_station(&map);

    t_localisation start_loc = loc_init(0, 0, NORTH);
    printf("\nStarting MARC at initial position:\n");
    print_localisation_with_base(start_loc, base_position);

    while (1) {
        printf("\n--- Options ---\n");
        printf("1: Simulate full path with detailed output\n");
        printf("2: Simulate full path with summary only\n");
        printf("3: Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            simulate_full_path(&map, start_loc, base_position, 1);
        } else if (choice == 2) {
            simulate_full_path(&map, start_loc, base_position, 0);
        } else if (choice == 3) {
            printf("Exiting simulation.\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
