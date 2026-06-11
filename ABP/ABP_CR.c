#include "ABP_CR.h"
#include <stdlib.h>


void ABP_CR_cria(ABP_CR* arvore) {
    arvore->raiz = NULL;
}

static void inserirEndereco(
    NoEndereco** lista,
    long endereco
) {
    NoEndereco* novo =
        malloc(sizeof(NoEndereco));

    novo->endereco = endereco;
    novo->prox = *lista;

    *lista = novo;
}

void ABP_CR_insere(
    ABP_CR* arvore,
    int cr,
    long endereco
) {

    if (arvore->raiz == NULL) {

        NoABP_CR* novo =
            malloc(sizeof(NoABP_CR));

        novo->dado.cr = cr;
        novo->dado.lista = NULL;

        inserirEndereco(
            &novo->dado.lista,
            endereco
        );

        novo->esq = NULL;
        novo->dir = NULL;

        arvore->raiz = novo;
        return;
    }

    NoABP_CR* atual = arvore->raiz;
    NoABP_CR* pai = NULL;

    while (atual != NULL) {

        pai = atual;

        if (cr < atual->dado.cr)
            atual = atual->esq;

        else if (cr > atual->dado.cr)
            atual = atual->dir;

        else {

            inserirEndereco(
                &atual->dado.lista,
                endereco
            );

            return;
        }
    }

    NoABP_CR* novo =
        malloc(sizeof(NoABP_CR));

    novo->dado.cr = cr;
    novo->dado.lista = NULL;

    inserirEndereco(
        &novo->dado.lista,
        endereco
    );

    novo->esq = NULL;
    novo->dir = NULL;

    if (cr < pai->dado.cr)
        pai->esq = novo;
    else
        pai->dir = novo;
}

static int contarLista(
    NoEndereco* lista
) {

    int total = 0;

    while (lista != NULL) {
        total++;
        lista = lista->prox;
    }

    return total;
}

static int contaMaioresRec(
    NoABP_CR* no,
    int limiar
) {

    if (no == NULL)
        return 0;

    if (no->dado.cr <= limiar) {

        return contaMaioresRec(
            no->dir,
            limiar
        );
    }

    return
        contarLista(no->dado.lista)
        +
        contaMaioresRec(
            no->esq,
            limiar
        )
        +
        contaMaioresRec(
            no->dir,
            limiar
        );
}

int ABP_CR_contaMaiores(
    ABP_CR* arvore,
    int limiar
) {
    return contaMaioresRec(
        arvore->raiz,
        limiar
    );
}
