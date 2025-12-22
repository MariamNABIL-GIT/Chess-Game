#ifndef PIECES_H
#define PIECES_H

#include "../include/Gamestate.h"
// those #define are for check_pawn returning value 
#define NORMAL_MOVE 1
#define INVALID_MOVE 0
#define EN_PASSANT 2
#define CAPTURE_CASE 3 
#define Castling 4

int check_rook(int row_from,int col_from, int row_to, int col_to, char board[8][8]);
int check_bishop(int row_from,int col_from, int row_to, int col_to, char board[8][8]);
int check_queen(int row_from,int col_from, int row_to, int col_to, char board[8][8]);
int check_pawn(char board[8][8],int from_row,int from_col,int row_to,int col_to,GameState* state);
int check_knight(int from_row,int from_col,int row_to,int col_to);
int check_check(int k_row, int k_col, char board[8][8], int K_type ,GameState *state);
void find_king(char board[8][8] ,int k_white ,int *k_row ,int *k_col) ;
int check_king(int row_from, int col_from, int row_to, int col_to,char board[8][8], GameState *state );

#endif