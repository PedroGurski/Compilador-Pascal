#ifndef AST_H
#define AST_H


/* Tipos possiveis para expressoes e variaveis.
 * Usado na analise semantica para verificacao de tipos.
 * TIPO_DESCONHECIDO e resolvido durante a analise. */
typedef enum {
    TIPO_DESCONHECIDO,
    TIPO_INTEIRO,
    TIPO_REAL,
    TIPO_VOID
} TipoExp;


/* Tipos de nos da Arvore Sintatica Abstrata (AST).
 * Cada valor corresponde a uma construcao da linguagem Pascal.
 * O tipo do no determina qual campo da union esta ativo. */
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


/* Operadores binarios suportados pelo compilador.
 * Inclui aritmeticos, logicos e relacionais.
 * A ordem importa: comparacoes sao verificadas por intervalo em codegen. */
typedef enum {
    OP_SOMA, OP_SUBTRACAO, OP_MULT, OP_DIV_INT, OP_DIV_REAL, OP_MOD,
    OP_E, OP_OU,
    OP_IGUAL, OP_DIF, OP_MAIOR, OP_MAIOR_IGUAL, OP_MENOR, OP_MENOR_IGUAL
} TipoOp;


/* No da Arvore Sintatica Abstrata.
 * A union compartilha memoria entre os diferentes tipos de no.
 * Consulte o campo 'tipo' para saber qual membro acessar. */
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


/* Cria o no raiz do programa Pascal
 * Armazena nome, lista de ids externos, declaracoes, subprogramas e corpo
 * Retorna o no AST_PROGRAMA preenchido */
NoAST* ast_novo_programa(char* nome, NoAST* lista_id, NoAST* declaracoes, NoAST* subprogramas, NoAST* composto, int linha);

/* Cria um no de declaracao de variavel
 * Associa uma lista de identificadores a um tipo (inteiro ou real)
 * Retorna o no AST_DECLARACAO preenchido */
NoAST* ast_nova_decl(NoAST* lista_id, TipoExp tipo, int linha);

/* Cria um no de declaracao de funcao ou procedimento
 * O campo eh_funcao diferencia funcao (1) de procedimento (0)
 * Retorna o no AST_DECL_FUNC preenchido */
NoAST* ast_nova_decl_func(char* nome, int eh_funcao, NoAST* parametros, TipoExp tipo_retorno, NoAST* declaracoes, NoAST* composto, int linha);

/* Cria um no de parametro formal de funcao
 * eh_ref indica passagem por referencia (VAR em Pascal)
 * Retorna o no AST_PARAMETRO preenchido */
NoAST* ast_novo_param(NoAST* lista_id, TipoExp tipo, int eh_ref, int linha);

/* Cria um no de bloco composto (begin..end)
 * Absorve os itens da lista de comandos e libera o no intermediario
 * Retorna o no AST_COMPOSTO com os filhos internalizados */
NoAST* ast_novo_composto(NoAST* lista_comandos, int linha);

/* Cria um no de atribuicao (id := expr)
 * Duplica o nome do identificador para posse propria
 * Retorna o no AST_ATRIBUICAO preenchido */
NoAST* ast_nova_atribuicao(char* id, NoAST* expr, int linha);

/* Cria um no de chamada de funcao ou procedimento
 * Duplica o nome do identificador para posse propria
 * Retorna o no AST_CHAMADA preenchido */
NoAST* ast_nova_chamada(char* id, NoAST* argumentos, int linha);

/* Cria um no de comando condicional if-then-else
 * ramo_senao pode ser NULL para comandos sem else
 * Retorna o no AST_SE preenchido */
NoAST* ast_novo_se(NoAST* condicao, NoAST* ramo_entao, NoAST* ramo_senao, int linha);

/* Cria um no de laco while com condicao e corpo
 * Avalia a condicao antes de cada iteracao
 * Retorna o no AST_ENQUANTO preenchido */
NoAST* ast_novo_enquanto(NoAST* condicao, NoAST* corpo, int linha);

/* Cria um no de operacao binaria
 * Armazena operador e os dois sub-nos operandos
 * Retorna o no AST_OP_BINARIA preenchido */
NoAST* ast_nova_op_binaria(TipoOp op, NoAST* esq, NoAST* dir, int linha);

/* Cria um no de referencia a variavel
 * Duplica o nome do identificador para posse propria
 * Retorna o no AST_VARIAVEL preenchido */
NoAST* ast_nova_variavel(char* id, int linha);

/* Cria um no de literal inteiro
 * Define tipo_exp como TIPO_INTEIRO diretamente
 * Retorna o no AST_NUM_INT preenchido */
NoAST* ast_novo_num_int(int val, int linha);

/* Cria um no de literal real
 * Define tipo_exp como TIPO_REAL diretamente
 * Retorna o no AST_NUM_REAL preenchido */
NoAST* ast_novo_num_real(double val, int linha);

// Operacoes em listas

/* Cria um no de lista generica vazia
 * Usado para listas de comandos, expressoes, parametros e declaracoes
 * Retorna o no de lista com vetor interno nulo */
NoAST* ast_nova_lista(TipoNoAST tipo_lista, int linha);

/* Adiciona um item ao final de uma lista de nos
 * Realoca o vetor interno para acomodar o novo elemento
 * Ignora silenciosamente se lista ou item forem nulos */
void ast_lista_adicionar(NoAST* lista, NoAST* item);

/* Cria um no folha representando um identificador em lista de ids
 * Duplica a string para posse propria do no
 * Retorna o no AST_LISTA_ID preenchido */
NoAST* ast_novo_item_lista_id(char* id, int linha);

/* Libera recursivamente um no e todos os seus filhos
 * Percorre a arvore em pos-ordem liberando strings e sub-nos
 * Nao faz nada se o ponteiro for nulo */
void ast_liberar(NoAST* no);

#endif
