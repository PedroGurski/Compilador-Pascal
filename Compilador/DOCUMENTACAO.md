# Documentação do Compilador Pascal para LLVM IR

Este documento descreve detalhadamente o funcionamento, a arquitetura e os arquivos que compõem o compilador desenvolvido. O objetivo do projeto é traduzir um subconjunto da linguagem Pascal para o formato intermediário LLVM IR (Intermediate Representation), utilizando as ferramentas Flex e Bison, além da linguagem C.

---

## 1. Arquitetura e Fluxo de Execução

O compilador opera em fases clássicas, realizando a leitura do código-fonte em Pascal, validando-o e gerando o código de destino:

1.  **Análise Léxica (Flex)**: Lê o arquivo fonte `.pas` e converte a sequência de caracteres em "tokens" (palavras reservadas, identificadores, números, operadores).
2.  **Análise Sintática e Construção da AST (Bison)**: Valida a sequência de tokens com base nas regras gramaticais da linguagem. Se a sintaxe for válida, o Bison constrói simultaneamente uma Árvore Sintática Abstrata (AST - *Abstract Syntax Tree*), que é uma representação em memória da estrutura do programa.
3.  **Análise Semântica e Tabela de Símbolos**: Através de uma estrutura de dados de escopos (a Tabela de Símbolos), o compilador garante que variáveis e funções não sejam redeclaradas, que funções sejam chamadas com o número correto de argumentos e que os tipos das variáveis estejam compatíveis.
4.  **Geração de Código (LLVM IR)**: A partir da AST gerada, o compilador faz um passeio (traversal) percorrendo os nós da árvore e escreve as instruções LLVM correspondentes no arquivo `a.out`.

---

## 2. Estrutura dos Arquivos

### `compilador.l` (Analisador Léxico)
- **Função**: Define expressões regulares para reconhecer o vocabulário da linguagem.
- **Lógica Principal**:
  - Pula espaços em branco, quebras de linha e tabulações.
  - Reconhece palavras-chave do Pascal (`program`, `var`, `integer`, `real`, `begin`, `end`, `if`, `while`, etc.) e emite os tokens definidos pelo Bison.
  - **Identificadores (Variáveis/Funções)**: O Pascal não faz distinção entre maiúsculas e minúsculas (*case-insensitive*). Para suportar isso, ao ler qualquer caractere alfanumérico que forme um identificador (`ID`), iteramos sobre a string (`yytext`) e convertemos todas as letras para minúsculo (`tolower()`) antes de armazená-lo. 
  - **Números**: Captura as sequências numéricas (`NUM`) inteiras e em ponto flutuante (inclusive notação científica, e.g. `1.5E+2`).

### `compilador.y` (Analisador Sintático)
- **Função**: Define a gramática do Pascal e direciona a criação da Árvore Sintática Abstrata (AST).
- **Lógica Principal**:
  - Contém as regras gramaticais no formato de regras de produção.
  - Possui diretivas `%union` que vinculam os *tokens* lidos a estruturas em C (como inteiros, strings e ponteiros de nós da AST).
  - Cada produção válida chama uma função de criação de nó correspondente (`ast_new_...`). No final do processamento (`PROGRAMA`), a variável global `root_ast` aponta para a raiz da árvore do código completo.
  - O método `main` está implementado aqui, onde recebe o nome do arquivo, direciona o ponteiro `yyin`, aciona o *parser* (`yyparse`) e, se houver sucesso sintático, dispara o processo de geração do LLVM (`codegen_generate`).

### `ast.h` e `ast.c` (Árvore Sintática Abstrata)
- **Função**: Estruturação dos dados vindos do parser em formato de árvore para avaliação semântica posterior.
- **Lógica Principal**:
  - Utiliza uma grande estrutura base `ASTNode`, acompanhada de uma `union` que guarda dados específicos para diferentes partes do programa (por exemplo: um nó de "Soma" guarda os nós Esquerdo e Direito, enquanto um nó de "Condição `if`" guarda a Expressão Teste, o bloco do `then` e o bloco do `else`).
  - Funções auxiliares organizam a construção e também a liberação de memória recursiva ao fim do programa (`ast_free`), evitando vazamentos (memory leaks).

### `symtab.h` e `symtab.c` (Tabela de Símbolos)
- **Função**: Controlar as variáveis, parâmetros, tipos e manter a validação das regras de visibilidade (escopo).
- **Lógica Principal**:
  - É implementada como uma Lista Encadeada de "Escopos" apontando de cima para baixo (`parent`). Quando o programa global se inicia, criamos um escopo. Quando entramos dentro de uma Função, acionamos a função `symtab_enter_scope()`, criando um "novo nível" isolado.
  - **Funções (`symtab_insert_var` e `symtab_insert_func`)**: Cadastram novas entidades. Elas validam imediatamente se o nome já não existe no escopo atual, retornando `NULL` e ajudando a barrar redeclarações irregulares.
  - **Busca**: A função `symtab_lookup(nome)` varre a tabela do escopo atual em direção ao pai até achar o símbolo de mesmo nome. Já a função `symtab_lookup_func(nome)` garante que o que for devolvido seja, de fato, do tipo função, contornando a regra do Pascal em que o valor de retorno da função leva o próprio nome dela (o que no LLVM foi resolvido gerando uma variável local implícita de mesmo nome).

### `codegen.h` e `codegen.c` (Geração de Código)
- **Função**: Conversão definitiva da AST validada e da Tabela de Símbolos em instruções seriais da representação LLVM IR.
- **Lógica Principal**:
  - Toda a saída ocorre de maneira manual usando `fprintf` direto no arquivo `a.out`.
  - **Declaração de Biblioteca (`read` e `write`)**: O preâmbulo injeta imediatamente as assinaturas de `@printf` e `@__isoc99_scanf` do C padrão, bem como suas máscaras lógicas `"\%d"` (inteiro) e `"\%f"` (ponto flutuante).
  - **Alocação de Memória (`alloca`)**: Assim que um escopo de função inicia, a leitura percorre as variáveis locais declaradas e adiciona para cada uma a instrução de `alloca` (alocação de registro temporário no topo da pilha de execução), seguida da sua inicialização em 0 ou 0.0 (float).
  - **Resolução Opaque Pointers**: Compatível com LLVM moderno, não há tipos castados de ponteiros explícitos nas declarações, o tipo é simplificado para instrução `ptr`.
  - **Conflito Numérico (Inteiro / Real)**: Sempre que há uma mistura numa expressão binária, injetamos a instrução de Cast (`sitofp i32 to float`) elevando o termo menor.
  - **Saltos e Condições**: Os Comandos `IF` e `WHILE` são tratados pela emissão prévia de Rótulos (Labels, como `%L1` ou `%L2`) através de um gerador sequencial (`label_counter`). Comandos `br i1` (branch de 1 bit, pois LLVM requer Boolean) são ativados com a checagem da condição, bifurcando a execução do bloco de blocos semânticos.
  - **Recursão**: É permitida e completamente tratada, já que variáveis locais não sobrescrevem variáveis de camadas mais altas, e a busca por função na tabela desvia a chamada corretamente.

### `Makefile`
- Orquestra os comandos. Roda o Bison gerando o cabeçalho tabulado (`bison -d`), roda o Flex (`flex`) para parear com a gramática e, por fim, chama o compilador nativo do sistema (`gcc`) para compilar todos os artefatos C e lincá-los em um único binário (`compilador`).

### `teste.bash`
- Script externo adicional de validação em lote, que varre todos os exemplos da linguagem Pascal e compila, empacotando num único diretório relatórios visuais de como cada programa de teste se comportou.

---

## 3. Na Prática: Como as Engrenagens Giram

Quando você executa:
`./compilador programa.pas`

O processo de baixo nível funciona na seguinte ordem:
1. O texto entra no Flex. Se ele encontrar `a := b + 1;`, o Flex cospe 5 tokens: `ID(a)`, `OPERADOR_ATRIBUICAO`, `ID(b)`, `MAIS`, `NUM_INT(1)`.
2. O Bison captura essa sequência e monta na memória:
   ```text
        [ATRIBUIÇÃO (=)]
        /             \
    [VAR: a]         [SOMA (+)]
                    /          \
                [VAR: b]      [NUM: 1]
   ```
3. O gerador de código recebe essa árvore e passeia de baixo para cima:
   - "Quero somar! Preciso do valor de `b`". (Gera: `%1 = load i32, ptr %_b`)
   - "Preciso do número constante 1". (Gera `%2 = add i32 0, 1`)
   - "Faça a soma!". (Gera `%3 = add nsw i32 %1, %2`)
   - "Guarde no `a`". (Gera `store i32 %3, ptr %_a`)
4. Em nenhum momento lidamos com registradores fixos, criando uma infinidade virtual com numeração `%1`, `%2` usando nosso contador `reg_counter++`. O próprio compilador otimizador embutido no LLVM nativo é quem cuidará, mais tarde, de colocar as coisas na memória RAM ou nos registradores reais da CPU (`eax`, `ebx`...).
5. Se uma variável não fosse encontrada (ex: tentou usar `c`), a Tabela de Símbolos (`symtab_lookup`) rejeitaria a requisição do gerador de código e o programa dispararia `exit(1)` matando toda a compilação, o que cumpre a regra essencial: nunca deixe passar erros.
