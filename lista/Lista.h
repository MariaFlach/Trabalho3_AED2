#ifndef LISTA_H
#define LISTA_H

#include "../dados/dados.h"

typedef struct NoLista {
    Indice dado;
    struct NoLista* proximo;
} NoLista;

typedef struct Lista {
    NoLista* cabeca;
    int tamanho;
} Lista;

Lista* criaLista();
void liberaLista(Lista* lista);
void insereLista(Lista* lista, Indice dado);
int buscaLista(Lista* lista, int matricula);

#endif
