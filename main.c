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

void questao1 (Aluno** alunos, FILE *arquivo, Indice** indices){
    printf("\n========================================================================================\n");
    printf("     Estrategia 1: Busca usando a árvore binária de pesquisa para o atributo chave  \n");
    printf("========================================================================================\n");
    
    ABP arvore;

    ABP_cria(&arvore);

    //inserindo os indices na ABP
    for (int i = 0; i < TAM; i++)
    {
    ABP_insere(&arvore, *indices[i]);
    }

    //preparando para as 30 buscas (15 existentes E 15 ALEATORIAS)
    int matriculasBuscar[TOTAL_BUSCAS];

    int posicoesBuscaRecentes[TOTAL_BUSCAS/2];
    retornarPosicaoVetor(TOTAL_BUSCAS/2, TAM, posicoesBuscaRecentes);


    //Buscando os 15 PRIMEIROS ELEMENTOS EXISTENTES
    for (int i = 0; i < 15; i++)
    {
        matriculasBuscar[i] = alunos[posicoesBuscaRecentes[i]] -> matricula;
    }
       
    // ultimas 15 buscas: ELEMENTOS INEXISTENTES
    for (int i = 15; i < TOTAL_BUSCAS; i++)
    {
        matriculasBuscar[i] = geraMatriculaAleatoria();
    }

    // executando as 30 buscas (15 EXISTENTES E 15 ALEATORIAS)
    printf("\nINICIANDO EXPERIMENTO DAS 30 BUSCAS NA ABP\n\n");
    double tempo_total = 0.0;
    double tempo_gasto;

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| %-2s | %-14s | %-14s | %-11s | %-28s |\n", "BUSCA", "STATUS", "TEMPO (s)", "MATRICULA", "NOME");
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");

    for(int i = 0; i < TOTAL_BUSCAS; i++){
        int matricula_procurada = matriculasBuscar[i];
        clock_t inicio = clock();
        Aluno aluno_encontrado = ABP_retornaAluno(&arvore, matricula_procurada, arquivo);
        clock_t fim = clock();

        tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        tempo_total += tempo_gasto;

        if(aluno_encontrado.matricula != -1){
            printf("|  %02d   | %-14s | %.12f | %11d | %-28.28s |\n",
                i+1, "ENCONTRADO", tempo_gasto, matricula_procurada, aluno_encontrado.nome);
        } else {
            printf("|  %02d   | %-14s | %.12f | %11d | %-28s |\n",
                i+1, "NAO ENCONTRADO", tempo_gasto, matricula_procurada, "---");
        }
    }

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| TEMPO MEDIO: %16.12f s                                                      |\n", tempo_total / TOTAL_BUSCAS);
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
}

void questao2 (Aluno** alunos, FILE *arquivo, Indice** indices){
    printf("\n========================================================================================\n");
    printf("               Estrategia 2: Busca usando hashing para o atributo chave                 \n");
    printf("========================================================================================\n\n");
    
    // Inserindo os elementos no hash
    Hash* indexador = criaHash(INICIALIZAR);
    inserirIndicesNoHash(indexador, TAM, indices);

    int posicoesBuscaExistentes[TOTAL_BUSCAS/2];
    retornarPosicaoVetor(TOTAL_BUSCAS/2, TAM, posicoesBuscaExistentes);

    double tempo_total = 0.0;
    double tempo_gasto;

    printf("INICIANDO EXPERIMENTO DAS 30 BUSCAS NA TABELA HASH\n\n");

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| %-5s | %-14s | %-14s | %-11s | %-28s |\n", "BUSCA", "STATUS", "TEMPO (s)", "MATRICULA", "NOME");
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");

    // Metade das buscas com posições existentes
    for (int i = 0; i < TOTAL_BUSCAS/2; i++) {
        int matricula_procurada = alunos[posicoesBuscaExistentes[i]]->matricula;
        clock_t inicio = clock();
        Aluno retornado = buscaPorMatricula(indexador, matricula_procurada, arquivo);
        clock_t fim = clock();

        tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        tempo_total += tempo_gasto;

        if (retornado.matricula != -1) {
            printf("|  %02d   | %-14s | %.12f | %11d | %-28.28s |\n",
                i+1, "ENCONTRADO", tempo_gasto, matricula_procurada, retornado.nome);
        } else {
            printf("|  %02d   | %-14s | %.12f | %11d | %-28s |\n",
                i+1, "NAO ENCONTRADO", tempo_gasto, matricula_procurada, "---");
        }
    }

    // Outra metade das buscas usando matriculas aleatorias
    for (int i = TOTAL_BUSCAS/2; i < TOTAL_BUSCAS; i++) {
        int matricula_procurada = geraMatriculaAleatoria();
        clock_t inicio = clock();
        Aluno retornado = buscaPorMatricula(indexador, matricula_procurada, arquivo);
        clock_t fim = clock();

        tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        tempo_total += tempo_gasto;

        if (retornado.matricula != -1) {
            printf("|  %02d   | %-14s | %.12f | %11d | %-28.28s |\n",
                i+1, "ENCONTRADO", tempo_gasto, matricula_procurada, retornado.nome);
        } else {
            printf("|  %02d   | %-14s | %.12f | %11d | %-28s |\n",
                i+1, "NAO ENCONTRADO", tempo_gasto, matricula_procurada, "---");
        }
    }

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| TEMPO MEDIO: %16.12f s                                                      |\n", tempo_total / TOTAL_BUSCAS);
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
}

void questao3 (Aluno** alunos, FILE *arquivo, Indice** indices){
    printf("\n========================================================================================\n");
    printf("            Estrategia 3:  Busca sequencial no arquivo para o atributo chave            \n");
    printf("========================================================================================\n\n");
    
    double tempo_total = 0.0;
    double tempo_gasto;

    printf("INICIANDO EXPERIMENTO DAS 30 BUSCAS SEQUENCIAIS\n\n");

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| %-5s | %-14s | %-14s | %-11s | %-28s |\n", "BUSCA", "STATUS", "TEMPO (s)", "MATRICULA", "NOME");
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");

    for(int i = 0; i < TOTAL_BUSCAS; i++){

        int matricula_procurada = alunos[i * (TAM/TOTAL_BUSCAS)]->matricula;
        int endereco_encontrado;
        Aluno aluno_encontrado;

        clock_t inicio = clock();

        endereco_encontrado = buscaSequencialPorMatricula(arquivo, matricula_procurada);
        if(endereco_encontrado > -1){
            fseek(arquivo, endereco_encontrado, SEEK_SET);
            fread(&aluno_encontrado, sizeof(Aluno), 1, arquivo);
        }

        clock_t fim = clock();
        tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        tempo_total += tempo_gasto;

        if(endereco_encontrado != -1){
            printf("|  %02d   | %-14s | %.12f | %11d | %-28.28s |\n",
                i+1, "ENCONTRADO", tempo_gasto, matricula_procurada, aluno_encontrado.nome);
        } else {
            printf("|  %02d   | %-14s | %.12f | %11d | %-28s |\n",
                i+1, "NAO ENCONTRADO", tempo_gasto, matricula_procurada, "---");
        }
    }

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| TEMPO MEDIO: %16.12f s                                                      |\n", tempo_total / TOTAL_BUSCAS);
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
}

void questao4 (Aluno** alunos, FILE *arquivo, Indice** indices){
    printf("\n========================================================================================\n");
    printf("       Estrategia 4: Busca do tipo > usando a ABP para um atributo nao-chave\n");
    printf("========================================================================================\n\n");

    ABP arvore_cr;
    ABP_cria(&arvore_cr);

    for (int i = 0; i < TAM; i++) {
        Indice idx_cr;
        idx_cr.matricula       = (int)(alunos[i]->CR * 100);
        idx_cr.endereco_inicio = i * sizeof(Aluno);
        ABP_insere(&arvore_cr, idx_cr);
    }

    int limiares_cr[TOTAL_BUSCAS];
    for (int i = 0; i < TOTAL_BUSCAS; i++) {
        limiares_cr[i] = (int)(alunos[i * (TAM / TOTAL_BUSCAS)]->CR * 100);
    }

    printf("INICIANDO EXPERIMENTO DAS 30 BUSCAS NA ABP (atributo nao-chave: CR)\n\n");
    double tempo_total = 0.0;
    double tempo_gasto;

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| %-5s | %-14s | %-14s | %-11s | %-28s |\n", "BUSCA", "STATUS", "TEMPO (s)", "LIMIAR CR", "REGISTROS ENCONTRADOS");
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");

    for (int i = 0; i < TOTAL_BUSCAS; i++) {
        int limiar = limiares_cr[i];

        clock_t inicio = clock();

        int count = 0;
        NoABP* pilha[TAM];
        int topo = -1;
        NoABP* atual = arvore_cr.raiz;

        while (atual != NULL || topo >= 0) {
            while (atual != NULL) {
                pilha[++topo] = atual;
                atual = atual->esq;
            }
            atual = pilha[topo--];

            if (atual->dado.matricula > limiar) {
                Aluno al;
                fseek(arquivo, atual->dado.endereco_inicio, SEEK_SET);
                fread(&al, sizeof(Aluno), 1, arquivo);
                count++;
            }
            atual = atual->dir;
        }

        clock_t fim = clock();
        tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        tempo_total += tempo_gasto;

        char registros_str[29];
        snprintf(registros_str, sizeof(registros_str), "%d registros", count);

        printf("|  %02d   | %-14s | %.12f | %8.2f    | %-28s |\n",
            i+1,
            count > 0 ? "ENCONTRADO" : "NAO ENCONTRADO",
            tempo_gasto,
            limiar / 100.0,
            registros_str);
    }

    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
    printf("| TEMPO MEDIO: %16.12f s                                                      |\n", tempo_total / TOTAL_BUSCAS);
    printf("+-------+----------------+----------------+-------------+------------------------------+\n");
}

int main(){
    Aluno** alunos = gerarVetorAlunosAleatorios(TAM);
    FILE *arquivo; 
    Indice** indices = gerarEsquemaIndice(alunos, TAM);
    arquivo = fopen("arquivoAlunos.bin", "w+b");
    for (int i=0; i<TAM; i++){
        fwrite(alunos[i], sizeof(Aluno), 1, arquivo);
    } // pensar se não seria melhor escrever o vetor inteiro logo, mas teria que ver como ficaria a questão do preenchimento dos endereços de memória, como o processo de escrever no arquivo só ocorre uma vez, creio que essa iteração não seja um grande problema

    questao1(alunos, arquivo, indices);
    questao2(alunos, arquivo, indices);
    questao3(alunos, arquivo, indices);
    questao4(alunos, arquivo, indices);
    
    fclose(arquivo);
}