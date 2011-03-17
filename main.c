/*
 * main.c
 *
 *  Created on: Mar 10, 2011
 *      Author: blabos
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "make_with_jumps.h"
#include "make_with_funcs.h"


int main(int argc, char** argv) {
    args_t args;
    int status = EXIT_SUCCESS;
    
    if (argc != 3) {
        fprintf(stderr, "Argumentos invalidos. Use:\n");
        fprintf(stderr, "    %s infile outfile\n", argv[0]);
        return -1;
    }
    
    if (get_args(argv[1], &args)) {
        if (args.mode & 0x1) {
            make_with_jumps(&args, argv[2]);
        }
        
        if (args.mode & 0x2) {
            make_with_func(&args, argv[2]);
        }
    }
    else {
        fprintf(stderr, "Falha ao ler o arquivo do automato\n");
        status = -1;
    }
    return status;
}
