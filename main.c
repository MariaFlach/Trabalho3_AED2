#include <stdio.h>
#include <stdlib.h>
#include "dados/dados.h"
#include "hash/TabelaHash.h"
#include "ABP/ABP.h"
#include <time.h>
#define TAM 10000
#define INICIALIZAR 100003
#define TOTAL_BUSCAS 30
int main(){
    
    // 
    Aluno** alunos = gerarVetorAlunosAleatorios(TAM);
    FILE *arquivo; 
    Indice** indices = gerarEsquemaIndice(alunos, TAM);
    arquivo = fopen("arquivoAlunos.bin", "w+b");
    for (int i=0; i<TAM; i++){
        fwrite(alunos[i], sizeof(Aluno), 1, arquivo);

    } // pensar se não seria melhor escrever o vetor inteiro logo, mas teria que ver como ficaria a questão do preenchimento dos endereços de memória, como o processo de escrever no arquivo só ocorre uma vez, creio que essa iteração não seja um grande problema

    // ----------------------------------- QUESTÃO 1 --------------------------------------------------------------------------

    printf("--------------------------------------------QUESTÃO 1------------------\n");
    ABP arvore;
    ABP_cria(&arvore);

       //inserindo os indices na ABP


       for (int i = 0; i < TAM; i++)
       {
        ABP_insere(&arvore, *indices[i]);
       }

       // executando as 30 buscas e medindo o resultado

        printf("INICIANDO EXPERIMENTO DAS 30 BUSCAS NA ABP\n");
        double tempo_total = 0.0;

        for(int i = 0; i < TOTAL_BUSCAS; i++){

            int matricula_procurada = alunos[i * (TAM/TOTAL_BUSCAS)] -> matricula;
            int endereco_encontrado;

            //cronometro 
            clock_t inicio = clock();


            // busca na abp
            int achou = ABP_busca(&arvore, matricula_procurada, &endereco_encontrado);

            if(achou){

                fseek(arquivo, endereco_encontrado, SEEK_SET);
                Aluno aluno_encontrado;
                fread(&aluno_encontrado, sizeof(Aluno), 1, arquivo);

                clock_t fim = clock();
                double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
                tempo_total += tempo_gasto;

                printf("ABP [BUSCA %02d]: Mat %d -> %s | Tempo: %f s\n", i+1, matricula_procurada, aluno_encontrado.nome, tempo_gasto);

            }
        }
        printf("----------------------------------------");
        printf("TEMPO MEDIO DA BUSCA POR ABP: %f segundos \n", tempo_total / TOTAL_BUSCAS);
        printf("----------------------------------------");
       

    
    printf("--------------------------------------------QUESTÃO 2-----------------\n");
    // Inserindo os elementos no hash
    Hash* indexador= criaHash(INICIALIZAR);
    // inserindo os indices na tabela Hash
    inserirIndicesNoHash(indexador, TAM, indices);
    // Fazer busca por matricula (já mostra o retorno)
    Aluno retornado = buscaPorMatricula(indexador, alunos[10]->matricula, arquivo);





fclose(arquivo);
}