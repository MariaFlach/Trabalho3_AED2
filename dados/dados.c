#include "dados.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void geraString(char* dest, int min, int max) {
    int tam = min + rand() % (max - min + 1);
    for (int i = 0; i < tam; i++)
        dest[i] = 'a' + rand() % 26;
    dest[tam] = '\0';
}

void geraNome(char* dest) {
    char primeiro[25], segundo[25];
    geraString(primeiro, 4, 10);
    geraString(segundo,  4, 10);
    primeiro[0] -= 32;
    segundo[0]  -= 32;
    strcpy(dest, primeiro);
    strcat(dest, " ");
    strcat(dest, segundo);
}

Aluno geraAlunoAleatorio() {
    Aluno aluno;
    geraNome(aluno.nome);
    aluno.CR = 4.0f + (float)(rand() % 61) / 10.0f;
    aluno.matricula = 225500000 + (int)(((long long)rand() << 15 | rand()) % 1000000);
    geraString(aluno.curso, 5, 15);
    aluno.curso[0] -= 32;
    aluno.pibic = rand() % 2;
    if (aluno.pibic == 1) {
        char sobrenome[25];
        geraString(sobrenome, 4, 10);
        sobrenome[0] -= 32;
        strcpy(aluno.orientador, "Prof. ");
        strcat(aluno.orientador, sobrenome);
    } else {
        strncpy(aluno.orientador, "n/a", sizeof(aluno.orientador) - 1);
    }

    return aluno;
}

int geraMatriculaAleatoria(){
    return 225500000 + (int)(((long long)rand() << 15 | rand()) % 1000000);
}

// Durante a geração do vetor de alunos, é preciso verificar o endereço de inicio;
// preciso revisar o conteúdo 

// SERIA IDEAL TRANSFORMAR EM VETOR DE PONTEIROS DO TIPO ALUNO? PORQUE HASH É PONTEIRO PARA PONTEIRO
// TEM QUE ALOCAR CADA ELEMENTO DO VETOR DE PONTEIROS
Aluno** gerarVetorAlunosAleatorios(int tam){
    srand(time(NULL));
    Aluno** alunos = malloc(tam * sizeof(Aluno*));
    Aluno novo;
    if (!alunos) return NULL;
    for(int i=0; i<tam; i++){
        novo = geraAlunoAleatorio();
        alunos[i] = malloc(sizeof(Aluno));
        *alunos[i] = novo;
    }
    return alunos;
}

// Para cada elemento do vetor dos alunos, o indice contém a matricula e o endereço de inicio, será possível acessar usando o endereço de inico + sizeof(aluno)
// SERIA IDEAL TRANSFORMAR EM VETOR DE PONTEIROS DO TIPO ALUNO? PORQUE HASH É PONTEIRO PARA PONTEIRO
Indice** gerarEsquemaIndice(Aluno** alunos, int tam){
    Indice** indices = (Indice**) malloc(tam * sizeof(Indice*));
    for (int i=0; i<tam; i++){
        indices[i] = malloc(sizeof(Indice));
        indices[i]->matricula = alunos[i]->matricula;
        indices[i]->endereco_inicio = i*sizeof(Aluno);
    }
    return indices;
}

// Sortear tamVetor posições aleatorias de um vetor de entrada e gerar um vetor de saida com essas posições
void retornarPosicaoVetor(int tamVetorSaida, int tamVetorEntrada, int vetorSaida[]){
    for(int i=0; i<tamVetorSaida; i++){
        vetorSaida[i] = (int) rand() % tamVetorEntrada;
    } 
}
