#include "../dados/dados.h"

typedef struct hash Hash; // o struct hash não deve estar visível para o usuário, apenas o nome do tipo de dados, que é definido no .h
Hash* criaHash(int tamanho);
void liberaHash(Hash* ha);
int insereHash_AreaOverflow(Hash* ha, Aluno *dado);
int mostrarColisoes(Hash* ha);
int buscaHash_AreaOverflow(Hash* ha, int chave);