# include <stdio.h>
# include <string.h>
# include <ctype.h>
#include "../include/input.h"
#include "../include/gamestate.h"
int read_input(int *ind_col_from, int  *ind_row_from,int  *ind_col_to ,int *ind_row_to ,GameState *state) {
    int input_len=0 ;
    char input[50] ;
    printf("Enter move : ") ;
    int c=0 ;
    while((c=getchar())!='\n' && c!=EOF){
        if (c==' ') continue ;
        input[input_len++]=toupper(c);
        if(input_len>=49) break ;
    }
    input[input_len]='\0' ;
    if(input_len<4 || input_len>5) {
        printf("invalid input \n") ;
        printf("Enter another move!\n");
        return -1 ; /* return -1 if invalid input */
    }

    if(strcmp(input,"UNDO")==0){
        return 0 ; /* return 0 for undo */
    }
    if(strcmp(input,"SAVE")==0){
        return 2 ; /* return 2 for save */
    }
    if(strcmp(input,"REDO")==0){
        return 3;
    }
    state->moves[state->move_count].promotion = 0;
    state->moves[state->move_count].promo_type = 0;
    state->moves[state->move_count].enpassant = 0;
    state->moves[state->move_count].castling = 0;
    if(input_len==5){
         char promo_type=input[4] ;
         if(promo_type=='B' || promo_type=='Q' || promo_type=='N' || promo_type=='R'){
            state->moves[state->move_count].promotion=1 ;
            state->moves[state->move_count].promo_type=promo_type ;
         }
         else {
            printf("Invalid Promotion Piece !\n") ;
            printf("Enter another move!\n");
            return -1 ;
         }
    }

    char col_from=input[0] ;
    char row_from=input[1] ;
    char col_to=input[2] ;
    char row_to=input[3] ;

    
    if (col_from < 'A' || col_from> 'H' ||col_to< 'A' ||col_to > 'H') {
        printf("Invalid column! Use A-H\n");
        printf("Enter another move!\n");
        return -1; /* return -1 if invalid input */
    }
    if (row_from < '1' || row_from > '8' || row_to< '1' || row_to> '8') {
        printf("Invalid row! Use 1-8\n");
        printf("Enter another move!\n");
        return -1; /* return -1 if invalid input */
    }
    *ind_col_from=col_from-'A' ;
    *ind_row_from=row_from-'1' ;
    *ind_col_to=col_to-'A' ;
    *ind_row_to=row_to-'1' ;
    return 1; /*for valid move*/
}
