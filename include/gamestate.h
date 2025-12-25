#ifndef GAMESTATE_H
#define GAMESTATE_H


typedef struct{
    char input[10];
    char captured;
    int enpassant;
    int castling;
    int promotion;
    char promo_type;
}Move;

typedef struct{
    int current_player;
    int size_moves;
    Move *moves;
    int move_count;
}GameState;

void init_game_state(GameState *state);
void add_move(GameState *state,char* input,char captured) ;
Move *get_last_move(GameState *state);
void undo(char board[8][8], GameState *state);
int save_game(char board[8][8],GameState*state);
int load_file(char board[8][8],GameState *state);
int start_game(char board[8][8],GameState *state);

#endif
