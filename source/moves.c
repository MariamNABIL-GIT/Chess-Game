#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include "../include/pieces.h"
#include "../include/gamestate.h"

int check_move(int row_from, int col_from, int row_to, int col_to,char board[8][8],GameState *state){
    if((row_from==row_to)&&(col_from==col_to)){
        printf("Invalid move:you have entered the same square\n");
        printf("Enter another move!\n");
        return 0;
    }
    int white=state->current_player==1? 1:0;
    if((white&&(board[row_from][col_from]>'A'&&board[row_from][col_from]<'Z'))||(!white&&(board[row_from][col_from]>'a'&&board[row_from][col_from]<'z'))){
        printf("Invalid move : not your piece\n");
        printf("Enter another move!\n");
        return 0;
    }
    if((white&&(board[row_to][col_to]>'a'&&board[row_to][col_to]<'z'))||(!white&&(board[row_to][col_to]>'A'&&board[row_to][col_to]<'Z'))){
        printf("Invalid move: you cannot capture your piece\n");
        printf("Enter another move!\n");
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
    if(!result){
        printf("Invalid move\n");
        printf("Enter another move!\n");
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
    char promo_type=state->moves[state->move_count].promo_type ;
    if(move_type==PROMOTION){
        promotion=1 ;
        board[row_to][col_to]= white ? tolower(promo_type) : promo_type ;
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    else if(move_type==EN_PASSANT){
        En_passant=1;
        capture=board[row_from][col_to];
        board[row_from][col_to]=(row_from+col_to)%2==0?'.':'-';
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    else if(move_type==Castling){
        king_castling=1;
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    
        int rook_col=(col_to>col_from)?7:0;  
        int c=(col_to>col_from)?(col_to-1):(col_to+1);  
    
        board[row_from][c]=board[row_from][rook_col];
        board[row_from][rook_col]=(row_from+rook_col)%2==0?'.':'-';
    }
    else if(move_type==1||move_type==2||move_type==3){
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    else if(move_type==0){
        return 0; 
    }
    if(state->move_count>=state->size_moves){
        Move *temp=realloc(state->moves,(state->size_moves+500)*sizeof(Move));
        if(temp==NULL){
            printf("End of Game : Out of memory\n");
            return -1; //returns -1 for realloc failure
        }    
        state->moves=temp;
        state->size_moves+=500;
    }
    Move *move=&state->moves[state->move_count];
    move->castling=king_castling;
    move->enpassant=En_passant;
    move->promotion=promotion;
    add_move(state,in,capture);
    state->current_player=white?2:1;
    state->undo_count=0;
    int k_row,k_col;
    find_king(board,white,&k_row,&k_col);
    if(check_check(k_row,k_col,board,white,state)){
        undo(board,state);
        state->undo_count=0;
        printf("Invalid move makes your king under check\n");
        printf("Enter another move!\n");
        return 0;//invalid move
    }
    return 1;//valid move
}

int try_move(int row_from, int col_from ,int row_to ,int col_to ,char board[8][8] ,int k_white ,GameState *state) {
    if(row_from==row_to && col_from==col_to) return 0; /* cannot go to same square*/

    char target=board[row_to][col_to] ;
    /*cannot capture the player own pieces*/
    if(target!='.' && target!='-'){
        if((k_white&& (target>='b' && target<='r')) || (!k_white && (target>='B' && target<='R'))) {
            return 0;
        }
    }

    char piece=board[row_from][col_from] ;
    int move_type=0 ;
    switch(tolower(piece)){
        case 'p' : move_type=check_pawn(board,row_from ,col_from ,row_to ,col_to,state) ;break ;
        case 'n' : move_type=check_knight(row_from ,col_from ,row_to ,col_to ) ;break ;
        case 'b' : move_type=check_bishop(row_from ,col_from ,row_to,col_to ,board) ; break ;
        case 'r' : move_type=check_rook(row_from ,col_from ,row_to ,col_to ,board) ;break ;
        case 'q' : move_type=check_queen(row_from ,col_from ,row_to ,col_to ,board) ;break ;
        case 'k' : move_type=check_king(row_from ,col_from ,row_to,col_to ,board ,state) ;break ;
    }
    if(move_type==0) return 0; /*invalid move */ 
    char capture ;
    if(move_type==EN_PASSANT){
        capture=board[row_from][col_to] ;
        board[row_from][col_to]=(row_from+col_to)%2==0?'.':'-';
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    else if(move_type==Castling){
    board[row_to][col_to]=board[row_from][col_from];
    board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    
    int rook_col=(col_to>col_from)?7:0;  
    int c=(col_to>col_from)?(col_to-1):(col_to+1);  
    
    board[row_from][c]=board[row_from][rook_col];
    board[row_from][rook_col]=(row_from+rook_col)%2==0?'.':'-';
}

    else {
        capture=board[row_to][col_to] ;
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    int k_row ,k_col ;
    find_king(board,k_white,&k_row,&k_col);
    int check=check_check(k_row,k_col,board,k_white,state) ;
     /*undo en passant */
     if(move_type==EN_PASSANT){
        board[row_from][col_from]=piece;
        board[row_to][col_to]=(row_to+col_to)%2==0?'.':'-';
        board[row_from][col_to]=capture;  
    }

    else if(move_type==Castling){
        board[row_from][col_from]=piece;
        board[row_to][col_to]=(row_to+col_to)%2==0?'.':'-';
        
        int rook_col=(col_to>col_from)?7:0;
        int c=(col_to>col_from)?(col_to-1):(col_to+1);
        board[row_from][rook_col]=board[row_from][c];
        board[row_from][c]=(row_from+c)%2==0?'.':'-';
    }
    else {
        board[row_from][col_from]=piece;
        board[row_to][col_to]=capture;
    }

    return !check ;

}

int is_there_move(int k_white ,char board[8][8] ,int k_row ,int k_col,GameState *state) {
 /*checking for king's moves*/ 
int king_moves[10][2]={{1,1},{1,0},{0,1},{1,-1},{-1,1},{-1,0},{0,-1},{-1,-1},{0,2},{0,-2}} ;
  
for(int i=0 ;i<10 ;i++){
    int row=k_row+king_moves[i][0];
    int col=k_col+king_moves[i][1] ;
    if(row>=0 && row<8 && col>=0 && col<8){
        if(try_move(k_row,k_col,row,col,board,k_white,state)){
            return 1;
        }
    }
}
for(int i=0 ;i<8 ;i++){
    for(int j=0 ;j<8 ;j++){
        char piece=board[i][j] ;
        if(piece=='.' || piece=='-') continue ;
        if((k_white &&(piece>='B' && piece<='R')) || (!k_white&&(piece>='b' && piece<='r')) || (piece=='k') || (piece=='K')) continue ;
        switch(tolower(piece)){
            case 'n' : 
            int knight_moves[8][2]={{1,2},{2,1},{-1,2},{2,-1},{1,-2},{-2,1},{-1,-2},{-2,-1}} ;
            for(int p=0 ;p<8 ;p++){
                int row=i+knight_moves[p][0] ;
                int col=j+knight_moves[p][1] ;
                if(row>=0 && row<8 && col>=0 && col<8){
                if(try_move(i,j,row,col,board,k_white,state)){
                    return 1;
                }}
            } break ;
            case 'p': 
            int direction=k_white?1:-1;
            int pawn_moves[4][2]={{2*direction,0},{direction,0},{direction,1},{direction,-1}} ;
            for(int p=0 ;p<4 ;p++){
                int row=i+pawn_moves[p][0] ;
                int col=j+pawn_moves[p][1] ;
                if(row>=0 && row<8 && col>=0 && col<8){
                if(try_move(i,j,row,col,board,k_white,state)) {
                    return 1;
                }}
            }break ;
            case 'q' :
            case 'b': 
            for(int p=1 ,q=1 ;(i+p)<8 && (j+q)<8 ;p++,q++){
                    int row=i+p ;
                    int col=j+q ;
                    if(try_move(i,j,row,col,board,k_white,state)) {
                        return 1 ;
                }
            }
            for(int p=1 ,q=-1 ;(i+p)<8 &&(j+q)>=0 ;p++ ,q-- ){
                int row=i+p ;
                int col=j+q ;
                if(try_move(i,j,row,col,board,k_white,state)){
                    return 1;
                }
            }
            for(int p=-1 ,q=-1 ;(i+p)>=0 &&(j+q)>=0 ;p-- ,q-- ){
                int row=i+p ;
                int col=j+q ;
                if(try_move(i,j,row,col,board,k_white,state)){
                    return 1;
                }
            }
            for(int p=-1 ,q=1 ;(i+p)>=0 &&(j+q)<8 ;p-- ,q++ ){
                int row=i+p ;
                int col=j+q ;
                if(try_move(i,j,row,col,board,k_white,state)){
                    return 1;
                }
            }
            if(tolower(piece)=='b') break ;
            case 'r' :
            for(int p=0 ,q=1 ;(j+q)<8 ;q++){
                    int row=i+p ;
                    int col=j+q ;
                    if(try_move(i,j,row,col,board,k_white,state)) {
                        return 1 ;
                }
            }
            for(int p=1 ,q=0 ;(i+p)<8 ;p++){
                    int row=i+p ;
                    int col=j+q ;
                    if(try_move(i,j,row,col,board,k_white,state)) {
                        return 1 ;
                }
            }
            for(int p=-1 ,q=0 ;(i+p)>=0 ;p--){
                    int row=i+p ;
                    int col=j+q ;
                    if(try_move(i,j,row,col,board,k_white,state)) {
                        return 1 ;
                }
            }
            for(int p=0 ,q=-1 ;(j+q)>=0 ;q--){
                    int row=i+p ;
                    int col=j+q ;
                    if(try_move(i,j,row,col,board,k_white,state)) {
                        return 1 ;
                }
            }break ;   
        }
    }
}
return 0;
}
int check_mate(char board[8][8],GameState *state) {
    int k_row,k_col;
    int k_white=state->current_player==1?1:0;
    find_king(board,k_white,&k_row,&k_col);
    if(!check_check(k_row,k_col,board,k_white,state)) {
        return 0;
    }
    else{
        printf("Player %d king is under check\n",state->current_player);
    }
    return !is_there_move(k_white,board,k_row,k_col,state) ;
}

int stale_mate(char board[8][8],GameState *state){
    int k_row,k_col;
    int k_white=state->current_player==1?1:0;
    find_king(board,k_white,&k_row,&k_col);
    if(check_check(k_row,k_col,board,k_white,state)) {
        return 0 ;
    }
    return !is_there_move(k_white,board,k_row,k_col,state);
}

