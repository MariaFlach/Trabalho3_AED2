#ifndef ABP_CR_H
#define ABP_CR_H

typedef struct NoEndereco {
    long endereco;
    struct NoEndereco* prox;
} NoEndereco;

typedef struct {
    int cr;
    NoEndereco* lista;
} IndiceCR;

typedef struct NoABP_CR {
    IndiceCR dado;
    struct NoABP_CR* esq;
    struct NoABP_CR* dir;
} NoABP_CR;

typedef struct {
    NoABP_CR* raiz;
} ABP_CR;

void ABP_CR_cria(ABP_CR* arvore);

void ABP_CR_insere(
    ABP_CR* arvore,
    int cr,
    long endereco
);

int ABP_CR_contaMaiores(
    ABP_CR* arvore,
    int limiar
);

#endif
