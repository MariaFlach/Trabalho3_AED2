typedef struct Aluno {
    char nome[50];
    int CR;
    int matricula;
    char curso[30];
    char pibic;
    char orientador[30];
}Aluno;

typedef struct hash Hash; // o struct hash não deve estar visível para o usuário, apenas o nome do tipo de dados, que é definido no .h
Hash* criaHash(int tamanho);
void liberaHash(Hash* ha);
int insereHash_SemColisao(Hash* ha, Aluno *dado);
int buscaHash_SemColisao(Hash* ha, int chave);
int insereHash_AreaOverflow(Hash* ha, Aluno *dado);
int buscaHash_AreaOverflow(Hash* ha, int chave);