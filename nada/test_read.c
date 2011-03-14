#include <stdio.h>
#include <stdlib.h>

#include "args.h"

int main(int argc, char** argv) {
    args_t args;
    int i;
    
    if (get_args("test.auto", &args)) {
        print_args(&args);
    }
    else {
        printf("Falha ao obter args\n");
    }
    
    return EXIT_SUCCESS;
}
