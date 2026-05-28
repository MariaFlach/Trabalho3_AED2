#ifndef ABP_H
#define ABP_H

#include "../dados/dados.h"

typedef struct NoABP {
    Indice dado;
    struct NoABP* esq;
    struct NoABP* dir;
} NoABP;

typedef struct ABP {
    NoABP* raiz;
} ABP;

void ABP_cria(ABP* arvore);
void ABP_insere(ABP* arvore, Indice idx);
int  ABP_busca(ABP* arvore, int matricula, int* endereco_out);
void ABP_emOrdem(NoABP* raiz);
void ABP_imprime(ABP* arvore);

#endif