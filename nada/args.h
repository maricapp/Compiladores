/*
* args.h
*
* Created on: Mar 10, 2011
* Author: blabos
*/

#ifndef ARGS_H_
#define ARGS_H_

#define MAX_BUFF 4096

typedef struct {
    int num_states;
    int num_symbols;
    int num_fstates;
    char** states;
    int* fstates;
    char* symbols;
    int* transitions;
} args_t;

int get_args(char* filename, args_t* args);
void print_args(args_t* args);

int state_index(args_t* args, char* label);
int symbol_index(args_t* args, char symbol);
int is_final_state(args_t* args, int state);

#endif /* ARGS_H_ */
