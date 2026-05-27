#include <stdio.h>
#include <stdlib.h>
#include "./dados/dados.h"
#include "./hash/TabelaHash.h"
#define TAM 10000
#define INICIALIZAR 100003
int main(){

// Dá pra transformar isso aqui na função "CriarArquivoDeVetor"
// Criação do vetor e do ponteiro para o arquivo
    Aluno** alunos = gerarVetorAlunosAleatorios(TAM);
    FILE *arquivo;
 // Criação do vetor de indices   
    Indice** indices = gerarEsquemaIndice(alunos, TAM);
// Criação do arquivo binário com 10000 alunos aleatórios
  // só criar um novo arquivo se n houver nenhum outro criado?
     arquivo = fopen("arquivoAlunos.bin", "w+b");
    for (int i=0; i<TAM; i++){
        fwrite(alunos[i], sizeof(Aluno), 1, arquivo);
    }

    // Inserindo os elementos no hash
    Hash* indexador= criaHash(INICIALIZAR);
    for(int i=0; i<TAM; i++){
        insereHash_AreaOverflow(indexador, indices[i]);
    }
    printf("Colisoes: %d \n", mostrarColisoes(indexador));

    // Fazer busca por matricula
    int buscado = alunos[10]->matricula;
    int endereco = buscaHash_AreaOverflow(indexador, buscado);
    printf("Matricula buscada: %d, endereco: %d \n", buscado, endereco);
    Aluno retornado = retornarRegistroEmArquivo(endereco, arquivo);

    printf("Nome: %s, curso: %s", retornado.nome, retornado.curso);



fclose(arquivo);
}