#include "stdio.h"
#include "stdlib.h"
#include "./dados/dados.h"
#include "./hash/TabelaHash.h"
#define TAM 10000
int main(){


// Criação do vetor e do ponteiro para o arquivo
    Aluno* alunos = gerarVetorAlunosAleatorios(TAM);
    FILE *arquivo;

 // Criação do vetor de indices   
     Indice* indices = gerarEsquemaIndice(alunos, TAM);

// Criação do arquivo binário com 10000 alunos aleatórios
  // só criar um novo arquivo se n houver nenhum outro criado?
    arquivo = fopen("arquivoAlunos.bin", "wb");
    fwrite(alunos, sizeof(Aluno), TAM, arquivo);

// A indexação pode ocorrer da seguinte forma: conforme preenche o arquivo, a matricula é usada como chave e o endereço é atribuido no struct indíce


// printar os indices, para ver se tudo está certo

for(int i =0; i<10; i++){
    printf("%d \n %d \n", indices[i].matricula, indices[i].endereco_inicio);
}




    fclose(arquivo);
}