# Trabalho 3 - AEDII

## Visão geral

Este projeto implementa um sistema de armazenamento e busca de registros de alunos em arquivo binário, utilizando uma tabela hash com esquema de índices para acesso eficiente. A ideia central é que os dados completos dos alunos ficam gravados em disco, e a tabela hash mantém em memória apenas os índices — pares (matrícula, posição no arquivo) — para localizar qualquer registro sem precisar varrer o arquivo inteiro.

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

O objetivo deste TAD é separar a chave de busca do dado completo. A tabela hash trabalha apenas com `Indice`s — estruturas pequenas — em vez de copiar registros inteiros de `Aluno` para a memória. Quando a busca encontra o `Indice`, usa o `endereco_inicio` para ir direto à posição correta no arquivo.

---

## TAD Hash

A estrutura interna `hash` é declarada em `hash/TabelaHash.c` e exposta apenas pelo tipo opaco `Hash` no header `TabelaHash.h`. Isso garante encapsulamento: o código externo não acessa os campos diretamente, apenas por meio das funções fornecidas.

A estrutura possui:

- `TABLE_SIZE`: tamanho da área principal da tabela.
- `qtd`: quantidade total de elementos inseridos (área normal mais overflow).
- `colisoes`: contador de colisões detectadas durante as inserções.
- `registros`: vetor de ponteiros para `Indice` com `TABLE_SIZE` posições, que é a área principal da tabela.
- `overflow`: vetor de ponteiros para `Indice` com `TABLE_SIZE / 3` posições, que é a área auxiliar usada quando ocorre colisão na área principal.

A função de espalhamento utilizada é o **método da divisão**: `matricula % TABLE_SIZE`. A colisão ocorre quando duas matrículas diferentes produzem o mesmo resultado nessa operação. O tratamento adotado é **área de overflow separada**: o elemento que não cabe na posição calculada vai para a primeira posição livre do vetor `overflow`. Recomenda-se usar um número primo como `TABLE_SIZE` para distribuir melhor as chaves e reduzir colisões; o projeto usa 100003.

---

## Funções do módulo `dados`

### `geraString`

Gera uma string aleatória de letras minúsculas com comprimento entre `min` e `max` caracteres, inserindo o terminador nulo ao final. É usada internamente para preencher campos como curso e partes de nomes.

### `geraNome`

Gera um nome composto por duas palavras aleatórias, cada uma com entre 4 e 10 caracteres. A primeira letra de cada palavra é convertida para maiúscula subtraindo 32 do código ASCII. As duas palavras são concatenadas com espaço e o resultado é copiado para o destino informado.

### `geraAlunoAleatorio`

Cria e retorna um `Aluno` com todos os campos preenchidos aleatoriamente. O nome é gerado por `geraNome`. O CR é um float no intervalo [4,0; 10,0] com uma casa decimal. A matrícula segue o padrão `225500000 + valor_aleatorio_ate_1000000`, combinando dois números aleatórios para atingir o intervalo necessário. O curso é uma string aleatória com inicial maiúscula. O campo `pibic` é 0 ou 1 com probabilidade igual; se for 1, o orientador recebe um nome fictício precedido de "Prof. ", senão recebe "n/a".

### `gerarVetorAlunosAleatorios`

Recebe um tamanho `tam` e retorna um vetor de `tam` ponteiros para `Aluno`, cada um alocado individualmente no heap e preenchido com dados aleatórios via `geraAlunoAleatorio`. A semente do gerador aleatório é inicializada com o tempo atual (`srand(time(NULL))`), garantindo resultados diferentes a cada execução.

### `gerarEsquemaIndice`

Recebe o vetor de ponteiros para `Aluno` e seu tamanho, e retorna um vetor de ponteiros para `Indice` com a mesma quantidade de elementos. Para cada posição `i`, cria um `Indice` com a matrícula do `aluno[i]` e com o deslocamento `i * sizeof(Aluno)`, que é exatamente onde esse aluno será gravado no arquivo binário.

---

## Funções do módulo `hash`

### `criaHash`

Aloca e inicializa uma nova tabela hash. Recebe o tamanho desejado para a área principal, aloca os dois vetores de ponteiros (`registros` e `overflow`), inicializa todos os ponteiros com `NULL` e os contadores com zero. Retorna o ponteiro para a estrutura criada, ou `NULL` em caso de falha na alocação.

### `liberaHash`

Libera toda a memória associada à tabela hash. Percorre a área principal e a área de overflow liberando cada `Indice` alocado, depois libera os próprios vetores e por fim a estrutura `Hash`. É segura para receber `NULL`.

### `mostrarColisoes`

Retorna o valor do campo `colisoes` da estrutura. Retorna 0 se o ponteiro for `NULL`.

### `matriculaDivisao`

Função de espalhamento interna. Calcula `matricula % TABLE_SIZE` e retorna o índice na área principal onde o elemento deve ser armazenado ou buscado.

### `insereHash_AreaOverflow`

Insere um `Indice` na tabela hash. Calcula a posição pela matrícula; se a posição na área principal estiver livre, armazena o `Indice` ali; caso contrário, incrementa o contador de colisões e procura a primeira posição livre na área de overflow. O dado é copiado internamente (nova alocação), portanto o ponteiro original pode ser liberado após a inserção. Retorna 1 em caso de sucesso e 0 em caso de falha (ponteiro nulo, tabela cheia, malloc falhou ou overflow sem espaço).

### `buscaHash_AreaOverflow`

Busca um `Indice` pela matrícula e retorna o `endereco_inicio` armazenado nele, ou -1 se não for encontrado. Primeiro verifica a posição calculada na área principal; se a posição estiver vazia ou ocupada por outra matrícula, percorre toda a área de overflow procurando uma entrada com a matrícula desejada.

### `retornarRegistroEmArquivo`

Recebe um deslocamento em bytes e um ponteiro para arquivo já aberto, posiciona o cursor com `fseek` e lê exatamente um `Aluno` com `fread`, retornando a estrutura por valor.

### `buscaPorMatricula`

Função de alto nível que combina a busca no hash e a leitura em arquivo. Recebe a tabela hash, uma matrícula e o arquivo aberto. Chama `buscaHash_AreaOverflow` para obter o deslocamento, depois posiciona o cursor e lê o registro correspondente, imprimindo todos os campos do aluno encontrado e retornando a estrutura.

### `inserirIndicesNoHash`

Percorre um vetor de ponteiros para `Indice` e insere cada elemento na tabela hash via `insereHash_AreaOverflow`. Ao final, imprime e retorna a quantidade total de colisões registradas.

---

## O que acontece na `main`

A função `main` executa o fluxo completo do sistema em seis etapas:

**1. Geração dos dados.** É criado um vetor de 10.000 alunos aleatórios com `gerarVetorAlunosAleatorios`. Cada aluno recebe uma matrícula única gerada aleatoriamente no intervalo esperado.

**2. Geração do esquema de índices.** A função `gerarEsquemaIndice` percorre o vetor de alunos e cria para cada um um `Indice` com sua matrícula e com o deslocamento exato onde será gravado no arquivo binário. Neste ponto os dados ainda estão somente em memória.

**3. Gravação no arquivo binário.** O arquivo `arquivoAlunos.bin` é aberto em modo leitura e escrita binária. Os 10.000 registros são escritos sequencialmente com `fwrite`, um de cada vez, na mesma ordem do vetor. Após isso, o arquivo contém os dados de todos os alunos em posições fixas e previsíveis.

**4. Criação da tabela hash.** É instanciada uma tabela hash com 100.003 posições (número primo escolhido para minimizar colisões). O tamanho é aproximadamente 10 vezes maior do que a quantidade de registros, o que reduz drasticamente as colisões pelo método da divisão.

**5. Inserção dos índices no hash.** `inserirIndicesNoHash` percorre o vetor de índices e insere cada par (matrícula, deslocamento) na tabela hash. Ao final, exibe a quantidade de colisões que ocorreram.

**6. Busca por matrícula.** A função `buscaPorMatricula` é chamada com a matrícula do décimo aluno do vetor (`alunos[10]->matricula`). Internamente, ela consulta o hash para obter o deslocamento no arquivo, vai direto àquela posição com `fseek` e lê o registro com `fread`, exibindo todos os dados do aluno encontrado.

O arquivo é fechado com `fclose` ao final. A memória dos vetores de alunos e índices não é explicitamente liberada nessa versão do código.

---

## Estrutura de arquivos

- `main.c` — ponto de entrada do programa, orquestra todo o fluxo.
- `dados/dados.h` — declaração dos TADs `Aluno` e `Indice` e das funções de geração de dados.
- `dados/dados.c` — implementação das funções de geração de dados.
- `hash/TabelaHash.h` — interface pública do módulo hash; a estrutura interna não é exposta.
- `hash/TabelaHash.c` — implementação completa da tabela hash com área de overflow.
- `arquivoAlunos.bin` — arquivo binário gerado em tempo de execução com os registros dos alunos.
