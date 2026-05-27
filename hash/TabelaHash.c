#include <stdlib.h>
#include <string.h>
#include "TabelaHash.h"

struct hash {
    int qtd, TABLE_SIZE, colisoes;
    Aluno **itens, **overflow;
};

Hash* criaHash (int TABLE_SIZE){
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if(ha){
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->itens = (Aluno**) malloc(TABLE_SIZE * sizeof(Aluno*));
        ha->overflow = (Aluno**) malloc((TABLE_SIZE/3) * sizeof(Aluno*));
        if(!ha->itens){
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        ha->colisoes = 0;
        for(int i=0; i< ha->TABLE_SIZE; i++){
            ha->itens[i] = NULL;
        }
        for(int i=0; i< (ha->TABLE_SIZE) / 3; i++){
            ha->overflow[i] = NULL;
        }
    }
    return ha;
}

void liberaHash(Hash* ha){
    if(ha){
        for(int i=0; i<(ha->TABLE_SIZE); i++){
            if(ha->itens[i]){
                free(ha->itens[i]);
            }
        }
        for (int j =0; j<(ha->TABLE_SIZE/3); j++){
            free(ha->overflow[j]);
        }
        free(ha->itens);
        free(ha->overflow);
        free(ha);
    }
}

int mostrarColisoes(Hash* ha){
    if(ha){
        return ha->colisoes;
    }
    return 0;
}

int matriculaDivisao(int matricula, int TABLE_SIZE){
    return matricula % TABLE_SIZE;
}

int insereHash_AreaOverflow(Hash* ha, Aluno* dado){
    if(!ha || ha->qtd == ha->TABLE_SIZE) return 0;
    int matricula = dado->matricula;
    int pos = matriculaDivisao(matricula, ha->TABLE_SIZE);
    Aluno* novo = (Aluno*) malloc(sizeof(Aluno));
    if(!novo) return 0;
    *novo = *dado;
    if(!ha->itens[pos]){
        ha->itens[pos] = novo;
        ha->qtd++;
        return 1;
    } else {
        ha->colisoes++;
        for(int i=0; i<(ha->TABLE_SIZE/3); i++){
            if(!ha->overflow[i]){
                ha->overflow[i] = novo;
                ha->qtd++;
                return 1;
            }
        }
    }
    free(novo);
    return 0;
}

int buscaHash_AreaOverflow(Hash* ha, int matricula){
    if(!ha) return 0;
    int pos = matriculaDivisao(matricula, ha->TABLE_SIZE);
    if(ha->itens[pos] && ha->itens[pos]->matricula == matricula) return 1;
    else{
        for(int i=0; i<(ha->TABLE_SIZE/3); i++){
            if(ha->overflow[i] && ha->overflow[i]->matricula == matricula){
                return 1;
            }
        }
    }
    return 0;
}