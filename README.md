# Trabalho 3 - AEDII

## Tabela Hash

Implementação de uma **Tabela Hash com tratamento de colisões por Área de Overflow**.

A tabela armazena ponteiros para registros do tipo `Aluno`. Ao ocorrer uma colisão (duas matrículas mapeadas para a mesma posição), o elemento excedente é encaminhado para uma área auxiliar de overflow de tamanho fixo igual a `TABLE_SIZE / 3`. A função de espalhamento utilizada é o **método da divisão** (`matricula % TABLE_SIZE`).

---

## Definição do struct

```c
struct hash {
    int qtd;        // Quantidade de elementos atualmente armazenados
    int TABLE_SIZE; // Tamanho da área normal da tabela
    int colisoes;   // Número de colisões detectadas desde a criação
    Aluno **itens;  // Vetor de ponteiros para a área normal  (tamanho: TABLE_SIZE)
    Aluno **overflow; // Vetor de ponteiros para a área de overflow (tamanho: TABLE_SIZE/3)
};
```

> **Por que ponteiro para ponteiro (`Aluno**`)?**
> Armazenar ponteiros em vez dos structs diretamente economiza memória: o vetor mantém apenas endereços (8 bytes cada), e o `Aluno` completo só é alocado quando de fato inserido.

### Layout visual

```
Área normal (TABLE_SIZE posições)       Área de overflow (TABLE_SIZE/3 posições)
┌────┬────┬────┬────┬─────┬────┐        ┌────┬────┐
│  * │NULL│  * │NULL│  *  │NULL│  ···   │  * │NULL│  ···
└──┬─┴────┴──┬─┴────┴──┬──┴────┘        └──┬─┴────┘
   │         │         │                   │
 Aluno     Aluno     Aluno               Aluno
(inserção  (inserção  (inserção          (colisão
 direta)    direta)    direta)           → overflow)
```

---

## Funções

### `criaHash`

```c
Hash* criaHash(int TABLE_SIZE);
```

**Descrição:** Aloca e inicializa uma nova Tabela Hash.

**Parâmetros:**
| Parâmetro    | Tipo  | Descrição                              |
|--------------|-------|----------------------------------------|
| `TABLE_SIZE` | `int` | Número de posições da área normal. Recomenda-se um **número primo** para reduzir colisões. Nunca usar potência de 2. |

**Retorno:**
| Valor    | Significado                              |
|----------|------------------------------------------|
| `Hash*`  | Ponteiro para a tabela criada com sucesso |
| `NULL`   | Falha na alocação de memória             |

**Comportamento interno:**
1. Aloca a estrutura `hash`.
2. Aloca `itens` com `TABLE_SIZE` ponteiros (área normal).
3. Aloca `overflow` com `TABLE_SIZE / 3` ponteiros (área de overflow).
4. Inicializa `qtd = 0`, `colisoes = 0`.
5. Preenche todos os ponteiros de `itens` e `overflow` com `NULL`.

**Complexidade:** O(n), onde n = `TABLE_SIZE`.

---

### `liberaHash`

```c
void liberaHash(Hash* ha);
```

**Descrição:** Percorre a tabela, libera cada `Aluno` alocado, os vetores de ponteiros e, por último, a própria estrutura. Passagem de `NULL` é segura (função retorna sem fazer nada).

**Parâmetros:**
| Parâmetro | Tipo    | Descrição                    |
|-----------|---------|------------------------------|
| `ha`      | `Hash*` | Ponteiro para a tabela Hash  |

**Retorno:** nenhum (`void`).

**Ordem de liberação:**
```
1. free(ha->itens[i])     para cada i em [0, TABLE_SIZE)   se != NULL
2. free(ha->overflow[j])  para cada j em [0, TABLE_SIZE/3)
3. free(ha->itens)
4. free(ha->overflow)
5. free(ha)
```

**Complexidade:** O(n), onde n = `TABLE_SIZE`.

---

### `mostrarColisoes`

```c
int mostrarColisoes(Hash* ha);
```

**Descrição:** Retorna o número de colisões registradas desde a criação da tabela. Uma colisão é contabilizada sempre que uma inserção encontra a posição da área normal já ocupada — **mesmo que a inserção no overflow depois falhe**.

**Parâmetros:**
| Parâmetro | Tipo    | Descrição                    |
|-----------|---------|------------------------------|
| `ha`      | `Hash*` | Ponteiro para a tabela Hash  |

**Retorno:**
| Valor | Significado                          |
|-------|--------------------------------------|
| `int` | Número de colisões detectadas        |
| `0`   | Se `ha == NULL`                      |

---

### `matriculaDivisao`

```c
int matriculaDivisao(int matricula, int TABLE_SIZE);
```

**Descrição:** Função de espalhamento (*hash function*) pelo **método da divisão**: calcula a posição de armazenamento de um elemento a partir de sua matrícula.

**Fórmula:**
```
pos = matricula % TABLE_SIZE
```

**Parâmetros:**
| Parâmetro    | Tipo  | Descrição                       |
|--------------|-------|---------------------------------|
| `matricula`  | `int` | Chave de identificação do aluno |
| `TABLE_SIZE` | `int` | Tamanho da tabela               |

**Retorno:** Índice de posição no vetor `itens`.

### `insereHash_AreaOverflow`

```c
int insereHash_AreaOverflow(Hash* ha, Aluno* dado);
```

**Descrição:** Insere um novo `Aluno` na tabela Hash. Calcula a posição pela matrícula; se a posição estiver livre, insere na área normal; caso contrário, procura a primeira posição livre no overflow. O dado é **copiado** internamente — alterações no ponteiro original não afetam a tabela.

**Parâmetros:**
| Parâmetro | Tipo     | Descrição                           |
|-----------|----------|-------------------------------------|
| `ha`      | `Hash*`  | Ponteiro para a tabela Hash         |
| `dado`    | `Aluno*` | Ponteiro para o aluno a ser inserido |

**Retorno:**
| Valor | Significado                                    |
|-------|------------------------------------------------|
| `1`   | Inserção bem-sucedida                          |
| `0`   | Falha: `ha == NULL`, tabela cheia, `malloc` falhou, ou overflow sem espaço |

**Fluxo de execução:**

```
insereHash_AreaOverflow(ha, dado)
        │
        ├─ ha == NULL ou qtd == TABLE_SIZE? ──► return 0
        │
        ├─ pos = matricula % TABLE_SIZE
        ├─ aloca novo = malloc(Aluno); copia *dado → *novo
        │
        ├─ itens[pos] == NULL?
        │       ├─ SIM ──► itens[pos] = novo; qtd++; return 1
        │       └─ NÃO ──► colisoes++
        │                   └─ percorre overflow[0..TABLE_SIZE/3)
        │                           ├─ achou vaga? ──► overflow[i] = novo; qtd++; return 1
        │                           └─ overflow cheio ──► free(novo); return 0
```

**Complexidade:**
- Caso médio (sem colisão): O(1)
- Pior caso (overflow percorrido inteiro): O(TABLE_SIZE / 3)

---

### `buscaHash_AreaOverflow`

```c
int buscaHash_AreaOverflow(Hash* ha, int matricula);
```

**Descrição:** Busca um `Aluno` pela matrícula. Verifica primeiro a posição calculada na área normal; se a posição estiver vazia ou ocupada por outra matrícula, percorre toda a área de overflow.

**Parâmetros:**
| Parâmetro   | Tipo    | Descrição                           |
|-------------|---------|-------------------------------------|
| `ha`        | `Hash*` | Ponteiro para a tabela Hash         |
| `matricula` | `int`   | Chave de busca                      |

**Retorno:**
| Valor | Significado                        |
|-------|------------------------------------|
| `1`   | Matrícula encontrada na tabela     |
| `0`   | Não encontrada, ou `ha == NULL`    |

**Fluxo de execução:**

```
buscaHash_AreaOverflow(ha, matricula)
        │
        ├─ ha == NULL? ──► return 0
        │
        ├─ pos = matricula % TABLE_SIZE
        │
        ├─ itens[pos] != NULL E itens[pos]->matricula == matricula?
        │       └─ SIM ──► return 1
        │
        └─ percorre overflow[0..TABLE_SIZE/3)
                ├─ overflow[i] != NULL E overflow[i]->matricula == matricula?
                │       └─ SIM ──► return 1
                └─ fim do loop ──► return 0
```

**Complexidade:**
- Melhor caso (encontrado na área normal): O(1)
- Pior caso (percorre todo o overflow): O(TABLE_SIZE / 3)

---