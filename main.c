#include <stdio.h>
#include <stdlib.h>
#include "dados/dados.h"
#include "hash/TabelaHash.h"
#include "ABP/ABP.h"
#include <time.h>
#define TAM 10000
#define INICIALIZAR 14281
#define TOTAL_BUSCAS 30

int buscaSequencialPorMatricula(FILE *fp, int matricula) {
    Aluno al;
    fseek(fp, 0, SEEK_SET);
    while(fread(&al, sizeof(Aluno), 1, fp) == 1) {
        if(al.matricula == matricula) {

            return ftell(fp) - sizeof(Aluno);
        }
    }
    return -1;
}

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

       //preparando para as 30 buscas (15 existentes E 15 ALEATORIAS)
       int matriculasBuscar[TOTAL_BUSCAS];

       //Buscando os 15 PRIMEIROS ELEMENTOS EXISTENTES
       for (int i = 0; i < 15; i++)
       {
        matriculasBuscar[i] = alunos[i  * (TAM/15)] -> matricula;
       }
       
       // ultimas 15 buscas: ELEMENTOS INEXISTENTES
         for (int i = 15; i < TOTAL_BUSCAS; i++)
         {
          matriculasBuscar[i] = rand() % 1000000;
         }



       // executando as 30 buscas (15 EXISTENTES E 15 ALEATORIAS)
        printf("INICIANDO EXPERIMENTO DAS 30 BUSCAS NA ABP\n");
        double tempo_total = 0.0;

        for(int i = 0; i < TOTAL_BUSCAS; i++){
            int matricula_procurada = matriculasBuscar[i];
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

            } else{

                clock_t fim = clock();
                double tempoGasto = (double)(fim-inicio)/ CLOCKS_PER_SEC;
                tempo_total += tempoGasto;
                printf("ABP [Busca %02d]: Mat %d -> NAO ENCONTRADA | Tempo: %f s\n", i + 1, matricula_procurada, tempoGasto);

            }
        }
        printf("----------------------------------------");
        printf("TEMPO MEDIO DA BUSCA POR ABP: %f segundos \n", tempo_total / TOTAL_BUSCAS);
        printf("\n ----------------------------------------");
       

    
    printf("--------------------------------------------QUESTÃO 2-----------------\n");
    // Inserindo os elementos no hash
    Hash* indexador= criaHash(INICIALIZAR);
    inserirIndicesNoHash(indexador, TAM, indices);
    int posicoesBuscaExistentes[TOTAL_BUSCAS/2];
    retornarPosicaoVetor(TOTAL_BUSCAS/2, TAM, posicoesBuscaExistentes);
    tempo_total = 0.0;
    double tempos[TOTAL_BUSCAS];

    // Metade das buscas com posições existentes
    for(int i =0; i<TOTAL_BUSCAS/2; i++){
      int matricula_procurada = alunos[posicoesBuscaExistentes[i]]->matricula;
      clock_t inicio = clock();  
      Aluno retornado = buscaPorMatricula(indexador, matricula_procurada , arquivo);
      clock_t fim = clock();
      double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
      tempo_total += tempo_gasto;
      tempos[i] = tempo_gasto;
      printf("Hash (posição existente) [BUSCA %02d]: Mat %d -> %s | Tempo: %f s\n", i+1, matricula_procurada, retornado.nome, tempo_gasto);
    }

    // Outra metade das buscas usando matriculas aleatorias
    for(int i=15; i<TOTAL_BUSCAS; i++){
      int matricula_procurada = geraMatriculaAleatoria();
      clock_t inicio = clock();  
      Aluno retornado = buscaPorMatricula(indexador, matricula_procurada , arquivo);
      clock_t fim = clock();
      double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
      tempo_total += tempo_gasto;
      tempos[i] = tempo_gasto;
      printf("Hash (posição aleatória) [BUSCA %02d]: Mat %d -> %s | Tempo: %f s\n", i+1, matricula_procurada, retornado.nome, tempo_gasto);
    }

    printf("----------------------------------------");
        printf("TEMPO MEDIO DA BUSCA POR Tabela Hash: %f segundos \n", tempo_total / TOTAL_BUSCAS);
    printf("----------------------------------------");

    printf("--------------------------------------------QUESTÃO 3-----------------\n");
        tempo_total = 0.0;

        for(int i = 0; i < TOTAL_BUSCAS; i++){

            int matricula_procurada = alunos[i * (TAM/TOTAL_BUSCAS)] -> matricula;
            int endereco_encontrado;

            //cronometro 
            clock_t inicio = clock();

            endereco_encontrado = buscaSequencialPorMatricula(arquivo, matricula_procurada);
            Aluno aluno_encontrado;
            if(endereco_encontrado > -1){
                fseek(arquivo, endereco_encontrado, SEEK_SET);
                fread(&aluno_encontrado, sizeof(Aluno), 1, arquivo);
            } 
            
            clock_t fim = clock();
            double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
            tempo_total += tempo_gasto;

            if(endereco_encontrado != -1) {
                printf("Busca Sequencial [BUSCA %02d]: Mat %d -> %s | Tempo: %f s\n", i+1, matricula_procurada, aluno_encontrado.nome, tempo_gasto);
            } else {
                printf("Busca Sequencial [BUSCA %02d]: Mat %d -> Não encontrado | Tempo: %f s\n", i+1, matricula_procurada, tempo_gasto);
            }
            
        }
        printf("----------------------------------------");
        printf("TEMPO MEDIO DA BUSCA SEQUENCIAL: %f segundos \n", tempo_total / TOTAL_BUSCAS);
        printf("----------------------------------------");




fclose(arquivo);
}