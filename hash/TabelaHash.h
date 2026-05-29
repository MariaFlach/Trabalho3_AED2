#include "../dados/dados.h"

typedef struct hash Hash;
Hash* criaHash(int tamanho);
void liberaHash(Hash* ha);
int insereHash_Encadeamento(Hash* ha, Indice *dado);
int mostrarColisoes(Hash* ha);
int buscaHash_Encadeamento(Hash* ha, int matricula);
Aluno retornarRegistroEmArquivo(int endereco, FILE *arquivo);
Aluno buscaPorMatricula(Hash* ha, int matricula, FILE* arquivo);
int inserirIndicesNoHash(Hash* ha, int qt_indices, Indice** indices);
