#ifndef PIECES_H
#define PIECES_H
int check_rook(int row_from,int col_from, int row_to, int col_to, char board[8][8]);
int check_bishop(int row_from,int col_from, int row_to, int col_to, char board[8][8]);
int check_queen(int row_from,int col_from, int row_to, int col_to, char board[8][8]);
#endif