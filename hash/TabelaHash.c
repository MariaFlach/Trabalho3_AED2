#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TabelaHash.h"
#include "../lista/Lista.h"

struct hash {
    int qtd, TABLE_SIZE, colisoes;
    Lista **registros;
};

Hash* criaHash(int TABLE_SIZE) {
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if (ha) {
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->registros = (Lista**) malloc(TABLE_SIZE * sizeof(Lista*));
        if (!ha->registros) {
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        ha->colisoes = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            ha->registros[i] = criaLista();
        }
    }
    return ha;
}

void liberaHash(Hash* ha) {
    if (ha) {
        for (int i = 0; i < ha->TABLE_SIZE; i++) {
            liberaLista(ha->registros[i]);
        }
        free(ha->registros);
        free(ha);
    }
}

int mostrarColisoes(Hash* ha) {
    if (ha) return ha->colisoes;
    return 0;
}

int matriculaHash(int matricula, int TABLE_SIZE) {
    unsigned int h = (unsigned int) matricula;
    h = (h ^ 61) ^ (h >> 16);
    h += (h << 3);
    h ^= (h >> 4);
    h *= 0x27d4eb2du;
    h ^= (h >> 15);
    return (int) (h % (unsigned int) TABLE_SIZE);
}

int insereHash_Encadeamento(Hash* ha, Indice* dado) {
    if (!ha) return 0;
    int pos = matriculaHash(dado->matricula, ha->TABLE_SIZE);
    if (ha->registros[pos]->tamanho > 0) {
        ha->colisoes++;
    }
    insereLista(ha->registros[pos], *dado);
    ha->qtd++;
    return 1;
}

int buscaHash_Encadeamento(Hash* ha, int matricula) {
    if (!ha) return -1;
    int pos = matriculaHash(matricula, ha->TABLE_SIZE);
    return buscaLista(ha->registros[pos], matricula);
}

Aluno retornarRegistroEmArquivo(int endereco, FILE *arquivo) {
    Aluno aluno;
    fseek(arquivo, endereco, SEEK_SET);
    fread(&aluno, sizeof(Aluno), 1, arquivo);
    return aluno;
}

Aluno buscaPorMatricula(Hash* ha, int matricula, FILE* arquivo) {
    Aluno retornado;
    retornado.matricula = -1;
    retornado.CR = -1.0f;
    retornado.pibic = -1;
    strncpy(retornado.nome, "Não encontrado", sizeof(retornado.nome) - 1);
    retornado.nome[sizeof(retornado.nome) - 1] = '\0';
    strncpy(retornado.curso, "Não encontrado", sizeof(retornado.curso) - 1);
    retornado.curso[sizeof(retornado.curso) - 1] = '\0';
    strncpy(retornado.orientador, "Não encontrado", sizeof(retornado.orientador) - 1);
    retornado.orientador[sizeof(retornado.orientador) - 1] = '\0';
    int endereco = buscaHash_Encadeamento(ha, matricula);
    if (endereco == -1) return retornado;
    fseek(arquivo, endereco, SEEK_SET);
    fread(&retornado, sizeof(Aluno), 1, arquivo);
    return retornado;
}

int inserirIndicesNoHash(Hash* ha, int qt_indices, Indice** indices) {
    for (int i = 0; i < qt_indices; i++) {
        insereHash_Encadeamento(ha, indices[i]);
    }
    printf("Quantidade de colisoes: %d \n", mostrarColisoes(ha));
    return mostrarColisoes(ha);
}
