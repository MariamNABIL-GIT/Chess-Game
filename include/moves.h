#ifndef MOVES_H
#define MOVES_H
#include "gamestate.h"
int check_move(int row_from, int col_from, int row_to, int col_to,char board[8][8],GameState *state);
int make_move(int row_from, int col_from, int row_to, int col_to,int move_type,char board[8][8],GameState *state);
#endif