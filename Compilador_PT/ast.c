#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Aloca um novo no da AST.
 * Inicializa tipo, linha e tipo_exp como desconhecido.
 * Retorna ponteiro para o no criado. */
NoAST* ast_novo_no(TipoNoAST tipo, int linha) {
    NoAST* no = (NoAST*)calloc(1, sizeof(NoAST));
    no->tipo = tipo;
    no->linha = linha;
    no->tipo_exp = TIPO_DESCONHECIDO;
    return no;
}


// Cria o no raiz do programa com nome, declaracoes e corpo principal
NoAST* ast_novo_programa(char* nome, NoAST* lista_id, NoAST* declaracoes, NoAST* subprogramas, NoAST* composto, int linha) {
    NoAST* n = ast_novo_no(AST_PROGRAMA, linha);
    n->programa.nome = strdup(nome);
    n->programa.lista_id = lista_id;
    n->programa.declaracoes = declaracoes;
    n->programa.subprogramas = subprogramas;
    n->programa.composto = composto;
    return n;
}


// Cria um no de declaracao de variavel com lista de ids e tipo
NoAST* ast_nova_decl(NoAST* lista_id, TipoExp tipo, int linha) {
    NoAST* n = ast_novo_no(AST_DECLARACAO, linha);
    n->decl.lista_id = lista_id;
    n->decl.tipo_var = tipo;
    return n;
}


// Cria um no de declaracao de funcao ou procedimento
NoAST* ast_nova_decl_func(char* nome, int eh_funcao, NoAST* parametros, TipoExp tipo_retorno, NoAST* declaracoes, NoAST* composto, int linha) {
    NoAST* n = ast_novo_no(AST_DECL_FUNC, linha);
    n->decl_func.nome = strdup(nome);
    n->decl_func.eh_funcao = eh_funcao;
    n->decl_func.parametros = parametros;
    n->decl_func.tipo_retorno = tipo_retorno;
    n->decl_func.declaracoes = declaracoes;
    n->decl_func.composto = composto;
    return n;
}


// Cria um no de parametro formal, com suporte a passagem por referencia
NoAST* ast_novo_param(NoAST* lista_id, TipoExp tipo, int eh_ref, int linha) {
    NoAST* n = ast_novo_no(AST_PARAMETRO, linha);
    n->parametro.lista_id = lista_id;
    n->parametro.tipo_var = tipo;
    n->parametro.eh_ref = eh_ref;
    return n;
}


// Cria um no de bloco composto (begin..end), absorvendo a lista de comandos
NoAST* ast_novo_composto(NoAST* lista_comandos, int linha) {
    NoAST* n = ast_novo_no(AST_COMPOSTO, linha);
    n->lista.itens = lista_comandos ? lista_comandos->lista.itens : NULL;
    n->lista.quantidade = lista_comandos ? lista_comandos->lista.quantidade : 0;
    if (lista_comandos) {
        free(lista_comandos); // Libera apenas o no da lista, nao os filhos
    }
    return n;
}


// Cria um no de atribuicao (variavel := expressao)
NoAST* ast_nova_atribuicao(char* id, NoAST* expr, int linha) {
    NoAST* n = ast_novo_no(AST_ATRIBUICAO, linha);
    n->atribuicao.id = strdup(id);
    n->atribuicao.expr = expr;
    return n;
}


// Cria um no de chamada de funcao ou procedimento com argumentos
NoAST* ast_nova_chamada(char* id, NoAST* argumentos, int linha) {
    NoAST* n = ast_novo_no(AST_CHAMADA, linha);
    n->chamada.id = strdup(id);
    n->chamada.argumentos = argumentos;
    return n;
}


// Cria um no de comando condicional if-then-else
NoAST* ast_novo_se(NoAST* condicao, NoAST* ramo_entao, NoAST* ramo_senao, int linha) {
    NoAST* n = ast_novo_no(AST_SE, linha);
    n->comando_se.condicao = condicao;
    n->comando_se.ramo_entao = ramo_entao;
    n->comando_se.ramo_senao = ramo_senao;
    return n;
}


// Cria um no de laco while com condicao e corpo
NoAST* ast_novo_enquanto(NoAST* condicao, NoAST* corpo, int linha) {
    NoAST* n = ast_novo_no(AST_ENQUANTO, linha);
    n->comando_enquanto.condicao = condicao;
    n->comando_enquanto.corpo = corpo;
    return n;
}


// Cria um no de operacao binaria com operador e dois operandos
NoAST* ast_nova_op_binaria(TipoOp op, NoAST* esq, NoAST* dir, int linha) {
    NoAST* n = ast_novo_no(AST_OP_BINARIA, linha);
    n->op_binaria.op = op;
    n->op_binaria.esq = esq;
    n->op_binaria.dir = dir;
    return n;
}


// Cria um no de referencia a variavel pelo nome
NoAST* ast_nova_variavel(char* id, int linha) {
    NoAST* n = ast_novo_no(AST_VARIAVEL, linha);
    n->variavel.id = strdup(id);
    return n;
}


// Cria um no de literal inteiro com tipo ja definido
NoAST* ast_novo_num_int(int val, int linha) {
    NoAST* n = ast_novo_no(AST_NUM_INT, linha);
    n->num_int = val;
    n->tipo_exp = TIPO_INTEIRO;
    return n;
}


// Cria um no de literal real com tipo ja definido
NoAST* ast_novo_num_real(double val, int linha) {
    NoAST* n = ast_novo_no(AST_NUM_REAL, linha);
    n->num_real = val;
    n->tipo_exp = TIPO_REAL;
    return n;
}


// Cria um no de lista generica (comandos, expressoes, parametros, etc)
NoAST* ast_nova_lista(TipoNoAST tipo_lista, int linha) {
    NoAST* n = ast_novo_no(tipo_lista, linha);
    n->lista.itens = NULL;
    n->lista.quantidade = 0;
    return n;
}


// Adiciona um item ao final de um no de lista, realocando o vetor interno
void ast_lista_adicionar(NoAST* lista, NoAST* item) {
    if (!lista || !item) return;
    lista->lista.quantidade++;
    lista->lista.itens = (NoAST**)realloc(lista->lista.itens, sizeof(NoAST*) * lista->lista.quantidade);
    lista->lista.itens[lista->lista.quantidade - 1] = item;
}


// Cria um no folha para um identificador dentro de uma lista de ids
NoAST* ast_novo_item_lista_id(char* id, int linha) {
    NoAST* n = ast_novo_no(AST_LISTA_ID, linha);
    n->str_id = strdup(id);
    return n;
}

// Libera recursivamente toda a memoria de um no e seus filhos
void ast_liberar(NoAST* no) {
    if (!no) return;
    switch(no->tipo) {
        case AST_PROGRAMA:
            free(no->programa.nome);
            ast_liberar(no->programa.lista_id);
            ast_liberar(no->programa.declaracoes);
            ast_liberar(no->programa.subprogramas);
            ast_liberar(no->programa.composto);
            break;
        case AST_DECLARACAO:
            ast_liberar(no->decl.lista_id);
            break;
        case AST_DECL_FUNC:
            free(no->decl_func.nome);
            ast_liberar(no->decl_func.parametros);
            ast_liberar(no->decl_func.declaracoes);
            ast_liberar(no->decl_func.composto);
            break;
        case AST_PARAMETRO:
            ast_liberar(no->parametro.lista_id);
            break;
        case AST_ATRIBUICAO:
            free(no->atribuicao.id);
            ast_liberar(no->atribuicao.expr);
            break;
        case AST_CHAMADA:
            free(no->chamada.id);
            ast_liberar(no->chamada.argumentos);
            break;
        case AST_SE:
            ast_liberar(no->comando_se.condicao);
            ast_liberar(no->comando_se.ramo_entao);
            ast_liberar(no->comando_se.ramo_senao);
            break;
        case AST_ENQUANTO:
            ast_liberar(no->comando_enquanto.condicao);
            ast_liberar(no->comando_enquanto.corpo);
            break;
        case AST_OP_BINARIA:
            ast_liberar(no->op_binaria.esq);
            ast_liberar(no->op_binaria.dir);
            break;
        case AST_VARIAVEL:
            free(no->variavel.id);
            break;
        case AST_LISTA_ID:
            free(no->str_id);
            break;
        case AST_COMPOSTO:
        case AST_LISTA_COMANDOS:
        case AST_LISTA_EXP:
        case AST_LISTA_PARAM:
        case AST_LISTA_DECL:
            for (int i = 0; i < no->lista.quantidade; i++) {
                ast_liberar(no->lista.itens[i]);
            }
            free(no->lista.itens);
            break;
        default:
            break;
    }
    free(no);
}
