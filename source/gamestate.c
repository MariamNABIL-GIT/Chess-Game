#include "../include/Gamestate.h"
#include <string.h>
#include <stdio.h>

void init_game_state(GameState* state){
    state->current_player=1;
    state->move_count=0;
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