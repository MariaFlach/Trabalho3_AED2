#include <stdlib.h>
#include <string.h>
// Precisamos incluir os protótipos, para estabelcer a comunicação entre a interface e a implementação
#include "TabelaHash.h"

// Definindo o tipo de dados Hash
struct hash {
    int qtd, TABLE_SIZE;
    struct Dado **itens, **overflow;// inicializamos como ponteiro para ponteiro, pois como o tipoDado é um struct e pode ter vários campos, o tamanho do vetor pode acabar ficando muito grande, ao inicializar como ponteiro para ponteiro, poderemos alocar um vetor de ponteiros e apenas o endereço de memória dos elementos ficará armazenado, economizando espaço.
};

// Criando uma Tabela Hash
Hash* criaHash (int TABLE_SIZE){
    Hash* ha = (Hash*) malloc(sizeof(Hash)); // alocar a estrutura hash;
    if(ha){ // se a alocação foi bem sucedida, prosseguimos para as operações
        ha->TABLE_SIZE = TABLE_SIZE; // o atributo TABLE_SIZE da Hash, recebe o parâmetro passado pela função
        ha->itens = (struct Dado**) malloc(TABLE_SIZE * sizeof(struct Dado*)); // alocamos o vetor de ponteiros, o tamanho do vetor de ponteiros em bytes deve ser a quantidade de elementos da tabela vezes o tamanho de cada ponteiro; 
        ha->overflow = (struct Dado**) malloc((TABLE_SIZE/3) * sizeof(struct Dado*))
        if(!ha->itens){ 
            free(ha); // se a alocação do vetor de ponteiros falhou, liberamos a estrutura e retornamos NULL denotando erro
            return NULL;
        }
        // no caso da alocação não falhar
        ha->qtd = 0; // iniciamos o atributo quantidade da tabela Hash comno 0, pois não tem nenhum elemento
        for(int i=0; i< ha->TABLE_SIZE; i++){ // para cada ponteiro iniciado do vetor, como eles não apontam para nenhum lugar ainda, iniciamos todos as posições do vetor como NULL
            ha->itens[i] = NULL;
        }

        for(int i=0; i< (ha->TABLE_SIZE) / 3; i++){ // para cada ponteiro iniciado do vetor, como eles não apontam para nenhum lugar ainda, iniciamos todos as posições do vetor como NULL
            ha->overflow[i] = NULL;
        }
    }

    // se tudo der certo, retornamos o endereço da Tabela Hash criada
    return ha;
}

// Destruindo uma tabela Hash
// após terminarmos de usar a tabela hash, devemos liberar ela para que não fique ocupando espaço desnecessário na memória
    // percorremos a Tabela Hash, sempre que encontramos algum elemento que tenha memória alocada, liberamos esse elemento, ao final, liberamos o vetor e a estrutura hash
void liberaHash(Hash* ha){
    if(ha){
        for(int i=0; i<(ha->TABLE_SIZE - 1); i++){
            if(ha->itens[i]){
                free(ha->itens[i]);
            }
        }
        free(ha->itens);
        free(ha);
    }
}

// função Hashing: método da divisão
    // O método da divisão, para calcular a posição do espalhamento de um elemento dado sua chave, consiste em calcular o resto da divisão do valor inteiro que representa a chave pelo tamanho da tabela 
    // Para evitar colisões, devemos procurar um número primo grande e nunca usar potências de 2 para ser o TABLE_SIZE
int chaveDivisao(int chave, int TABLE_SIZE){
    return chave % TABLE_SIZE;
}

// Inserção em Tabela Hash

// Sem tratar colisões 
int insereHash_SemColisao(Hash* ha, struct Dado dado){
    if(!ha || ha->qtd == ha->TABLE_SIZE) return 0; // não podemos inserir em uma tabela que já está com todas as posições preenchidas ou cuja alocação falhou
    int chave = dado->chave;
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);
    struct Dado* novo = (struct Dado*) malloc(sizeof(struct Dado));
    if(!novo) return 0;
    *novo = dado;
    ha->itens[pos] = novo;
    ha->qtd++;
    return 1;
}

// Tratando as coliões
    // Uma colisão é a ocorrência de duas ou mais chaves na tabela Hash com o mesmo valor de posição
    // Área de overflow: a tabela hash é divida em área normal e área de overflow, a ultima será usada para armazenar as colisões

int insereHash_AreaOverflow(Hash* ha, struct Dado dado){
    if(!ha || ha->qtd == ha->TABLE_SIZE) return 0; // não podemos inserir em uma tabela que já está com todas as posições preenchidas ou cuja alocação falhou
    int chave = dado->chave;
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);
    struct Dado* novo = (struct Dado*) malloc(sizeof(struct Dado));
    if(!novo) return 0;
    *novo = dado;
    if(!ha->itens[pos]){ 
    ha->itens[pos] = novo;
    ha->qtd++;
    return 1;
    }else{
    // inserir na área de overflow
    for(int i=0; i<(ha->TABLE_SIZE/3)-1; i++){
        if(!ha->overflow[i]){
            ha->overflow[i] = novo;
            ha->qtd++;
            return 1;
        }
    }
    }
    return 0;
}

// Busca em tabelas Hash 
    // Sem tratar colisões
int buscaHash_SemColisao(Hash* ha, int chave){
    if(!ha) return 0;
    int pos = chaveDivisao(mat, ha->TABLE_SIZE);
    if(!ha->itens[pos]) return 0;
    return 1;
}

int buscaHash_AreaOverflow(Hash* ha, int chave){
    if(!ha) return 0;
    int pos = chaveDivisao(mat, ha->TABLE_SIZE);
    if(ha->itens[pos]) return 1;
    else{ // (!ha->itens[pos])
        for(int i=0; i<(ha->TABLE_SIZE/3); i++){
            if(ha->overflow[i]->chave == chave){
                return 1;
            }
        }
    }
    return 0;
}

