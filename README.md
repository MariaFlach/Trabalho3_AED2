# Trabalho 3 - AEDII

## RODAR O PROJETO

```
gcc main.c ABP/ABP.c dados/dados.c hash/TabelaHash.c lista/Lista.c -o main
```

## Visão geral

Este projeto implementa e compara três estratégias de busca de registros de alunos armazenados em arquivo binário: **Árvore Binária de Pesquisa (ABP)**, **Tabela Hash com encadeamento** e **busca sequencial**. Os dados completos dos alunos ficam gravados em disco; as estruturas em memória (ABP e Hash) mantêm apenas índices — pares (matrícula, posição no arquivo) — para localizar qualquer registro sem varrer o arquivo inteiro.

---

## TAD Aluno

O TAD `Aluno` representa um estudante universitário. É definido em `dados/dados.h` como:

```c
typedef struct Aluno {
    char nome[50];
    float CR;
    int matricula;
    char curso[30];
    int pibic;
    char orientador[30];
} Aluno;
```

Cada campo guarda uma informação do estudante: o nome completo (até 50 caracteres), o coeficiente de rendimento (número real), a matrícula (inteiro único que serve de chave de identificação), o nome do curso, um indicador binário de participação em PIBIC (0 ou 1), e o nome do orientador quando houver bolsa PIBIC, ou a string `"n/a"` caso contrário.

O tamanho fixo de cada campo é fundamental para o sistema de busca em arquivo: como todos os `Aluno`s ocupam exatamente `sizeof(Aluno)` bytes, é possível calcular a posição exata de qualquer registro multiplicando seu índice de ordem pelo tamanho da estrutura.

---

## TAD Indice

O TAD `Indice` é definido em `dados/dados.h` como:

```c
typedef struct Indice {
    int matricula;
    int endereco_inicio;
} Indice;
```

Cada `Indice` funciona como um atalho para um registro no arquivo binário: armazena a matrícula do aluno e o deslocamento em bytes (byte offset) a partir do início do arquivo onde aquele registro começa. O deslocamento é calculado como `i * sizeof(Aluno)`, onde `i` é a posição do aluno no vetor gerado.

O objetivo deste TAD é separar a chave de busca do dado completo. Tanto a ABP quanto a Hash trabalham apenas com `Indice`s — estruturas pequenas — em vez de copiar registros inteiros de `Aluno` para a memória.

---

## TAD Lista

O TAD `Lista` é uma lista encadeada simples definida em `lista/Lista.h`, usada internamente pela tabela hash para tratar colisões por encadeamento.

```c
typedef struct NoLista {
    Indice dado;
    struct NoLista* proximo;
} NoLista;

typedef struct Lista {
    NoLista* cabeca;
    int tamanho;
} Lista;
```

Cada nó armazena um `Indice` e um ponteiro para o próximo nó. A lista não é ordenada; inserções ocorrem sempre no início (`cabeca`).

### Funções do módulo `lista`

- **`criaLista`** — aloca e inicializa uma lista vazia.
- **`liberaLista`** — percorre e libera todos os nós, depois libera a estrutura.
- **`insereLista`** — insere um `Indice` no início da lista.
- **`buscaLista`** — percorre a lista procurando pela matrícula e retorna o `endereco_inicio` correspondente, ou -1 se não encontrado.

---

## TAD ABP

A Árvore Binária de Pesquisa (ABP) é definida em `ABP/ABP.h`. Cada nó armazena um `Indice` e mantém ponteiros para os filhos esquerdo e direito. A chave de ordenação é `matricula`.

```c
typedef struct NoABP {
    Indice dado;
    struct NoABP* esq;
    struct NoABP* dir;
} NoABP;

typedef struct ABP {
    NoABP* raiz;
} ABP;
```

### Funções do módulo `ABP`

- **`ABP_cria`** — inicializa a árvore com raiz `NULL`.
- **`ABP_insere`** — insere um `Indice` na posição correta da árvore comparando matrículas. Inserção iterativa; duplicatas são ignoradas.
- **`ABP_busca`** — percorre a árvore comparando a matrícula buscada. Retorna 1 e preenche `endereco_out` quando encontrado, 0 caso contrário. A busca é O(log n) em árvores balanceadas.
- **`ABP_emOrdem`** — imprime todos os nós em ordem crescente de matrícula (travessia in-order recursiva).
- **`ABP_imprime`** — imprime a árvore com formatação visual de galhos para facilitar inspeção.

---

## TAD Hash

A estrutura interna `hash` é declarada em `hash/TabelaHash.c` e exposta apenas pelo tipo opaco `Hash` no header `TabelaHash.h`, garantindo encapsulamento.

```c
struct hash {
    int qtd, TABLE_SIZE, colisoes;
    Lista **registros;
};
```

- `TABLE_SIZE`: tamanho da tabela (número de listas/buckets).
- `qtd`: quantidade total de elementos inseridos.
- `colisoes`: contador de colisões detectadas durante as inserções.
- `registros`: vetor de ponteiros para `Lista`, um por posição da tabela; cada lista encadeia todos os `Indice`s que colidiram naquela posição.

A função de espalhamento utilizada é uma variante do **hash de Thomas Wang** (manipulação de bits com XOR, shifts e multiplicação), que distribui as chaves de forma mais uniforme do que o simples método da divisão. O tamanho da tabela usado no projeto é **14281**.

O tratamento de colisões é por **encadeamento separado**: cada posição da tabela aponta para uma lista encadeada, e todos os elementos que mapeiam para o mesmo índice são inseridos nessa lista. Não há área de overflow separada.

### Funções do módulo `hash`

- **`criaHash`** — aloca a estrutura, cria uma lista vazia para cada posição e inicializa os contadores.
- **`liberaHash`** — libera todas as listas e depois a estrutura.
- **`mostrarColisoes`** — retorna o valor do contador de colisões.
- **`matriculaHash`** — função de espalhamento interna. Aplica a sequência de operações de bits sobre a matrícula e retorna `resultado % TABLE_SIZE`.
- **`insereHash_Encadeamento`** — calcula a posição pela matrícula e insere o `Indice` na lista daquela posição. Incrementa o contador de colisões se a lista já continha algum elemento.
- **`buscaHash_Encadeamento`** — calcula a posição e delega a busca para `buscaLista`, retornando o `endereco_inicio` ou -1.
- **`retornarRegistroEmArquivo`** — posiciona o cursor no arquivo com `fseek` e lê um `Aluno` com `fread`, retornando a estrutura por valor.
- **`buscaPorMatricula`** — função de alto nível que chama `buscaHash_Encadeamento` para obter o deslocamento e lê o registro em arquivo. Retorna um `Aluno` com campos `"Não encontrado"` se a matrícula não existir na tabela.
- **`inserirIndicesNoHash`** — percorre o vetor de ponteiros para `Indice` e insere cada um via `insereHash_Encadeamento`. Ao final imprime e retorna a quantidade de colisões.

---

## Funções do módulo `dados`

- **`geraString`** — gera uma string aleatória de letras minúsculas com comprimento entre `min` e `max` caracteres.
- **`geraNome`** — gera um nome composto por duas palavras aleatórias com inicial maiúscula, concatenadas com espaço.
- **`geraAlunoAleatorio`** — cria um `Aluno` com todos os campos preenchidos aleatoriamente. A matrícula segue o padrão `225500000 + valor_aleatorio_ate_1000000`.
- **`gerarVetorAlunosAleatorios`** — retorna um vetor de `tam` ponteiros para `Aluno` alocados no heap, com semente inicializada por `srand(time(NULL))`.
- **`gerarEsquemaIndice`** — para cada `aluno[i]`, cria um `Indice` com sua matrícula e com o deslocamento `i * sizeof(Aluno)`.
- **`retornarPosicaoVetor`** — preenche um vetor de saída com posições aleatórias no intervalo `[0, tamVetorEntrada)`, usadas para selecionar alunos existentes nas buscas de benchmark.
- **`geraMatriculaAleatoria`** — gera e retorna uma matrícula aleatória no mesmo intervalo usado por `geraAlunoAleatorio`, podendo ou não corresponder a um aluno existente.

---

## O que acontece na `main`

A `main` executa o fluxo em duas fases: preparação dos dados e experimentos de busca.

**Preparação:**

1. Gera um vetor de 10.000 alunos aleatórios.
2. Gera o vetor de índices correspondente com `gerarEsquemaIndice`.
3. Grava todos os 10.000 registros sequencialmente no arquivo `arquivoAlunos.bin`.

**Questão 1 — Busca por ABP:**

Insere todos os índices na ABP. Realiza 30 buscas cronometradas: as 15 primeiras usam matrículas existentes (extraídas do vetor de alunos em posições espaçadas), e as 15 seguintes usam matrículas aleatórias que podem não existir. Para cada busca encontrada, o registro é lido do arquivo pelo endereço retornado pela ABP. Ao final, exibe o tempo médio das 30 buscas.

**Questão 2 — Busca por Tabela Hash:**

Insere todos os índices na tabela hash (tamanho 14281, encadeamento separado). Realiza 30 buscas cronometradas com a mesma divisão de 15 existentes e 15 aleatórias. Ao final, exibe o tempo médio das 30 buscas.

**Questão 3 — Busca Sequencial:**

Implementada diretamente na `main` pela função `buscaSequencialPorMatricula`, que varre o arquivo binário do início ao fim comparando matrículas. Realiza 30 buscas cronometradas usando matrículas existentes do vetor. Ao final, exibe o tempo médio.

O arquivo é fechado com `fclose` ao final. A memória dos vetores não é explicitamente liberada.

---

## Estrutura de arquivos

- `main.c` — ponto de entrada; orquestra geração de dados, inserção nas estruturas e os três experimentos de busca.
- `dados/dados.h` — declaração dos TADs `Aluno` e `Indice` e das funções do módulo.
- `dados/dados.c` — implementação das funções de geração de dados.
- `lista/Lista.h` — declaração do TAD `Lista` encadeada.
- `lista/Lista.c` — implementação das funções de lista encadeada.
- `ABP/ABP.h` — declaração do TAD `ABP` e suas funções.
- `ABP/ABP.c` — implementação da árvore binária de pesquisa.
- `hash/TabelaHash.h` — interface pública do módulo hash; a estrutura interna não é exposta.
- `hash/TabelaHash.c` — implementação da tabela hash com encadeamento separado por listas.
- `arquivoAlunos.bin` — arquivo binário gerado em tempo de execução com os registros dos alunos.
