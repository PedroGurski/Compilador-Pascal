#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Ponteiro para o escopo ativo no momento da compilacao
Escopo* escopo_atual = NULL;


// Inicializa a tabela de simbolos criando o escopo global
void tabela_simb_iniciar() {
    escopo_atual = (Escopo*)calloc(1, sizeof(Escopo));
    escopo_atual->pai = NULL;
    escopo_atual->locais = NULL;
}


// Empilha um novo escopo filho sobre o escopo atual
void tabela_simb_entrar_escopo() {
    Escopo* novo = (Escopo*)calloc(1, sizeof(Escopo));
    novo->pai = escopo_atual;
    novo->locais = NULL;
    escopo_atual = novo;
}


// Desempilha o escopo atual, retornando ao escopo pai
void tabela_simb_sair_escopo() {
    if (escopo_atual->pai) {
        Escopo* velho = escopo_atual;
        escopo_atual = escopo_atual->pai;
        // Na pratica, em um compilador de verdade, a gente liberaria a memoria aqui,
        // mas para fins didaticos vamos apenas desempilhar.
        // NoSimb* atual = velho->locais;
        // enquanto (atual) { ... free ... }
        // free(velho);
    }
}


// Insere uma variavel no escopo atual. Retorna NULL se ja existir
NoSimb* tabela_simb_inserir_var(char* nome, TipoExp tipo, int eh_ref, int eh_global) {
    // Verifica se ja existe no escopo atual
    NoSimb* atual = escopo_atual->locais;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return NULL; // Ja existe
        }
        atual = atual->prox;
    }

    NoSimb* n = (NoSimb*)calloc(1, sizeof(NoSimb));
    n->nome = strdup(nome);
    n->tipo = tipo;
    n->eh_ref = eh_ref;
    n->eh_func = 0;
    n->eh_global = eh_global;
    
    char buf[256];
    if (eh_global) {
        snprintf(buf, sizeof(buf), "@%s", nome);
    } else {
        snprintf(buf, sizeof(buf), "%%_%s", nome);
    }
    n->nome_llvm = strdup(buf);
    
    n->prox = escopo_atual->locais;
    escopo_atual->locais = n;
    
    return n;
}


// Insere uma funcao no escopo atual com sua lista de parametros
NoSimb* tabela_simb_inserir_func(char* nome, TipoExp tipo_retorno, NoSimb* parametros) {
    // Verifica se ja existe no escopo atual
    NoSimb* atual = escopo_atual->locais;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return NULL; // Ja existe
        }
        atual = atual->prox;
    }

    NoSimb* n = (NoSimb*)calloc(1, sizeof(NoSimb));
    n->nome = strdup(nome);
    n->tipo = tipo_retorno;
    n->eh_ref = 0;
    n->eh_func = 1;
    n->parametros = parametros;
    n->eh_global = 1; // Funcoes sao sempre globais no Pascal basico
    
    char buf[256];
    snprintf(buf, sizeof(buf), "@%s", nome);
    n->nome_llvm = strdup(buf);
    
    n->prox = escopo_atual->locais;
    escopo_atual->locais = n;
    
    return n;
}


// Busca um simbolo pelo nome percorrendo os escopos do atual ate o global
NoSimb* tabela_simb_buscar(char* nome) {
    Escopo* s = escopo_atual;
    while (s) {
        NoSimb* atual = s->locais;
        while (atual) {
            if (strcmp(atual->nome, nome) == 0) {
                return atual;
            }
            atual = atual->prox;
        }
        s = s->pai;
    }
    return NULL;
}


// Busca especificamente uma funcao pelo nome na cadeia de escopos
NoSimb* tabela_simb_buscar_func(char* nome) {
    Escopo* s = escopo_atual;
    while (s) {
        NoSimb* atual = s->locais;
        while (atual) {
            if (strcmp(atual->nome, nome) == 0 && atual->eh_func) {
                return atual;
            }
            atual = atual->prox;
        }
        s = s->pai;
    }
    return NULL;
}


// Cria um no de parametro sem inseri-lo na tabela (auxiliar de declaracao)
NoSimb* tabela_simb_criar_param(char* nome, TipoExp tipo, int eh_ref) {
    NoSimb* n = (NoSimb*)calloc(1, sizeof(NoSimb));
    n->nome = strdup(nome);
    n->tipo = tipo;
    n->eh_ref = eh_ref;
    n->eh_func = 0;
    n->eh_global = 0;
    n->prox = NULL;
    return n;
}
