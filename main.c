#include "stdio.h"
#include "stdlib.h"
#include "./dados/dados.h"
#include "./hash/TabelaHash.h"
#define TAM 10000
int main(){
    Aluno* alunos = gerarVetorAlunosAleatorios(TAM);
    FILE *arquivo;
    arquivo = fopen("arquivoAlunos.bin", "wb");

    fwrite(alunos, sizeof(Aluno), TAM, arquivo);

    fclose(arquivo);
}