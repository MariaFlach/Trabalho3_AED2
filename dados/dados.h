#ifndef DADOS_H
#define DADOS_H

typedef struct Aluno {
    char nome[50];
    float CR;
    int matricula;
    char curso[30];
    char pibic;
    char orientador[30];
} Aluno;

void geraString(char* dest, int min, int max);
void geraNome(char* dest);
Aluno geraAlunoAleatorio();
Aluno* gerarVetorAlunosAleatorios(int tam);

#endif