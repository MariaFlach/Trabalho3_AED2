#include <stdio.h>
#include <stdlib.h>
#include "./dados/dados.h"
#include "./hash/TabelaHash.h"
#define TAM 10000
#define INICIALIZAR 100003
int main(){
    
    // 
    Aluno** alunos = gerarVetorAlunosAleatorios(TAM);
    FILE *arquivo; 
    Indice** indices = gerarEsquemaIndice(alunos, TAM);
    arquivo = fopen("arquivoAlunos.bin", "w+b");
    for (int i=0; i<TAM; i++){
        fwrite(alunos[i], sizeof(Aluno), 1, arquivo);
    } // pensar se não seria melhor escrever o vetor inteiro logo, mas teria que ver como ficaria a questão do preenchimento dos endereços de memória, como o processo de escrever no arquivo só ocorre uma vez, creio que essa iteração não seja um grande problema

    // Inserindo os elementos no hash
    Hash* indexador= criaHash(INICIALIZAR);
    // inserindo os indices na tabela Hash
    inserirIndicesNoHash(indexador, TAM, indices);
    // Fazer busca por matricula (já mostra o retorno)
    Aluno retornado = buscaPorMatricula(indexador, alunos[10]->matricula, arquivo);





fclose(arquivo);
}