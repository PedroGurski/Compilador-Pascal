#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

typedef struct NoSimb {
    char* nome;
    TipoExp tipo;
    int eh_ref;                // 1 se for passagem por referencia (var)
    int eh_func;               // 1 se for uma funcao
    struct NoSimb* parametros; // Para funcoes
    int eh_global;             // 1 se variavel global
    char* nome_llvm;           // Nome alocado no LLVM (ex: %_a, @x)
    struct NoSimb* prox;
} NoSimb;

typedef struct Escopo {
    NoSimb* locais;
    struct Escopo* pai;
} Escopo;

/* Ponteiro para o escopo ativo no momento da compilacao. */
extern Escopo* escopo_atual;


/* Inicializa a tabela de simbolos com um escopo global vazio
 * Deve ser chamada uma unica vez antes da geracao de codigo
 * O escopo criado nao tem pai (raiz da pilha) */
void tabela_simb_iniciar();


/* Cria e empilha um novo escopo filho sobre o escopo atual
 * Usado ao entrar em funcoes, procedimentos ou blocos
 * O escopo filho aponta para o pai e se torna o escopo ativo */
void tabela_simb_entrar_escopo();


/* Desempilha o escopo atual, voltando ao escopo pai
 * Ignora se ja estiver no escopo global (sem pai)
 * Nao libera a memoria do escopo removido (fins didaticos) */
void tabela_simb_sair_escopo();


/* Insere uma variavel no escopo atual com nome LLVM gerado automaticamente
 * Retorna NULL se o nome ja existir no mesmo escopo (redeclaracao)
 * Variaveis globais recebem prefixo @, locais recebem %_*/
NoSimb* tabela_simb_inserir_var(char* nome, TipoExp tipo, int eh_ref, int eh_global);


/* Insere uma funcao no escopo atual com tipo de retorno e parametros
 * Retorna NULL se o nome ja existir (redeclaracao)
 * Funcoes sao sempre tratadas como globais no Pascal basico */
NoSimb* tabela_simb_inserir_func(char* nome, TipoExp tipo_retorno, NoSimb* parametros);


/* Busca um simbolo pelo nome percorrendo escopos do atual ate o global
 * Retorna o primeiro simbolo encontrado (shadowing resolvido corretamente)
 * Retorna NULL se o simbolo nao for encontrado em nenhum escopo */
NoSimb* tabela_simb_buscar(char* nome);


/* Busca especificamente uma funcao pelo nome na cadeia de escopos
 * Ignora variaveis com o mesmo nome (verifica eh_func)
 * Retorna NULL se nenhuma funcao com esse nome for encontrada */
NoSimb* tabela_simb_buscar_func(char* nome);


/* Cria um no de parametro sem inseri-lo em nenhum escopo
 * Usado para montar a lista de parametros antes da insercao da funcao
 * Retorna o no alocado com nome, tipo e flag de referencia */
NoSimb* tabela_simb_criar_param(char* nome, TipoExp tipo, int eh_ref);

#endif
