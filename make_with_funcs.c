#include <stdio.h>
#include <stdlib.h>
#include "args.h"
#include "make_with_funcs.h"

void make_with_func(args_t* args, char* outfile){
    
    int i, j, count;
    
    if (get_args("test.auto", args)) {
        //cria arquivo 
		FILE * arq = fopen(outfile, "w");
		
		//insere includes
		fprintf(arq, "#include <stdio.h>\n");
		fprintf(arq, "#include <stdlib.h>\n");
		fprintf(arq, "#define TAM 10\n");
		fprintf(arq, "\n");
		
		//variaveis globais
		fprintf(arq, "char v[TAM];\n");
		fprintf(arq, "int p = 0;\n");

		fprintf(arq, "\n");
		
		//protitipos das funcões
		for(i = 0; i < args->num_states; i++){
		    fprintf(arq, "void e%d();\n", i);
		}    
		fprintf(arq, "void sucesso();\n");
		fprintf(arq, "void erro();\n");
		
		fprintf(arq, "\n");

		//main
		fprintf(arq, "int main(void) {\n");
		fprintf(arq, "    p = 0;\n");
		fprintf(arq, "    printf(\"Sequencia:\"); \n");
		fprintf(arq, "    fgets(v, TAM, stdin);\n");
		fprintf(arq, "    e0();\n");
		fprintf(arq, "    return(0);\n");
		fprintf(arq, "}\n");
		
		fprintf(arq, "\n");
		
		//funcoes dos estados
		for (i = 0; i < args->num_states; i++) {
		    fprintf(arq, "\nvoid %s(){\n", args->states[i]);            
		    count = 0;
		               
		    for (j = 0; j < args->num_symbols; j++) {
		        if (args.transitions[i + j * args->num_states] != -1) {
		            if (count > 0){
		                fprintf(arq, " else \n");
		                fprintf(arq, "        if(v[p] == '%c'){ \n", args->symbols[j]);
		            }
		            else{
		                   fprintf(arq, "    if(v[p] == '%c'){ \n", args->symbols[j]);
		            }    
		                fprintf(arq, "        p++;\n");
		                fprintf(arq, "        e%d();\n", args->transitions[i + j * args->num_states]);
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
		
		fprintf(arq, "\n");
		
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
    }
    else {
        printf("Falha ao obter args\n");
    }
    
    
}
