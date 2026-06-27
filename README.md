# Compilador Pascal

Compilador para a linguagem Pascal desenvolvido em C com Lex e Yacc, gerando código LLVM IR.

## Sobre o Projeto

Este projeto implementa um compilador completo para um subconjunto da linguagem Pascal, desenvolvido como trabalho acadêmico da disciplina de Compiladores. O compilador passa pelas etapas clássicas de compilação: análise léxica, análise sintática, análise semântica e geração de código.

## Tecnologias Utilizadas

- **C** — implementação principal do compilador
- **Lex** — análise léxica (tokenização)
- **Yacc** — análise sintática e semântica
- **LLVM IR** — geração de código intermediário
- **Makefile / Shell** — automação do build

## Como Compilar e Executar

### Pré-requisitos

- GCC
- Flex (Lex)
- Bison (Yacc)
- LLVM

### Build

```bash
cd Compilador
make
```

### Uso

```bash
./compilador arquivo.pas
```

## Fases do Compilador

1. **Análise Léxica** — reconhecimento de tokens via Lex
2. **Análise Sintática** — validação da gramática via Yacc
3. **Análise Semântica** — verificação de tipos e escopos
4. **Geração de Código** — saída em LLVM IR
