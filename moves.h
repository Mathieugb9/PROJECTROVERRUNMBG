#ifndef UNTITLED1_MOVES_H
#define UNTITLED1_MOVES_H

#include "loc.h"

typedef enum e_move {
    F_10,
    F_20,
    F_30,
    B_10,
    T_LEFT,
    T_RIGHT,
    U_TURN
} t_move;

char* getMoveAsString(t_move move);
t_localisation move(t_localisation loc, t_move move);
void updateLocalisation(t_localisation* p_loc, t_move m);
t_move* getRandomMoves(int N);

t_orientation rotate(t_orientation ori, t_move move);
t_localisation translate(t_localisation loc, t_move move);

#endif // UNTITLED1_MOVES_H
