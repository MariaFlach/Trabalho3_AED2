#include <stdlib.h>
#include <string.h>
#include "TabelaHash.h"

struct hash {
    int qtd, TABLE_SIZE, colisoes;
    Indice **registros, **overflow;
};

// preciso adaptar as funções da tabela hash para colocar o indice

Hash* criaHash (int TABLE_SIZE){
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if(ha){
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->registros = (Indice**) malloc(TABLE_SIZE * sizeof(Indice*));
        ha->overflow= (Indice**) malloc((TABLE_SIZE/3) * sizeof(Indice*));
        if(!ha->registros){
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        ha->colisoes = 0;
        for(int i=0; i< ha->TABLE_SIZE; i++){
            ha->registros[i] = NULL;
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
            if(ha->registros[i]){
                free(ha->registros[i]);
            }
        }
        for (int j =0; j<(ha->TABLE_SIZE/3); j++){
            free(ha->overflow[j]);
        }
        free(ha->registros);
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

int insereHash_AreaOverflow(Hash* ha, Indice* dado){
    if(!ha || ha->qtd == ha->TABLE_SIZE) return 0;
    int matricula = dado->matricula;
    int pos = matriculaDivisao(matricula, ha->TABLE_SIZE);
    Indice* novo = (Indice*) malloc(sizeof(Indice));
    if(!novo) return 0;
    *novo = *dado;
    if(!ha->registros[pos]){
        ha->registros[pos] = novo;
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


// A função de busca deve não só retornar se o elemento foi encontrado ou não, mas também deve retornar o endereço de inicio 
int buscaHash_AreaOverflow(Hash* ha, int matricula){
    if(!ha) return 0;
    int pos = matriculaDivisao(matricula, ha->TABLE_SIZE);
    if(ha->registros[pos] && ha->registros[pos]->matricula == matricula) return ha->registros[pos]->endereco_inicio;
    else{
        for(int i=0; i<(ha->TABLE_SIZE/3); i++){
            if(ha->overflow[i] && ha->overflow[i]->matricula == matricula){
                return ha->overflow[i]->endereco_inicio;
            }
        }
    }
    return -1;
}

Aluno retornarRegistroEmArquivo(int endereco, FILE *arquivo){
    Aluno aluno;
    // preciso posicionar o cursor no primeiro byte do arquivo
    // estamos começando do começo do arquivo e indo até o endereço de onde começa o elemento que estamos buscando
    fseek(arquivo, endereco, SEEK_SET);
    fread(&aluno, sizeof(Aluno), 1, arquivo);
    return aluno;
}

Aluno buscaPorMatricula (Hash* ha,int matricula, FILE* arquivo){
    Aluno retornado;
    int endereco = buscaHash_AreaOverflow(ha, matricula);
    fseek(arquivo, endereco, SEEK_SET);
    fread(&retornado, sizeof(Aluno), 1, arquivo);
    printf("Nome: %s \n CR: %f \n Matricula: %d \n Curso: %s \n PIBIC: %d \n Orientador: %s \n\n ", retornado.nome, retornado.CR, retornado.matricula, retornado.curso, retornado.pibic, retornado.orientador);
    return retornado;
}

int inserirIndicesNoHash(Hash* ha, int qt_indices,      Indice** indices){
    for(int i=0; i<qt_indices; i++){
        insereHash_AreaOverflow(ha, indices[i]);
    }
    printf("Quantidade de colisoes: %d \n", mostrarColisoes(ha));
    return mostrarColisoes(ha);
}