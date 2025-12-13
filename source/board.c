#include "../include/board.h"
void initialize_board(char board[8][8]){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
          if(i==1)
          board[i][j]='p';
          else if(i==6)
          board[i][j]='P';
          else if((i==0)&&(j==0||j==7))
          board[i][j]='r'; 
          else if((i==0)&&(j==1||j==6))
          board[i][j]='n';
          else if((i==0)&&(j==2||j==5))
          board[i][j]='b';
          else if((i==0)&&(j==3))
          board[i][j]='q';
          else if((i==0)&&(j==4))
          board[i][j]='k';
          else if((i==7)&&(j==0||j==7))
          board[i][j]='R';
          else if((i==7)&&(j==1||j==6))
          board[i][j]='N';
          else if((i==7)&&(j==2||j==5))
          board[i][j]='B';
          else if((i==7)&&(j==3))
          board[i][j]='Q';
          else if((i==7)&&(j==4))
          board[i][j]='K';
          else if((i+j)%2==0)
          board[i][j]='.';
          else 
          board[i][j]='-';
        }
    }
}
void display_board(char board[8][8],char captured_from_W[16],int sizeW, char captured_from_B[16], int sizeB ){
    printf("The pieces taken out from Black: ");
    for(int i=0;i<sizeB;i++){
        printf("%c ",captured_from_B[i]);
    }
    printf("\n");
    printf("The Board:\n\n");
    printf(" \t");
    for(char i='A';i<'I';i++){
        printf("%c\t",i);
    }
    printf("\n\n");
    for(int i=7;i>=0;i--){
        for(int j=-1;j<9;j++){
            if(j==-1)
            printf("%d\t",i+1);
            else if(j==8)
            printf("%d",i+1);
            else{
                printf("%c\t",board[i][j]);
            }
        }
        printf("\n\n");
    }
  printf(" \t");
  for(char i='A';i<'I';i++){
    printf("%c\t",i);
  }
  printf("\n\n");
  printf("The pieces taken out from white: ");
  for(int i=0;i<sizeW;i++){
    printf("%c ",captured_from_W[i]);
  }
  printf("\n");
}