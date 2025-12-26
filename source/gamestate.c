#include "../include/Gamestate.h"
#include "../include/board.h"
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include<ctype.h>

int init_game_state(GameState* state){
    state->current_player=1;
    state->move_count=0;
    state->undo_count=0;
    state->size_moves=500;
    state->moves= malloc(500*sizeof(Move));
    if(state->moves==NULL){
        printf("NO enough memory\n");
        return 0;
    }
    return 1;
}

void add_move(GameState *state,char *input,char captured){
    Move *move=&state->moves[state->move_count];
    strcpy(move->input,input);
    move->captured=captured;
    state->move_count++;
}

Move *get_last_move(GameState *state){
    if(state->move_count==0){
        return NULL;
    }
    return &state->moves[state->move_count-1];
}

void undo(char board[8][8], GameState *state){
    if(state->move_count==0){
        printf("First move undone cannot undo more\n");
        return;
    }
    int col_from= state->moves[state->move_count-1].input[0]-'A';
    int row_from= state->moves[state->move_count-1].input[1]-'1';
    int col_to= state->moves[state->move_count-1].input[2]-'A';
    int row_to= state->moves[state->move_count-1].input[3]-'1';
    if(state->moves[state->move_count-1].enpassant==1){
        board[row_from][col_to]=state->moves[state->move_count-1].captured;
        board[row_from][col_from]=board[row_to][col_to];
        board[row_to][col_to]=(row_to+col_to)%2==0?'.':'-';
    }
    else if(state->moves[state->move_count-1].castling==1){
        int direction=col_to>col_from?1:-1;
        int col=col_to>col_from?7:0;
        board[row_from][col_from]=board[row_to][col_to];
        board[row_to][col_to]=(row_to+col_to)%2==0?'.':'-';
        board[row_from][col]=board[row_from][col_from+direction];
        board[row_from][col_from+direction]=(row_from+col_from+direction)%2==0?'.':'-';
    }
    else if(state->moves[state->move_count-1].promotion==1){
        board[row_from][col_from]=board[row_to][col_to];
        board[row_to][col_to]=state->moves[state->move_count-1].captured;
        board[row_from][col_from]=state->current_player==2?'p':'P';
    }
    else{
        board[row_from][col_from]=board[row_to][col_to];
        board[row_to][col_to]=state->moves[state->move_count-1].captured;
    }
    state->undo_count++;
    state->current_player=state->current_player==1?2:1;
    state->move_count-=1;
}

int save_game(char board[8][8],GameState*state){
    char file_name[11];
    printf("Enter the file name:(maximum 10 characters) ");
    char c;
    int i=0;
    while((c=getchar())!='\n'&&c!=EOF){
        if(i<10){
            file_name[i]=c;
            i++;
        }
    }
    file_name[i]='\0';
    char path[40]="gameFiles/";
    strcat(path,file_name);
    FILE *file=fopen(path,"wb");
    if(file==NULL){
        printf("Cannot open the File :Save failed\n");
        printf("Enter another move!\n");
        return 0;
    }

    for(int j=0;j<8;j++){
        fwrite(board[j],sizeof(char),8,file);
    }
    fwrite(&state->current_player, sizeof(int), 1, file);
    fwrite(&state->size_moves, sizeof(int), 1, file);
    fwrite(&state->move_count, sizeof(int), 1, file);
    fwrite(&state->undo_count, sizeof(int), 1, file);
    fwrite(state->moves, sizeof(Move), state->move_count, file);
    printf("Saved Successfully\n");
    fclose(file);
    return 1;
}

int load_file(char board[8][8],GameState *state){
    char file_name[11];
    printf("Enter the file name:(maximum 10 characters) ");
    char c;
    int i=0;
    while((c=getchar())!='\n'&&c!=EOF){
        if(i<10){
            file_name[i]=c;
            i++;
        }
    }
    file_name[i]='\0';
    char path[40]="gameFiles/";
    strcat(path,file_name);
    FILE *file=fopen(path,"rb");
    if(file==NULL){
        printf("failed to load the game\n");
        printf("End of program\n");
        return 0;
    }
    for(int j=0;j<8;j++){
        fread(board[j],sizeof(char),8,file);
    }
    fread(&state->current_player,sizeof(int),1,file);
    fread(&state->size_moves,sizeof(int),1,file);
    fread(&state->move_count,sizeof(int),1,file);
    fread(&state->undo_count,sizeof(int),1,file);
    state->undo_count=0;
    state->moves=malloc(state->size_moves*sizeof(Move));
    if(state->moves==NULL){
        printf("No enough memory\n");
        fclose(file);
        return 0;
    }
    fread(state->moves,sizeof(Move),state->move_count,file);
    printf("Loaded successfully\n");
    fclose(file);
    return 1;
}
int start_game(char board[8][8],GameState *state){
    char start[15];
    printf("Choose (New game) or (saved game): ");
    char c;
    int i=0;
    while((c=getchar())!='\n'&&c!=EOF){
        if(i<14){
            start[i]=c;
            i++;
        }
        
    }
    start[i]='\0';
    if(strcasecmp(start,"new game")==0){
        initialize_board(board);
        int init= init_game_state(state);
        printf("---------------New Game-----------------\n");
        return init;
    }
    else if(strcasecmp(start,"saved game")==0){
    state->moves = NULL;
    return load_file(board,state);
    }
    else{
        printf("Invalid input\n");
        printf("End of program!\n");
        return 0;
    }
}

void redo(char board[8][8], GameState *state){
    if(state->undo_count==0){
        printf("All undone data are Redone: cannot undo more\n");
        return;
    }
    int col_from=state->moves[state->move_count].input[0]-'A';
    int row_from=state->moves[state->move_count].input[1]-'1';
    int col_to=state->moves[state->move_count].input[2]-'A';
    int row_to=state->moves[state->move_count].input[3]-'1';
    if(state->moves[state->move_count].enpassant==1){
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
        board[row_from][col_to]=(row_from+col_to)%2==0?'.':'-';
    }
    else if(state->moves[state->move_count].castling==1){
        int direction=col_to>col_from?1:-1;
        int rook_col=col_to>col_from?7:0;
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
        board[row_from][col_from+direction]=board[row_from][rook_col];
        board[row_from][rook_col]=(row_from+rook_col)%2==0?'.':'-';
    }
    else if(state->moves[state->move_count].promotion==1){
        char prom=state->moves[state->move_count].promo_type;
        if(board[row_from][col_from]=='p'){
            board[row_to][col_to]=tolower(prom);
        }
        else{
            board[row_to][col_to]=prom;
        }
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    else{
        board[row_to][col_to]=board[row_from][col_from];
        board[row_from][col_from]=(row_from+col_from)%2==0?'.':'-';
    }
    state->undo_count--;
    state->move_count++;
    state->current_player=state->current_player==1?2:1;
}
