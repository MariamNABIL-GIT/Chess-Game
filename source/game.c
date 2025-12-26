#include "../include/board.h"
#include "../include/gamestate.h"
#include "../include/input.h"
#include "../include/moves.h"
#include<stdio.h>
#include <stdlib.h>
int main(){
    char board[8][8];
    GameState state;
    int start=start_game(board,&state);
    if(!start)
    return 0;
    while(1){
        printf("\n\n");
        printf("player %d turn\n",state.current_player);
        int row_from,col_from,row_to,col_to,input;
       input= read_input(&col_from,&row_from,&col_to,&row_to,&state);
       if(input==-1)
       continue;
       else if(input==0){
        undo(board,&state);
        display_board(board,&state);
        continue;
       }
       else if(input==2){
        int save=save_game(board,&state);
        if(!save)
        continue;
        else
        break;
       }
       else if(input==3){
        redo(board,&state);
        display_board(board,&state);
        continue;
       }
       else if(input==1){
        int move_type=check_move(row_from,col_from,row_to,col_to,board,&state);
        int move=make_move(row_from,col_from,row_to,col_to,move_type,board,&state);
        display_board(board,&state);
        if(move==-1)
        break;
        else if(move==0)
        continue;
        else{
            int winner=state.current_player==1?2:1;
            if(check_mate(board,&state)){
                printf("Check mate\n");
                printf("Player %d wins\n",winner);
                break;
            }
            if(stale_mate(board,&state)){
              printf("Stale mate\n");
              break;  
            }
        }
      }
    }
    free(state.moves);
    return 0;
    
}