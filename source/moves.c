#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include "../include/pieces.h"
#include "../include/gamestate.h"

int check_move(int row_from, int col_from, int row_to, int col_to,char board[8][8],GameState *state){
    if((row_from==row_to)&&(col_from==col_to)){
        printf("Invalid move:you have entered the same square");
        return 0;
    }
    int white=state->current_player==1? 1:0;
    if((white&&(board[row_from][col_from]>'A'&&board[row_from][col_from]<'Z'))||(!white&&(board[row_from][col_from]>'a'&&board[row_from][col_from]<'z'))){
        printf("Invalid move : not your piece");
        return 0;
    }
    if((white&&(board[row_to][col_to]>'a'&&board[row_to][col_to]<'z'))||(!white&&(board[row_to][col_to]>'A'&&board[row_to][col_to]<'Z'))){
        printf("Invalid move: you cannot capture your piece");
        return 0;
    }
    char piece=board[row_from][col_from];
    int result=0;
    switch(piece){
        case'P':case'p':result=check_pawn(board,row_from,col_from,row_to,col_to,state);break;
        case'R':case'r':result=check_rook(row_from,col_from,row_to,col_to,board);break;
        case'N':case'n':result=check_knight(row_from,col_from,row_to,col_to);break;
        case'B':case'b':result=check_bishop(row_from,col_from,row_to,col_to,board);break;
        case'Q':case'q':result=check_queen(row_from,col_from,row_to,col_to,board);break;
        case'K':case'k':result=check_king(row_from,col_from,row_to,col_to,board,state);break;
        default:result=0;break;
    }
    return result;
}

int make_move(int row_from, int col_from, int row_to, int col_to,int move_type,char board[8][8],GameState *state){
    char capture=board[row_to][col_to];
    char in[5]={col_from+'A',row_from+'1',col_to+'A',row_to+'1','\0'};
    int En_passant=0;
    int king_castling=0;
    int promotion=0;
    int white=state->current_player==1?1:0;
    if(move_type==EN_PASSANT){
        En_passant=1;
        capture=board[row_from][col_to];
        board[row_from][col_to]=(row_from+col_to)%2==0?'.':'-';
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    else if(move_type==Castling){
        king_castling=1;
        board[row_to][col_to]=board[row_from][col_from];
        if(white)
        board[row_from][col_from]='r';
        else
        board[row_from][col_from]='R';
        int r,c;
        if(white && (col_to>col_from)){
            r=0;
            c=7;
        }
        else if(white && (col_from>col_to)){
            r=0;
            c=0;
        }
        else if(!white && (col_to>col_from)){
            r=7;
            c=7;
        }
        else if(!white && (col_from>col_to)){
            r=7;
            c=0;
        }
        board[r][c]=(r+c)%2==0?'.':'-';
    }
    else if(move_type==1||move_type==2){
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    Move *move=&state->moves[state->move_count];
    move->castling=king_castling;
    move->enpassant=En_passant;
    move->promotion=promotion;
    add_move(state,in,capture);
    state->current_player=white?2:1;
    int k_row,k_col;
    find_king(board,white,&k_row,&k_col);
    if(check_check(k_row,k_col,board,white,state)){
        undo(board,state);
        printf("Invalid move makes your king under check");
        return 0;
    }
    return 1;
}