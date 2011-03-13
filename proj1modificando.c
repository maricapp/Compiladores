#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int num_states;
    int num_symbols;
    char** states;
    char** symbols;
    int* transitions;
} args_t;

int* get_matrix() {
    int* matrix = (int*) malloc(4 * 3 * sizeof(int));

    if (matrix) {
        matrix[0] = 0;
        matrix[1] = -1;
        matrix[2] = 2;
        matrix[3] = -1;
        matrix[4] = -1;
        matrix[5] = -1;
        matrix[6] = 2;
        matrix[7] = 0;
        matrix[8] = -1;
        matrix[9] = 1;
        matrix[10] = 2;
        matrix[11] = -1;
    }

    return matrix;
}

int is_final_state(args_t* args, int state){ 
	return state == 2;
}

int main(){
	
    int i, j, count;
    char symbols[4] = { 'a', 'b', 'c', 'd' };
    char* states[3] = { "e0", "e1", "e2" };    
    
    args_t args;
    args.num_states = 3;
    args.num_symbols = 4;
       
    args.symbols = symbols;
    args.states = states;
    args.transitions = get_matrix();
   	
   	//cria arquivo 
	FILE * arq = fopen("teste.c", "w");
	//insere includes
	fprintf(arq, "#include <stdio.h>\n");
	fprintf(arq, "#include <stdlib.h>\n");
	
	fprintf(arq, "\n");
	
	//variaveis globais
	fprintf(arq, "char v[8];\n");
	fprintf(arq, "int p = 0;\n");

	fprintf(arq, "\n");
	
	//protitipos das funcões
	for(i = 0; i < args.num_states; i++){
		fprintf(arq, "void e%d();\n", i);
	}	
	fprintf(arq, "void sucesso();\n");
	fprintf(arq, "void erro();\n");
	
	fprintf(arq, "\n");

	//main
	fprintf(arq, "int main(void) {\n");
	fprintf(arq, "    p = 0;\n");
	fprintf(arq, "    printf(\"Sequencia:\"); \n");
	fprintf(arq, "    fgets(v, 8, stdin);\n");
	fprintf(arq, "    e0();\n");
	fprintf(arq, "    return(0);\n");
	fprintf(arq, "}\n");
	
	fprintf(arq, "\n");
	
	//funcoes dos estados
		for (i = 0; i < args.num_states; i++) {
        	fprintf(arq, "\nvoid %s(){\n", states[i]);        	
       		count = 0;
       		
        for (j = 0; j < args.num_symbols; j++) {
            if (args.transitions[i + j * args.num_states] != -1) {
           		if (count > 0){
					fprintf(arq, " else \n");
            		fprintf(arq, "        if(v[p] == '%c'){ \n", symbols[j]);
                }
                else{
               		fprintf(arq, "    if(v[p] == '%c'){ \n", symbols[j]);
                }	
                	fprintf(arq, "        p++;\n");
                	fprintf(arq, "        e%d();\n", args.transitions[i + j * args.num_states]);
                	fprintf(arq, "    }");
	                count++;
              
            }
        }
        
        if (count > 0) {
        	fprintf(arq, " else {\n");
        }
        if (is_final_state(&args, i)){
        	fprintf(arq, "    sucesso();\n");
			fprintf(arq, "    }\n");
			fprintf(arq, "}\n");   
        }
        else {
        	fprintf(arq, "    erro();\n");
			fprintf(arq, "    }\n");        	
    		fprintf(arq, "}\n");   
    	}
    }
	//cria funcao sucesso
	fprintf(arq, "void sucesso(){\n");	
	fprintf(arq, "    printf(\" Sucesso!\");\n");
	fprintf(arq, "}\n");
	
	fprintf(arq, "\n");
	
	//cria funcao erro
	fprintf(arq, "void erro(){\n");	
	fprintf(arq, "    printf(\" Erro!\");\n");
	fprintf(arq, "}\n");
	
	fprintf(arq, "\n");
	//fecha arquivo
	fclose(arq);
		
	return(0);	
}
