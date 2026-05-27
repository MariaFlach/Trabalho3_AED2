#include "dados.h"
#include <stdlib.h>
#include <string.h>
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
    aluno.CR = (float)(rand() % 101) / 10.0f;
    aluno.matricula = 22550000 + (rand() % 10000);
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

Aluno* gerarVetorAlunosAleatorios(int tam){
    Aluno* alunos = malloc(tam * sizeof(Aluno));
    if (!alunos) return NULL;
    for(int i=0; i<tam; i++){
        alunos[i] = geraAlunoAleatorio();
    }
    return alunos;
}
