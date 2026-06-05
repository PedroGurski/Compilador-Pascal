# Trabalho - CI1211

Construir um compilador para a representação intermediária do LLVM ([LLVM-IR](https://llvm.org/docs/LangRef.html), ou LLVM Assembly Language).

A linguagem de programação poderá ser definida pelo usuário, desde que tenha um nível mínimo de dificuldade. O nível está definido abaixo, mas as propostas devem ser avaliadas pelo professor.

Caso o aluno não queira desenvolver sua própria linguagem, um subconjunto da linguagem Pascal deverá ser utilizado. Os detalhes também estão abaixo.

## Regras e requisitos - **IMPORTANTE**

- O trabalho pode ser feito em grupos de dois alunos. O trabalho deve ser apresentado pelo grupo. Não apresentar implica em nota zero.
    - O aluno que não souber responder as perguntas, ou explicar o código inteiro, poderá receber uma nota inferior que o colega.

- Os critérios para a nota serão:
    - Funcionamento correto do código para todos os testes realizados pelo professor (20%).
    - Compreensão e domínio do código produzido (40%)
    - Domínio geral do tema, incluindo capacidade de implementar mudanças/melhorias propostas pelo professor, como implementar outras features na linguagem (40%)

- Não deixe para submeter (fazer push) apenas no último momento. Faça o trabalho e submeta de forma incremental, já usando o repositório criado pelo GitHub Classroom. Dessa forma, submissões parciais poderão receber alguma nota. **Quando o prazo encerrar, não serão aceitas novas submissões (push) mesmo com redução da nota!**

- Arquivos **plagiados, copiados**, serão zerados. Não importa quem fez e quem copiou, ambos serão zerados. Portanto, não deixe o seu trabalho disponível de forma pública.
    - Plágio com trabalhos de anos anteriores também receberão nota zero

- Utilize a versão 15 ou superior do LLVM, onde [*Opaque Pointer*](https://llvm.org/docs/OpaquePointers.html) são suportados por padrão.

- Não é permitido o uso da [biblioteca LLVM](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html) para gerar código LLVM-IR, ou qualquer outra biblioteca para isso. Portanto, é necessário gerar o código de forma manual.
    - Não utilize bibliotecas extras, além das bibliotecas padrões de C, como Boost, pois podem complicar a compilação e validação pelo professor. Qualquer estrutura de dados, como a utilizada para tabela de símbolos, pode ser facilmente implementada pelo aluno (listas ligadas, tabelas hash e etc.).

- O compilador deve ler um arquivo de entrada (um único arquivo), que será o arquivo com o código na linguagem de programação escolhida (Pascal ou definida pelo grupo). Esse arquivo será passado ao compilador como argumento na linha de comando (o único argumento)

- Um arquivo de saída, com as instruções LLVM-IR, será criado no diretório de trabalho atual. Esse arquivo sempre será nomeado `a.out`.

- O compilador deve retornar um valor diferente de zero quando encontrar qualquer tipo de erro. E retornar zero apenas em uma compilação bem sucedida.

- Recursão e passagem por referência são permitidas. 

- As funções `read` e `write` (ou equivalentes) devem ser providas pelo compilador, e podem ser substituídas por uma chamada ao scanf e printf. Não é necessário ler e imprimir caracteres na tela, trabalhe apenas com números inteiros e de ponto flutuante. Mais detalhes em como usar/chamar essas funções pode ser obtido [aqui](./read_write_llvm.md).

## Ferramentas

O compilador deve, obrigatoriamente, usar as ferramentas [Flex](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/) e [Bison](https://www.gnu.org/software/bison/manual/html_node/). 

Um arquivo inicial para o Flex [compilador.l](./compilador.l) e Bison [compilador.y](./compilador.y) já estão incluídos neste repositório. 

Um arquivo Makefile também foi incluído, e pode ser modificado de acordo com a organização do seu projeto. Porém, o arquivo executável não pode ser alterado (chamado `compilador`
).


## Pascal

A especificação do subconjunto Pascal está em [Pascal.md](./Pascal.md).

## Linguagem Própria

Caso queira criar uma linguagem de programação específica, ela deve seguir os critérios abaixo:

- Trabalhar com pelo menos dois tipos numéricos primitivos (Inteiro e Ponto Flutuante).
    - Não sendo obrigatório tipos agregados (como array).
    - Detalhar como cast será feito.

- Conter instruções para execução seletiva (como IF-ELSE).

- Permitir a definição de funções recursivas.

- Não precisa trabalhar com ponteiros de forma explícita, mas deve permitir a passagem por referência nas funções.

- Não precisa ser imperativa, pode criar uma linguagem usando outro paradigma.

- Uma gramática, sem ações sintáticas, deve ser entregue na submissão.
    - Bem como a lista de tokens para o analisador léxico.

- Vários exemplos de programas, que trabalham com cada ponto da linguagem, deve ser entregue. Por exemplo, programas para:
    - testar expressões simples
    - testar o cast entre tipos
    - testar funções recursivas
    - ...

