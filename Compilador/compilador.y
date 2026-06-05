%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
extern FILE *yyin;
extern int yylineno;
void yyerror(const char *s);

ASTNode* root_ast = NULL;

OpType str_to_op(const char* str) {
    if (strcmp(str, "=") == 0) return OP_EQ;
    if (strcmp(str, "<>") == 0) return OP_NEQ;
    if (strcmp(str, ">") == 0) return OP_GT;
    if (strcmp(str, ">=") == 0) return OP_GTE;
    if (strcmp(str, "<") == 0) return OP_LT;
    if (strcmp(str, "<=") == 0) return OP_LTE;
    if (strcmp(str, "*") == 0) return OP_MUL;
    if (strcmp(str, "/") == 0) return OP_DIV_REAL;
    if (strcasecmp(str, "div") == 0) return OP_DIV_INT;
    if (strcasecmp(str, "mod") == 0) return OP_MOD;
    if (strcasecmp(str, "and") == 0) return OP_AND;
    return OP_ADD; // Should not happen
}

%}

// Faz com que os erros sintaticos sejam bem detalhados
%define parse.error detailed

%union {
    char* str_val;
    struct ASTNode* ast_node;
    int int_val; // For types and signs
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
            $$ = ast_new_program($2, $4, $7, $8, $9, yylineno);
            root_ast = $$;
            free($2);
        }
        ;

LISTA_DE_IDENTIFICADORES: ID 
                        {
                            $$ = ast_new_list(AST_ID_LIST, yylineno);
                            ast_list_add($$, ast_new_id_list_item($1, yylineno));
                            free($1);
                        }
                        | LISTA_DE_IDENTIFICADORES VIRGULA ID 
                        {
                            $$ = $1;
                            ast_list_add($$, ast_new_id_list_item($3, yylineno));
                            free($3);
                        }
                        ;

DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA 
           {
               if ($1 == NULL) {
                   $$ = ast_new_list(AST_DECL_LIST, yylineno);
               } else {
                   $$ = $1;
               }
               ast_list_add($$, ast_new_decl($3, (ExprType)$5, yylineno));
           }
           | /* empty */ 
           {
               $$ = NULL;
           }
           ;

TIPO: INTEIRO 
    { $$ = TYPE_INTEGER; }
    | REAL 
    { $$ = TYPE_REAL; }
    ;

DECLARACOES_DE_SUBPROGRAMAS: DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA
                           {
                               if ($1 == NULL) {
                                   $$ = ast_new_list(AST_DECL_LIST, yylineno);
                               } else {
                                   $$ = $1;
                               }
                               ast_list_add($$, $2);
                           }
                           | /* empty */
                           {
                               $$ = NULL;
                           }
                           ;

DECLARACAO_DE_SUBPROGRAMA: CABECALHO_DE_SUBPROGRAMA DECLARACOES ENUNCIADO_COMPOSTO
                         {
                             $$ = $1; // CABECALHO creates the func_decl node
                             $$->func_decl.decls = $2;
                             $$->func_decl.compound = $3;
                         }
                         ;

CABECALHO_DE_SUBPROGRAMA: FUNCTION ID ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA 
                        {
                            $$ = ast_new_func_decl($2, 1, $3, (ExprType)$5, NULL, NULL, yylineno);
                            free($2);
                        }
                        | PROCEDURE ID ARGUMENTOS PONTO_VIRGULA 
                        {
                            $$ = ast_new_func_decl($2, 0, $3, TYPE_VOID, NULL, NULL, yylineno);
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
                       $$ = ast_new_list(AST_PARAM_LIST, yylineno);
                       ast_list_add($$, ast_new_param($1, (ExprType)$3, 0, yylineno));
                   }
                   | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
                   {
                       $$ = ast_new_list(AST_PARAM_LIST, yylineno);
                       ast_list_add($$, ast_new_param($2, (ExprType)$4, 1, yylineno));
                   }
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
                   {
                       $$ = $1;
                       ast_list_add($$, ast_new_param($3, (ExprType)$5, 0, yylineno));
                   }
                   | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
                   {
                       $$ = $1;
                       ast_list_add($$, ast_new_param($4, (ExprType)$6, 1, yylineno));
                   }
                   ;

ENUNCIADO_COMPOSTO: BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END
                  {
                      $$ = ast_new_compound($2, yylineno);
                  }
                  ;

ENUNCIADOS_OPCIONAIS: LISTA_DE_ENUNCIADOS
                    { $$ = $1; }
                    | /* empty */
                    { $$ = NULL; }
                    ;

LISTA_DE_ENUNCIADOS: ENUNCIADO
                   {
                       $$ = ast_new_list(AST_STMT_LIST, yylineno);
                       ast_list_add($$, $1);
                   }
                   | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO
                   {
                       $$ = $1;
                       ast_list_add($$, $3);
                   }
                   ;

ENUNCIADO: VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO 
         {
             $$ = ast_new_assign($1->var.id, $3, yylineno);
             ast_free($1);
         }
         | CHAMADA_DE_PROCEDIMENTO
         { $$ = $1; }
         | ENUNCIADO_COMPOSTO
         { $$ = $1; }
         | IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO
         {
             $$ = ast_new_if($2, $4, $6, yylineno);
         }
         | WHILE EXPRESSAO DO ENUNCIADO
         {
             $$ = ast_new_while($2, $4, yylineno);
         }
         ;

VARIAVEL: ID 
        {
            $$ = ast_new_var($1, yylineno);
            free($1);
        }
        ;

CHAMADA_DE_PROCEDIMENTO: ID 
                    {
                        $$ = ast_new_call($1, NULL, yylineno);
                        free($1);
                    }
                    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES 
                    {
                        $$ = ast_new_call($1, $3, yylineno);
                        free($1);
                    }
                    ;

LISTA_DE_EXPRESSOES: EXPRESSAO 
                   {
                       $$ = ast_new_list(AST_EXPR_LIST, yylineno);
                       ast_list_add($$, $1);
                   }
                   | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO  
                   {
                       $$ = $1;
                       ast_list_add($$, $3);
                   }
                   ;

EXPRESSAO: EXPRESSAO_SIMPLES
         { $$ = $1; }
         | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES   
         {
             OpType op = str_to_op($2);
             $$ = ast_new_binop(op, $1, $3, yylineno);
             free($2);
         }
         ;

EXPRESSAO_SIMPLES: TERMO
                 { $$ = $1; }
                 | SINAL TERMO   
                 {
                     if ($1 == OP_SUB) {
                         ASTNode* zero = ast_new_num_int(0, yylineno);
                         $$ = ast_new_binop(OP_SUB, zero, $2, yylineno);
                     } else {
                         $$ = $2;
                     }
                 }
                 | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES 
                 { $$ = ast_new_binop(OP_ADD, $1, $3, yylineno); }
                 | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES 
                 { $$ = ast_new_binop(OP_SUB, $1, $3, yylineno); }
                 | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES  
                 { $$ = ast_new_binop(OP_OR, $1, $3, yylineno); }
                 ;

TERMO: FATOR
     { $$ = $1; }
     | TERMO OPERADOR_MULTIPLICATIVO FATOR  
     {
         OpType op = str_to_op($2);
         $$ = ast_new_binop(op, $1, $3, yylineno);
         free($2);
     }
     ;

FATOR: ID 
     {
         $$ = ast_new_var($1, yylineno);
         free($1);
     }
     | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES  
     {
         $$ = ast_new_call($1, $3, yylineno);
         free($1);
     }
     | NUM 
     {
         if (strchr($1, '.') || strchr($1, 'e') || strchr($1, 'E')) {
             $$ = ast_new_num_real(atof($1), yylineno);
         } else {
             $$ = ast_new_num_int(atoi($1), yylineno);
         }
         free($1);
     }
     | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES 
     { $$ = $2; }
     ;

SINAL: MAIS
     { $$ = OP_ADD; }
     | MENOS
     { $$ = OP_SUB; }
     ;

%%

extern void codegen_generate(ASTNode* root);

int main(int argc, char ** argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        yylineno = 1;
        if (!yyparse()) {
            codegen_generate(root_ast);
        }
        ast_free(root_ast);
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
