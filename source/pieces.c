#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include "../include/pieces.h"
#include "../include/Gamestate.h"
//function checks rook's move
int check_rook(int row_from,int col_from, int row_to, int col_to, char board[8][8]){
    if((row_from!=row_to)&&(col_from!=col_to))
    return 0;   //return 0 if the path is invalid
    else{
        //x for row direction,y for column direction
        int x,y;
        //choose the direction(up ,down,left ,right)
        if(row_to>row_from){
            x=1;
            y=0;
        }
        else if(row_from>row_to){
            x=-1;
            y=0;
        } 
        else if(col_to>col_from){
            x=0;
            y=1;
        }
        else if(col_from>col_to){
            x=0;
            y=-1;
        } 
        int newX=row_from+x;
        int newY=col_from+y;//checking each square in the path
        while((newX!=row_to)||(newY!=col_to)){
              if((board[newX][newY]!='.')&&(board[newX][newY]!='-'))
             return 0;//return 0 if the path is not empty
             newX+=x;
             newY+=y;
        }
        return 1;//return 1 for valid move
    }
}
//function checks bishop's move
int check_bishop(int row_from,int col_from, int row_to, int col_to, char board[8][8]){
    if(abs(row_to-row_from)!=abs(col_to-col_from))
     return 0;//return 0 if it is not a bishop's path
     else{
        //x for row direction,y for column direction
        int x,y;
        if((row_to>row_from)&&(col_to>col_from)){
            x=1;
            y=1;
        }
        else if((row_to>row_from)&&(col_to<col_from)){
            x=1;
            y=-1;
        }
        else if((row_to<row_from)&&(col_to>col_from)){
            x=-1;
            y=1;
        }
        else if((row_to<row_from)&&(col_to<col_from)){
            x=-1;
            y=-1;
        }
        int newX=row_from+x;
        int newY=col_from+y;
        //checking if the path is empty
        while((newX!=row_to)&&(newY!=col_to)){
            if((board[newX][newY]!='.')&&(board[newX][newY]!='-'))
            return 0;//return 0 if not empty
            newX+=x;
            newY+=y;
        }
        return 1;//return 1 for a valid move
     }
}
//function checks queen's move 
int check_queen(int row_from,int col_from, int row_to, int col_to, char board[8][8]){
    int straight_move=0;
    int diagonal_move=0;
    //check if it is a straight move or not 
    straight_move=check_rook(row_from, col_from, row_to, col_to, board);
    if(straight_move)
    return 1;//return 1 if it is a valid straight move
    else//if not check if it is a diagonal move
    diagonal_move=check_bishop(row_from, col_from, row_to, col_to, board);
    if(diagonal_move)
    return 1;//return 1 if it is valid 
    else
    return 0;//return 0 for invalid move
}

int check_pawn(char board[8][8],int row_from,int col_from,int row_to,int col_to,GameState* state){
    int is_white=state->current_player== 1? 1:0 ;
    int direction=is_white? 1:-1 ;
    int start_row=is_white? 1:6 ;
    int row_diff=row_to-row_from ;
    int col_diff=col_to-col_from ;
    char target=board[row_to][col_to];
    
    /* forward one square */
    if(col_diff==0 && row_diff==direction){
        if(target=='.' || target=='-'){
            return NORMAL_MOVE;
        }
    }
    /* forward two squares from start */
    if(col_diff==0 && row_diff==2*direction && row_from==start_row){
        if(target=='.' || target=='-'){
            char middle=board[row_from+direction][col_from] ;
            if(middle=='.' || middle=='-'){
                return NORMAL_MOVE;
            }
        }
    }
    /* diagonal capture */
    if(abs(col_diff)==1 && row_diff==direction){
        if(target!='.' && target!='-'){
            return CAPTURE_CASE;
        }
    }
    /* en passant */
    if(abs(col_diff)==1 && row_diff==direction){
        if(target=='.' || target=='-'){
            Move *last_move=get_last_move(state);
            if(last_move!=NULL){
                int last_col_from=last_move->input[0]-'A' ;
                int last_row_from=last_move->input[1]-'1' ;
                int last_col_to=last_move->input[2]-'A' ;
                int last_row_to=last_move->input[3]-'1' ;
                char last_moved_piece=board[last_row_to][last_col_to] ;
                
                if(tolower(last_moved_piece)=='p'){
                    int last_row_diff=abs(last_row_to-last_row_from);
                    if(last_row_diff==2){
                        if(last_row_to==row_from && last_col_to==col_to){
                            return EN_PASSANT;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

int check_knight(int row_from,int col_from,int row_to,int col_to){
    int row_diff=abs(row_to-row_from);
    int col_diff=abs(col_to-col_from);
    
    if((row_diff==2&&col_diff==1) || (row_diff==1&&col_diff==2)){
        return 1;
    }
    
    return 0;
}

int check_check(int k_row, int k_col,char board[8][8],int k_white,GameState *state) {
    for(int i=0 ;i<8 ;i++) {
        for(int j=0 ;j<8 ;j++){
            char piece=board[i][j] ;
            if(piece=='.' || piece=='-') continue ;
            if(k_white&& (piece >='b' && piece<='r') || !k_white && (piece>='B' && piece<='R')) continue ;
            piece=tolower(piece) ;
            switch(piece) {
                case 'p' : if(check_pawn(board,i,j,k_row,k_col,state)==CAPTURE_CASE) return 1 ; break ;
                case 'r' : if (check_rook(i,j,k_row,k_col,board)) return 1; break ;
                case 'b' :if (check_bishop(i,j,k_row,k_col,board)) return 1; break ;
                case 'n' :if(check_knight(i,j,k_row,k_col)) return 1 ; break ;
                case 'q' :if(check_queen(i,j,k_row,k_col,board) ) return 1; break;
                case 'k' :if(abs(i-k_row)<=1 && abs(j-k_col)<=1) return 1; break ;
            }
        }
    }
    return 0 ; // king isn't under check 

}

void find_king(char board[8][8],int k_white ,int *k_row ,int *k_col){
    char k_type=k_white?'k':'K' ;
    for(int i=0 ;i<8 ;i++){
        for(int j=0 ;j<8 ;j++){
            if(board[i][j]==k_type){
                *k_row=i;
                *k_col=j;
                return ;
            }
        }
    }
}

int check_king(int row_from, int col_from, int row_to, int col_to,char board[8][8], GameState *state ){
    int row_diff=abs(row_to-row_from);
    int col_diff=abs(col_to-col_from);
    int white;
    if(state->current_player==1)
    white=1;
    else
    white=0;
    if((row_diff==1&&col_diff==0)||(col_diff==1&&row_diff==0)||(row_diff==1&&col_diff==1))
        {if(!check_check(row_to,col_to,board,white,state))
        return 1;
        else
        return 0;}
    else if(col_diff==2 &&row_diff==0){
        if((white && row_from==0 && col_from==4)||(!white && row_from==7 &&col_from==4)){
            char king_position[3]={col_from+'A',row_from+'1','\0'};
            char rook_position[3];
            rook_position[0]=(col_to>col_from)?'H':'A';
            rook_position[1]=row_from+'1';
            rook_position[2]='\0';
            for(int i=0;i<state->move_count;i++){
                if(strncmp(state->moves[i].input,king_position,2)==0||strncmp(state->moves[i].input,rook_position,2)==0)
                return 0;
            }
            int dir=(col_to-col_from)/2;
            int index=col_from+dir;
            while(index>0&&index<7){
                if(board[row_from][index]!='.'&&board[row_from][index]!='-')
                return 0;
                index+=dir;
            }
            if(!check_check(row_from,col_from,board,white,state)&&!check_check(row_from,col_from+dir,board,white,state)&&!check_check(row_to,col_to,board,white,state))
            return Castling;
        }
    }
    return 0;
}