#ifndef AST_H
#define AST_H

typedef enum {
    TIPO_DESCONHECIDO,
    TIPO_INTEIRO,
    TIPO_REAL,
    TIPO_VOID
} TipoExp;

typedef enum {
    AST_PROGRAMA,
    AST_DECLARACAO,
    AST_DECL_FUNC,
    AST_PARAMETRO,
    AST_COMPOSTO,
    AST_ATRIBUICAO,
    AST_CHAMADA,
    AST_SE,
    AST_ENQUANTO,
    AST_OP_BINARIA,
    AST_VARIAVEL,
    AST_NUM_INT,
    AST_NUM_REAL,
    AST_LISTA_COMANDOS,
    AST_LISTA_EXP,
    AST_LISTA_PARAM,
    AST_LISTA_DECL,
    AST_LISTA_ID
} TipoNoAST;

typedef enum {
    OP_SOMA, OP_SUBTRACAO, OP_MULT, OP_DIV_INT, OP_DIV_REAL, OP_MOD,
    OP_E, OP_OU,
    OP_IGUAL, OP_DIF, OP_MAIOR, OP_MAIOR_IGUAL, OP_MENOR, OP_MENOR_IGUAL
} TipoOp;

typedef struct NoAST {
    TipoNoAST tipo;
    TipoExp tipo_exp; // Para expressoes e variaveis
    int linha;

    union {
        // AST_PROGRAMA
        struct {
            char* nome;
            struct NoAST* lista_id;
            struct NoAST* declaracoes;
            struct NoAST* subprogramas;
            struct NoAST* composto;
        } programa;

        // AST_DECLARACAO
        struct {
            struct NoAST* lista_id;
            TipoExp tipo_var;
        } decl;

        // AST_DECL_FUNC
        struct {
            char* nome;
            int eh_funcao; // 1 para funcao, 0 para procedimento
            struct NoAST* parametros;
            TipoExp tipo_retorno;
            struct NoAST* declaracoes;
            struct NoAST* composto;
        } decl_func;

        // AST_PARAMETRO
        struct {
            struct NoAST* lista_id;
            TipoExp tipo_var;
            int eh_ref; // 1 para parametro VAR (referencia)
        } parametro;

        // AST_COMPOSTO, AST_LISTA_COMANDOS, AST_LISTA_EXP, AST_LISTA_PARAM, AST_LISTA_DECL, AST_LISTA_ID
        struct {
            struct NoAST** itens;
            int quantidade;
        } lista;

        // AST_ATRIBUICAO
        struct {
            char* id;
            struct NoAST* expr;
        } atribuicao;

        // AST_CHAMADA
        struct {
            char* id;
            struct NoAST* argumentos;
        } chamada;

        // AST_SE
        struct {
            struct NoAST* condicao;
            struct NoAST* ramo_entao;
            struct NoAST* ramo_senao;
        } comando_se;

        // AST_ENQUANTO
        struct {
            struct NoAST* condicao;
            struct NoAST* corpo;
        } comando_enquanto;

        // AST_OP_BINARIA
        struct {
            TipoOp op;
            struct NoAST* esq;
            struct NoAST* dir;
        } op_binaria;

        // AST_VARIAVEL
        struct {
            char* id;
        } variavel;

        // AST_NUM_INT
        int num_int;

        // AST_NUM_REAL
        double num_real;
        
        // String generica para elemento de lista de IDs
        char* str_id;
    };
} NoAST;

NoAST* ast_novo_programa(char* nome, NoAST* lista_id, NoAST* declaracoes, NoAST* subprogramas, NoAST* composto, int linha);
NoAST* ast_nova_decl(NoAST* lista_id, TipoExp tipo, int linha);
NoAST* ast_nova_decl_func(char* nome, int eh_funcao, NoAST* parametros, TipoExp tipo_retorno, NoAST* declaracoes, NoAST* composto, int linha);
NoAST* ast_novo_param(NoAST* lista_id, TipoExp tipo, int eh_ref, int linha);
NoAST* ast_novo_composto(NoAST* lista_comandos, int linha);
NoAST* ast_nova_atribuicao(char* id, NoAST* expr, int linha);
NoAST* ast_nova_chamada(char* id, NoAST* argumentos, int linha);
NoAST* ast_novo_se(NoAST* condicao, NoAST* ramo_entao, NoAST* ramo_senao, int linha);
NoAST* ast_novo_enquanto(NoAST* condicao, NoAST* corpo, int linha);
NoAST* ast_nova_op_binaria(TipoOp op, NoAST* esq, NoAST* dir, int linha);
NoAST* ast_nova_variavel(char* id, int linha);
NoAST* ast_novo_num_int(int val, int linha);
NoAST* ast_novo_num_real(double val, int linha);

// Operacoes em listas
NoAST* ast_nova_lista(TipoNoAST tipo_lista, int linha);
void ast_lista_adicionar(NoAST* lista, NoAST* item);
NoAST* ast_novo_item_lista_id(char* id, int linha);

void ast_liberar(NoAST* no);

#endif
