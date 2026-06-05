#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"


/* Inicia a geracao de codigo LLVM IR a partir da raiz da AST.
 * Inicializa a tabela de simbolos e escreve o resultado em "a.out".
 * Ponto de entrada principal do backend do compilador. */
void gerador_codigo_gerar(NoAST* raiz);

#endif
