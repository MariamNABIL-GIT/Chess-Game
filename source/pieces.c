#include<stdlib.h>
#include "../include/pieces.h"
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