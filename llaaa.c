#include <stdio.h>
#include <stdlib.h>
#define TAM 10

char v[TAM];
int p = 0;

void e0();
void e1();
void e2();
void sucesso();
void erro();

int main(void) {
    p = 0;
    printf("Sequencia:"); 
    fgets(v, TAM, stdin);
    e0();
    return(0);
}


void e0(){
    if(v[p] == 'a'){ 
        p++;
        e1();
    } else 
        if(v[p] == 'b'){ 
        p++;
        e1();
    } else 
        if(v[p] == 'c'){ 
        p++;
        e2();
    } else {
    erro();
    }
}

void e1(){
    if(v[p] == 'b'){ 
        p++;
        e2();
    } else 
        if(v[p] == 'c'){ 
        p++;
        e2();
    } else {
    erro();
    }
}

void e2(){
    if(v[p] == 'a'){ 
        p++;
        e0();
    } else 
        if(v[p] == 'b'){ 
        p++;
        e0();
    } else 
        if(v[p] == 'c'){ 
        p++;
        e2();
    } else {
    sucesso();
    }
}

void sucesso(){
    printf(" Sucesso!");
}

void erro(){
    printf(" Erro!");
}

