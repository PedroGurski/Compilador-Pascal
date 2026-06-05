%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
extern FILE *yyin;
extern int yylineno;
void yyerror(const char *s);

NoAST* ast_raiz = NULL;

TipoOp str_to_op(const char* str) {
    if (strcmp(str, "=") == 0) return OP_IGUAL;
    if (strcmp(str, "<>") == 0) return OP_DIF;
    if (strcmp(str, ">") == 0) return OP_MAIOR;
    if (strcmp(str, ">=") == 0) return OP_MAIOR_IGUAL;
    if (strcmp(str, "<") == 0) return OP_MENOR;
    if (strcmp(str, "<=") == 0) return OP_MENOR_IGUAL;
    if (strcmp(str, "*") == 0) return OP_MULT;
    if (strcmp(str, "/") == 0) return OP_DIV_REAL;
    if (strcasecmp(str, "div") == 0) return OP_DIV_INT;
    if (strcasecmp(str, "mod") == 0) return OP_MOD;
    if (strcasecmp(str, "and") == 0) return OP_E;
    return OP_SOMA; // Nao deve acontecer
}

%}

// Faz com que os erros sintaticos sejam bem detalhados
%define parse.error detailed

%union {
    char* str_val;
    struct NoAST* ast_node;
    int int_val; // Para tipos e sinais
}

// Lista de TOKENS da gramatica
%token PROGRAM VAR 
%token INTEIRO REAL
%token FUNCTION PROCEDURE BEGIN_TOKEN END 
%token IF THEN ELSE WHILE DO 
%token <str_val> NUM ID OPERADOR_RELACIONAL OPERADOR_MULTIPLICATIVO 
%token OPERADOR_ATRIBUICAO MAIS MENOS OR
%token ABRE_PARENTESES FECHA_PARENTESES
%token PONTO_FINAL PONTO_VIRGULA VIRGULA DOIS_PONTOS

// Define a associatividade dessas operacoes
%left MAIS MENOS OR
%left OPERADOR_MULTIPLICATIVO

%type <ast_node> PROGRAMA LISTA_DE_IDENTIFICADORES DECLARACOES DECLARACOES_DE_SUBPROGRAMAS
%type <ast_node> DECLARACAO_DE_SUBPROGRAMA CABECALHO_DE_SUBPROGRAMA ARGUMENTOS
%type <ast_node> LISTA_DE_PARAMETROS ENUNCIADO_COMPOSTO ENUNCIADOS_OPCIONAIS
%type <ast_node> LISTA_DE_ENUNCIADOS ENUNCIADO VARIAVEL CHAMADA_DE_PROCEDIMENTO
%type <ast_node> LISTA_DE_EXPRESSOES EXPRESSAO EXPRESSAO_SIMPLES TERMO FATOR
%type <int_val> TIPO SINAL

%%

PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA
        DECLARACOES 
        DECLARACOES_DE_SUBPROGRAMAS 
        ENUNCIADO_COMPOSTO 
        PONTO_FINAL
        {
            $$ = ast_novo_programa($2, $4, $7, $8, $9, yylineno);
            ast_raiz = $$;
            free($2);
        }
        ;

LISTA_DE_IDENTIFICADORES: ID 
                        {
                            $$ = ast_nova_lista(AST_LISTA_ID, yylineno);
                            ast_lista_adicionar($$, ast_novo_item_lista_id($1, yylineno));
                            free($1);
                        }
                        | LISTA_DE_IDENTIFICADORES VIRGULA ID 
                        {
                            $$ = $1;
                            ast_lista_adicionar($$, ast_novo_item_lista_id($3, yylineno));
                            free($3);
                        }
                        ;

DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA 
           {
               if ($1 == NULL) {
                   $$ = ast_nova_lista(AST_LISTA_DECL, yylineno);
               } else {
                   $$ = $1;
               }
               ast_lista_adicionar($$, ast_nova_decl($3, (TipoExp)$5, yylineno));
           }
           | /* empty */ 
           {
               $$ = NULL;
           }
           ;

TIPO: INTEIRO 
    { $$ = TIPO_INTEIRO; }
    | REAL 
    { $$ = TIPO_REAL; }
    ;

DECLARACOES_DE_SUBPROGRAMAS: DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA
                           {
                               if ($1 == NULL) {
                                   $$ = ast_nova_lista(AST_LISTA_DECL, yylineno);
                               } else {
                                   $$ = $1;
                               }
                               ast_lista_adicionar($$, $2);
                           }
                           | /* empty */
                           {
                               $$ = NULL;
                           }
                           ;

DECLARACAO_DE_SUBPROGRAMA: CABECALHO_DE_SUBPROGRAMA DECLARACOES ENUNCIADO_COMPOSTO
                         {
                             $$ = $1; // CABECALHO cria o no
                             $$->decl_func.declaracoes = $2;
                             $$->decl_func.composto = $3;
                         }
                         ;

CABECALHO_DE_SUBPROGRAMA: FUNCTION ID ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA 
                        {
                            $$ = ast_nova_decl_func($2, 1, $3, (TipoExp)$5, NULL, NULL, yylineno);
                            free($2);
                        }
                        | PROCEDURE ID ARGUMENTOS PONTO_VIRGULA 
                        {
                            $$ = ast_nova_decl_func($2, 0, $3, TIPO_VOID, NULL, NULL, yylineno);
                            free($2);
                        }
                        ;

ARGUMENTOS: ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES 
          { $$ = $2; }
          | /* empty */ 
          { $$ = NULL; }
          ;

LISTA_DE_PARAMETROS: LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO 
                   {
                       $$ = ast_nova_lista(AST_LISTA_PARAM, yylineno);
                       ast_lista_adicionar($$, ast_novo_param($1, (TipoExp)$3, 0, yylineno));
                   }
                   | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
                   {
                       $$ = ast_nova_lista(AST_LISTA_PARAM, yylineno);
                       ast_lista_adicionar($$, ast_novo_param($2, (TipoExp)$4, 1, yylineno));
                   }
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
                   {
                       $$ = $1;
                       ast_lista_adicionar($$, ast_novo_param($3, (TipoExp)$5, 0, yylineno));
                   }
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
                   {
                       $$ = $1;
                       ast_lista_adicionar($$, ast_novo_param($4, (TipoExp)$6, 1, yylineno));
                   }
                   ;

ENUNCIADO_COMPOSTO: BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END
                  {
                      $$ = ast_novo_composto($2, yylineno);
                  }
                  ;

ENUNCIADOS_OPCIONAIS: LISTA_DE_ENUNCIADOS
                    { $$ = $1; }
                    | /* empty */
                    { $$ = NULL; }
                    ;

LISTA_DE_ENUNCIADOS: ENUNCIADO
                   {
                       $$ = ast_nova_lista(AST_LISTA_COMANDOS, yylineno);
                       ast_lista_adicionar($$, $1);
                   }
                   | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO
                   {
                       $$ = $1;
                       ast_lista_adicionar($$, $3);
                   }
                   ;

ENUNCIADO: VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO 
         {
             $$ = ast_nova_atribuicao($1->variavel.id, $3, yylineno);
             ast_liberar($1);
         }
         | CHAMADA_DE_PROCEDIMENTO
         { $$ = $1; }
         | ENUNCIADO_COMPOSTO
         { $$ = $1; }
         | IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO
         {
             $$ = ast_novo_se($2, $4, $6, yylineno);
         }
         | WHILE EXPRESSAO DO ENUNCIADO
         {
             $$ = ast_novo_enquanto($2, $4, yylineno);
         }
         ;

VARIAVEL: ID 
        {
            $$ = ast_nova_variavel($1, yylineno);
            free($1);
        }
        ;

CHAMADA_DE_PROCEDIMENTO: ID 
                    {
                        $$ = ast_nova_chamada($1, NULL, yylineno);
                        free($1);
                    }
                    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES 
                    {
                        $$ = ast_nova_chamada($1, $3, yylineno);
                        free($1);
                    }
                    ;

LISTA_DE_EXPRESSOES: EXPRESSAO 
                   {
                       $$ = ast_nova_lista(AST_LISTA_EXP, yylineno);
                       ast_lista_adicionar($$, $1);
                   }
                   | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO  
                   {
                       $$ = $1;
                       ast_lista_adicionar($$, $3);
                   }
                   ;

EXPRESSAO: EXPRESSAO_SIMPLES
         { $$ = $1; }
         | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES   
         {
             TipoOp op = str_to_op($2);
             $$ = ast_nova_op_binaria(op, $1, $3, yylineno);
             free($2);
         }
         ;

EXPRESSAO_SIMPLES: TERMO
                 { $$ = $1; }
                 | SINAL TERMO   
                 {
                     if ($1 == OP_SUBTRACAO) {
                         NoAST* zero = ast_novo_num_int(0, yylineno);
                         $$ = ast_nova_op_binaria(OP_SUBTRACAO, zero, $2, yylineno);
                     } else {
                         $$ = $2;
                     }
                 }
                 | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES 
                 { $$ = ast_nova_op_binaria(OP_SOMA, $1, $3, yylineno); }
                 | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES 
                 { $$ = ast_nova_op_binaria(OP_SUBTRACAO, $1, $3, yylineno); }
                 | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES  
                 { $$ = ast_nova_op_binaria(OP_OU, $1, $3, yylineno); }
                 ;

TERMO: FATOR
     { $$ = $1; }
     | TERMO OPERADOR_MULTIPLICATIVO FATOR  
     {
         TipoOp op = str_to_op($2);
         $$ = ast_nova_op_binaria(op, $1, $3, yylineno);
         free($2);
     }
     ;

FATOR: ID 
     {
         $$ = ast_nova_variavel($1, yylineno);
         free($1);
     }
     | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES  
     {
         $$ = ast_nova_chamada($1, $3, yylineno);
         free($1);
     }
     | NUM 
     {
         if (strchr($1, '.') || strchr($1, 'e') || strchr($1, 'E')) {
             $$ = ast_novo_num_real(atof($1), yylineno);
         } else {
             $$ = ast_novo_num_int(atoi($1), yylineno);
         }
         free($1);
     }
     | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES 
     { $$ = $2; }
     ;

SINAL: MAIS
     { $$ = OP_SOMA; }
     | MENOS
     { $$ = OP_SUBTRACAO; }
     ;

%%

extern void gerador_codigo_gerar(NoAST* raiz);

int main(int argc, char ** argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        yylineno = 1;
        if (!yyparse()) {
            gerador_codigo_gerar(ast_raiz);
        }
        ast_liberar(ast_raiz);
        fclose(yyin);
    } else {
        fprintf(stderr, "Arquivo de entrada nao foi fornecido\n");
    }    
    return 0;
}

void yyerror(const char *s) {
  fprintf(stderr, "Erro na linha %d: %s\n", yylineno,s);
  exit(1);
}
