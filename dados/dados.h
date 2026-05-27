#ifndef DADOS_H
#define DADOS_H
#include <stdio.h>
typedef struct Aluno {
    char nome[50];
    float CR;
    int matricula;
    char curso[30];
    int pibic;
    char orientador[30];
} Aluno;

typedef struct Indice{
    int matricula;
    int endereco_inicio;
}Indice;

void geraString(char* dest, int min, int max);
void geraNome(char* dest);
Aluno geraAlunoAleatorio();
Aluno** gerarVetorAlunosAleatorios(int tam);
Indice** gerarEsquemaIndice(Aluno** alunos, int tam);
#endif