/*
 * args.c
 *
 *  Created on: Mar 10, 2011
 *      Author: blabos
 */
#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_mode(FILE* fp, args_t* args);
int get_states(FILE* fp, args_t* args);
int get_symbols(FILE* fp, args_t* args);
int get_transitions(FILE* fp, args_t* args);

int _skip_line(char* line);
int _num_states(char* buffer);
int _get_states(args_t* args, char* buffer);
int _num_symbols(char* buffer);
int _get_symbols(char* buffer, char* vector);
char* _alloc_token(char* buffer, int first, int count);
int _parse_line_transition(args_t* args, char* line, int curr_line);

int get_args(char* filename, args_t* args) {
    FILE* fp;

    int ret = 0;

    if ((fp = fopen(filename, "r")) != NULL) {
        ret =  get_mode(fp, args)
            && get_states(fp, args)
            && get_symbols(fp, args)
            && get_transitions(fp, args);
        
        fclose(fp);
    }

    return ret;
}

void print_args(args_t* args) {
    int i, j;
    
    printf("\nSTATES:  %d/%d\n", args->num_fstates, args->num_states);
    for (i = 0; i < args->num_states; i++) {
        if (is_final_state(args, i)) {
            printf("(%s), ", args->states[i]);
        }
        else {
            printf("%s , ", args->states[i]);
        }
    }
    printf("\b\b \n");
    
    printf("\nSYMBOLS: %d\n", args->num_symbols);
    for (i = 0; i < args->num_symbols; i++) {
        printf("%c , ", args->symbols[i]);
    }
    printf("\b\b \n");
    
    printf("\nTRANSITIONS:\n");
    for (i = 0; i < args->num_states; i++) {
        for (j = 0; j < args->num_symbols; j++) {
            if (args->transitions[args->num_states * i + j] >= 0) {
                printf("%03d ", args->transitions[args->num_states * i + j]);
            }
            else {
                printf("--- ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int state_index(args_t* args, char* label) {
    int i;
    
    for (i = 0; i < args->num_states; i++) {
        if (strncmp(args->states[i], label, strlen(args->states[i])) == 0) {
            return i;
        }
    }
    
    return -1;
}

int symbol_index(args_t* args, char symbol) {
    int i;
    
    for (i = 0; i < args->num_symbols; i++) {
        if (args->symbols[i] == symbol) {
            return i;
        }
    }
    
    return -1;
}

int is_final_state(args_t* args, int state) {
    int i;
    
    return state >= 0 && state < args->num_states
           ? args->fstates[state]
           : 0;
}


int get_mode(FILE* fp, args_t* args) {
    char line[MAX_BUFF];
    
    // Copia a linha com o modo para o buffer line, ignornando comentários
    // ( # ) e linhas em branco
    do {
        if (fgets(line, MAX_BUFF, fp) == NULL) return 0;
    } while (_skip_line(line));
    line[strlen(line) - 1] = '\0';
    
    args->mode = atoi(line);
    
    return 1;
}

int get_states(FILE* fp, args_t* args) {
    char line[MAX_BUFF];
    
    // Copia a linha com os estados para o buffer line, ignornando comentários
    // ( # ) e linhas em branco
    do {
        if (fgets(line, MAX_BUFF, fp) == NULL) return 0;
    } while (_skip_line(line));
    line[strlen(line) - 1] = '\0';
    
    // Conta o número de estados para poder alocar o vetor de estados
    args->num_states = _num_states(line);
    if (args->num_states <= 0) return 0;
    
    // Aloca memória para o vetor de estados
    args->states = (char**)malloc(sizeof(char*) * args->num_states);
    if (args->states == NULL) return 0;
    
    // Aloca memória para o vetor de estados finais, que podem ser os mesmos
    args->fstates = (int*)malloc(sizeof(int) * args->num_states);
    if (args->fstates == NULL) return 0;
    memset(args->fstates, 0, args->num_states);
    args->num_fstates = 0;
    
    // Copia os estados para o vetor de estados.
    // Se der erro, libera a memória alocada
    if (_get_states(args, line) <= 0) {
        free(args->states);
        args->num_states = 0;
        return 0;
    }
    
    return 1;
}

int get_symbols(FILE* fp, args_t* args) {
    char line[MAX_BUFF];
    
    // Copia a linha com os símbolos para o buffer line, ignornando
    // comentários ( # ) e linhas em branco
    do {
        if (fgets(line, MAX_BUFF, fp) == NULL) return 0;
    } while (_skip_line(line));
    line[strlen(line) - 1] = '\0';
    
    // Conta o número de símbolos para poder alocar o vetor de símbolos
    args->num_symbols = _num_symbols(line);
    if (args->num_symbols <= 0) return 0;
    
    // Aloca memória para o vetor de símbolos
    args->symbols = (char*)malloc(sizeof(char) * args->num_symbols);
    if (args->symbols == NULL) return 0;
    
    // Copia os estados para o vetor de símbolos.
    // Se der erro, libera a memória alocada
    if (_get_symbols(line, args->symbols) <= 0) {
        free(args->symbols);
        args->num_symbols = 0;
        return 0;
    }
    
    return 1;
}

int get_transitions(FILE* fp, args_t* args) {
    char line[MAX_BUFF];
    int curr_line = 0;
    int has_error = 0;
    
    args->transitions = (int*)malloc(sizeof(int) * args->num_states * args->num_symbols);
    if (args->transitions == NULL) {
        return 0;
    }
    
    while (!has_error && curr_line < args->num_states) {
        do {
            if (fgets(line, MAX_BUFF, fp) == NULL) return 0;
        } while (_skip_line(line));
        line[strlen(line) - 1] = '\0';
        
        if(_parse_line_transition(args, line, curr_line)) {
            
        }
        else {
            has_error = 1;
        }
        
        curr_line++;
    }
    
    return !has_error;
}

int _skip_line(char* line) {
    return line[0] == '#' || line[0] == '\n';
}

int _num_states(char* buffer) {
    int count = 0;
    
    if (*buffer == '(') goto _open_parentesis_;
    else if (isalpha(*buffer)) goto _alpha_;
    else if (isspace(*buffer)) goto _space_;
    else goto _error_;
    
    // Caracter corrente é um (
    _open_parentesis_:
        buffer++;
        if (*buffer && isalpha(*buffer)) goto _alpha_;
        else goto _error_;
    
    // Caracter corrente é um )
    _close_parentesis_:
        buffer++;
        if (*buffer) {
            if (isspace(*buffer)) goto _space_;
            else goto _error_;
        }
        else goto _end_;
    
    // Caracter corrente é alfabético
    // Achei o começo de um possivel token 
    _alpha_:
        count++;
        buffer++;
        if (*buffer) {
            if (isalnum(*buffer)) goto _alnum_;
            else if (*buffer == ')') goto _close_parentesis_;
            else if (isspace(*buffer)) goto _space_;
            else goto _error_;
        }
        else goto _end_;
    
    // Caracter corrente é alfanumérico
    // se continuar assim eu tenho um token
    _alnum_:
        buffer++;
        if (*buffer) {
            if (isalnum(*buffer)) goto _alnum_;
            else if (*buffer == ')') goto _close_parentesis_;
            else if (isspace(*buffer)) goto _space_;
            else goto _error_;
        }
        else goto _end_;
    
    // Caracter corrente é um espaço
    _space_:
        buffer++;
        if (*buffer) {
            if (isspace(*buffer)) goto _space_;
            else if (isalpha(*buffer)) goto _alpha_;
            else if (*buffer == '(') goto _open_parentesis_;
            else goto _error_;
        }
        else goto _end_;
    
    _error_:
        return -1;
    
    _end_:
        return count;
}

int _get_states(args_t* args, char* buffer) {
    int curr_char = 0;
    int curr_token = 0;
    int tk_size = 0;
    int i;
    
    if (buffer[curr_char] == '(') {
        goto _open_parentesis_;
    }
    else if (isalpha(buffer[curr_char])) {
        goto _alpha_;
    }
    else if (isspace(buffer[curr_char])) {
        goto _space_;
    }
    else {
        goto _error_;
    }
    
    // Caracter corrente é um (
    _open_parentesis_:
        curr_char++;
        if (buffer[curr_char] && isalpha(buffer[curr_char])) {
            goto _alpha_;
        }
        else {
            goto _error_;
        }
    
    // Caracter corrente é um )
    _close_parentesis_:
        curr_char++;
        if (buffer[curr_char]) {
            if (isspace(buffer[curr_char])) {
                goto _space_;
            }
            else {
                goto _error_;
            }
        }
        else {
            goto _end_;
        }
    
    // Caracter corrente é alfabético
    // Achei o começo de um possivel token 
    _alpha_:
        curr_char++;
        tk_size = 1;
        if (buffer[curr_char]) {
            if (isalnum(buffer[curr_char])) {
                goto _alnum_;
            }
            else if (buffer[curr_char] == ')') {
                args->states[curr_token] = _alloc_token(
                    buffer,
                    curr_char - tk_size,
                    tk_size
                );
                
                if (args->states[curr_token]) {
                    curr_token++;
                    goto _close_parentesis_;
                }
                else {
                    goto _error_;
                }
            }
            else if (isspace(buffer[curr_char])) {
                args->states[curr_token] = _alloc_token(
                    buffer,
                    curr_char - tk_size,
                    tk_size
                );
                
                if (args->states[curr_token]) {
                    curr_token++;
                    goto _space_;
                }
                else {
                    goto _error_;
                }
            }
            else {
                goto _error_;
            }
        }
        else {
            args->states[curr_token] = _alloc_token(
                buffer,
                curr_char - tk_size,
                tk_size
            );
            
            if (args->states[curr_token]) {
                curr_token++;
                goto _end_;
            }
            else {
                goto _error_;
            }
        }
    
    // Caracter corrente é alfanumérico
    // se continuar assim eu tenho um token
    _alnum_:
        curr_char++;
        tk_size++;
        if (buffer[curr_char]) {
            if (isalnum(buffer[curr_char])) {
                goto _alnum_;
            }
            else if (buffer[curr_char] == ')') {
                args->states[curr_token] = _alloc_token(
                    buffer,
                    curr_char - tk_size,
                    tk_size
                );
                
                if (args->states[curr_token]) {
                    args->fstates[curr_token] = 1;
                    args->num_fstates++;
                    curr_token++;
                    goto _close_parentesis_;
                }
                else {
                    goto _error_;
                }
            }
            else if (isspace(buffer[curr_char])) {
                args->states[curr_token] = _alloc_token(
                    buffer,
                    curr_char - tk_size,
                    tk_size
                );
                
                if (args->states[curr_token]) {
                    curr_token++;
                    goto _space_;
                }
                else {
                    goto _error_;
                }
            }
            else {
                goto _error_;
            }
        }
        else {
            args->states[curr_token] = _alloc_token(
                buffer,
                curr_char - tk_size,
                tk_size
            );
            
            if (args->states[curr_token]) {
                curr_token++;
                goto _end_;
            }
            else {
                goto _error_;
            }
        }
    
    // Caracter corrente é um espaço
    _space_:
        curr_char++;
        if (buffer[curr_char]) {
            if (isspace(buffer[curr_char])) {
                goto _space_;
            }
            else if (isalpha(buffer[curr_char])) {
                goto _alpha_;
            }
            else if (buffer[curr_char] == '(') {
                goto _open_parentesis_;
            }
            else{
                goto _error_;
            }
        }
        else {
            goto _end_;
        }
    
    _error_:
        // Cleanup
        for (i = 0; i <= curr_token; i++) {
            free(args->states[i]);
        }
        
        return -1;
    
    _end_:
         return 1;
}

int _num_symbols(char* buffer) {
    int count = 0;
    
    while (*buffer) {
        if (isgraph(*buffer)) {
            count++;
        }
        
        buffer++;
    }
    
    return count;
}

int _get_symbols(char* buffer, char* vector) {
    int curr_sym = 0;
    
    if (isgraph(*buffer)) {
        goto _symbol_;
    }
    else if (isspace(*buffer)) {
        goto _space_;
    }
    else {
        goto _error_;
    }
    
    _symbol_:
        buffer++;
        
        if (*buffer == '\0') {
            vector[curr_sym] = *(buffer - 1);
            curr_sym++;
            
            goto _end_;
        }
        else if (isspace(*buffer)) {
            vector[curr_sym] = *(buffer - 1);
            curr_sym++;
            
            goto _space_;
        }
        else {
            goto _error_;
        }
    
    _space_:
        buffer++;
        
        if (*buffer == '\0') {
            goto _end_;
        }
        else if (isgraph(*buffer)) {
            goto _symbol_;
        }
        else if (isspace(*buffer)) {
            goto _space_;
        }
        else {
            goto _error_;
        }
    
    _error_:
        return -1;
    
    _end_:
        return 1;
}

char* _alloc_token(char* buffer, int first, int count) {
    int num_bytes = sizeof(char) * count;
    char* token = (char*)malloc(num_bytes);
    
    if (token) {
        memset(token, 0, num_bytes);
        strncpy(token, buffer + first, count);
    }
    
    return token;
}

int _parse_line_transition(args_t* args, char* line, int curr_line) {
    char buffer[MAX_BUFF];
    int i, tokens, size, idx;
    
    i = tokens = size = 0;
    
    if (isgraph(line[i])) {
        goto _graph_;
    }
    else if (isspace(line[i])) {
        goto _space_;
    }
    else {
        goto _error_;
    }
    
    _graph_:
        size++;
        i++;
        if (isgraph(line[i])) {
            goto _graph_;
        }
        else {
            if (tokens >= args->num_symbols) {
                goto _error_;
            }
            
            memset(buffer, 0, MAX_BUFF);
            strncpy(buffer, line + i - 2, size);
            idx = state_index(args, buffer);
            idx = idx < 0 ? -1 : idx;
            
            args->transitions[curr_line * args->num_states + tokens] = idx;
            tokens++;
            
            if (line[i] == '\0') {
                goto _end_;
            }
            else if (isspace(line[i])){
                goto _space_;
            }
            else {
                goto _error_;
            }
        }
    
    _space_:
        i++;
        if (isgraph(line[i])) {
            size = 0;
            goto _graph_;
        }
        else if (isspace(line[i])) {
            goto _space_;
        }
        else if(line[i] == '\0') {
            goto _end_;
        }
        else {
            goto _error_;
        }
    
    _error_:
        return 0;
    
    _end_:
        return 1;
}
