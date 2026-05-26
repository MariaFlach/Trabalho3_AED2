struct Dado {

};

typedef struct hash Hash; // o struct hash não deve estar visível para o usuário, apenas o nome do tipo de dados, que é definido no .h
Hash* criaHash(int tamanho);
void liberaHash(Hash* ha);
int valorString(char *str);
int insereHash_SemColisao(Hash* ha, int chave, Dado dado);
int buscaHash_SemColisao(Hash* ha, int chave, Dado *dado);
int insereHash_AreaOverlow(Hash* ha, Dado dado);
int buscaHash_AreaOverflow(Hash* ha, int chave, Dado *dado);