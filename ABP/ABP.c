#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ABP.h"

void ABP_cria(ABP* arvore) {
    arvore->raiz = NULL;
}

void ABP_insere(ABP* arvore, Indice idx) {
    NoABP* novo = malloc(sizeof(NoABP));
    novo->dado = idx;
    novo->esq  = NULL;
    novo->dir  = NULL;

    if (!arvore->raiz) {
        arvore->raiz = novo;
        return;
    }

    NoABP* anterior  = NULL;
    NoABP* atual = arvore->raiz;

    while (atual != NULL) {
        anterior = atual;
        if (idx.matricula > atual->dado.matricula)
            atual = atual->dir;
        else if (idx.matricula < atual->dado.matricula)
            atual = atual->esq;
        else {
            return;
        }
    }

    if (idx.matricula > anterior->dado.matricula)
        anterior->dir = novo;
    else
        anterior->esq = novo;
}

//O ENDERECO OUT É REALMENTE NECESSÁRIO???? VERIFICAR DEPOIS 
int ABP_busca(ABP* arvore, int matricula, int* endereco_out) {
    NoABP* atual = arvore->raiz;

    while (atual != NULL) {
        if (atual->dado.matricula == matricula) {
            *endereco_out = atual->dado.endereco_inicio;
            return 1;
        } else if (matricula > atual->dado.matricula)
            atual = atual->dir;
        else
            atual = atual->esq;
    }

    return 0;
}

void visitaNo(NoABP* no) {
    printf("<matricula=%d, endereco=%d>\n", no->dado.matricula, no->dado.endereco_inicio);
}

void ABP_emOrdem(NoABP* raiz) {
    if (raiz) {
        ABP_emOrdem(raiz->esq);
        visitaNo(raiz);
        ABP_emOrdem(raiz->dir);
    }
}

void imprimeBonito(NoABP* no, char* prefixo, int isEsq) {
    if (no == NULL) return;
    printf("%s", prefixo);
    printf(isEsq ? "|-- e: " : "\\-- d: ");
    printf("<mat=%d, end=%d>\n", no->dado.matricula, no->dado.endereco_inicio);

    char novoPrefixo[1000];
    sprintf(novoPrefixo, "%s%s", prefixo, isEsq ? "|   " : "    ");
    imprimeBonito(no->esq, novoPrefixo, 1);
    imprimeBonito(no->dir, novoPrefixo, 0);
}

void ABP_imprime(ABP* arvore) {
    if (!arvore->raiz) {
        printf("Arvore vazia.\n");
        return;
    }
    printf("<mat=%d, end=%d>\n",
           arvore->raiz->dado.matricula,
           arvore->raiz->dado.endereco_inicio);
    imprimeBonito(arvore->raiz->esq, "", 1);
    imprimeBonito(arvore->raiz->dir, "", 0);
}

Aluno ABP_retornaAluno(ABP* arvore, int matricula, FILE*arquivo){
    Aluno aluno;
    int endereco;

    int achou = ABP_busca(arvore, matricula, &endereco);
    if(achou){
        fseek(arquivo, endereco, SEEK_SET);
        fread(&aluno, sizeof(Aluno), 1, arquivo);
    } else{
        aluno.matricula = -1;
        strncpy(aluno.nome, "NAO ENCONTRADO", sizeof(aluno.nome)-1);
    }
    return aluno;
}