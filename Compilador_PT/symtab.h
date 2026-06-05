#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

typedef struct NoSimb {
    char* nome;
    TipoExp tipo;
    int eh_ref; // 1 se for passagem por referencia (var)
    int eh_func; // 1 se for uma funcao
    struct NoSimb* parametros; // Para funcoes
    int eh_global; // 1 se variavel global
    char* nome_llvm; // Nome alocado no LLVM (ex: %_a, @x)
    struct NoSimb* prox;
} NoSimb;

typedef struct Escopo {
    NoSimb* locais;
    struct Escopo* pai;
} Escopo;

extern Escopo* escopo_atual;

void tabela_simb_iniciar();
void tabela_simb_entrar_escopo();
void tabela_simb_sair_escopo();

NoSimb* tabela_simb_inserir_var(char* nome, TipoExp tipo, int eh_ref, int eh_global);
NoSimb* tabela_simb_inserir_func(char* nome, TipoExp tipo_retorno, NoSimb* parametros);
NoSimb* tabela_simb_buscar(char* nome);
NoSimb* tabela_simb_buscar_func(char* nome);

// Auxiliar para parametros de funcao
NoSimb* tabela_simb_criar_param(char* nome, TipoExp tipo, int eh_ref);

#endif
