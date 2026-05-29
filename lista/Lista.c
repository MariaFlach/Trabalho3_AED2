#include <stdlib.h>
#include "Lista.h"

Lista* criaLista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    if (lista) {
        lista->cabeca = NULL;
        lista->tamanho = 0;
    }
    return lista;
}

void liberaLista(Lista* lista) {
    if (lista) {
        NoLista* atual = lista->cabeca;
        while (atual) {
            NoLista* prox = atual->proximo;
            free(atual);
            atual = prox;
        }
        free(lista);
    }
}

void insereLista(Lista* lista, Indice dado) {
    NoLista* novo = (NoLista*) malloc(sizeof(NoLista));
    if (!novo) return;
    novo->dado = dado;
    novo->proximo = lista->cabeca;
    lista->cabeca = novo;
    lista->tamanho++;
}

/* Retorna endereco_inicio do Indice com a matricula dada, ou -1 se não encontrado */
int buscaLista(Lista* lista, int matricula) {
    NoLista* atual = lista->cabeca;
    while (atual) {
        if (atual->dado.matricula == matricula)
            return atual->dado.endereco_inicio;
        atual = atual->proximo;
    }
    return -1;
}
