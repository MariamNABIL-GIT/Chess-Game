#include "../include/Gamestate.h"
#include "../include/board.h"
#include <string.h>
#include <stdio.h>
#include<stdlib.h>

void init_game_state(GameState* state){
    state->current_player=1;
    state->move_count=0;
    state->moves= malloc(500*sizeof(Move));
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
        printf("First move undone\n");
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
        board[row_to][col_to]=(row_to+col_to)%2==0?'.':'-';
        board[row_from][col_from]=state->current_player==2?'p':'P';
    }
    else{
        board[row_from][col_from]=board[row_to][col_to];
        board[row_to][col_to]=state->moves[state->move_count-1].captured;
    }
    state->move_count-=1;
    state->current_player=state->current_player==1?2:1;
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
    char path[30]="../gameFiles/";
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
    fwrite(state,sizeof(GameState),1,file);
    fwrite(state->moves,sizeof(Move),state->move_count,file);
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
    char path[30]="../gameFiles/";
    strcat(path,file_name);
    FILE *file=fopen(path,"rb");
    if(file==NULL){
        printf("failed to load the game\n");
        printf("End of program");
        return 0;
    }
    for(int j=0;j<8;j++){
        fread(board[j],sizeof(char),8,file);
    }
    fread(state,sizeof(GameState),1,file);
    state->moves=malloc(state->size_moves*sizeof(Move));
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
        init_game_state(state);
        printf("---------------New Game-----------------\n");
        return 1;
    }
    else if(strcasecmp(start,"saved game")==0){
        return load_file(board,state);
    }
    else{
        printf("Invalid input\n");
        printf("End of program!\n");
        return 0;
    }
}
