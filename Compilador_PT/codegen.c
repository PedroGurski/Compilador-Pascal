#include "codegen.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* saida;
static int contador_reg = 1;
static int contador_label = 1;

static void gerar_no(NoAST* no);
static char* gerar_exp(NoAST* no, TipoExp* tipo_saida);
static void gerar_comando(NoAST* no);


/* Aloca e retorna um novo registrador LLVM temporario
 * Incrementa o contador global de registradores a cada chamada
 * O chamador e responsavel por liberar a string retornada */
static char* obter_novo_reg() {
    char* buf = (char*)malloc(32);
    sprintf(buf, "%%%d", contador_reg++);
    return buf;
}


/* Aloca e retorna um novo nome de label LLVM
 * Incrementa o contador global de labels a cada chamada
 * O chamador e responsavel por liberar a string retornada */
static char* obter_nova_label() {
    char* buf = (char*)malloc(32);
    sprintf(buf, "L%d", contador_label++);
    return buf;
}


/* Retorna a string de tipo LLVM correspondente ao TipoExp
 * Mapeia TIPO_INTEIRO -> i32, TIPO_REAL -> float, outros -> void
 * Retorna ponteiro para string literal estatica (nao liberar) */
static const char* tipo_llvm(TipoExp t) {
    if (t == TIPO_INTEIRO) return "i32";
    if (t == TIPO_REAL) return "float";
    return "void";
}


/* Unifica dois operandos para o mesmo tipo emitindo cast se necessario
 * Converte inteiro para real via sitofp quando os tipos diferem
 * Retorna o tipo resultante apos a unificacao */
static TipoExp unificar_tipos(char** reg_esq, TipoExp tipo_esq, char** reg_dir, TipoExp tipo_dir) {
    if (tipo_esq == tipo_dir) {
        return tipo_esq;
    }
    
    if (tipo_esq == TIPO_INTEIRO && tipo_dir == TIPO_REAL) {
        char* novo_reg = obter_novo_reg();
        fprintf(saida, "  %s = sitofp i32 %s to float\n", novo_reg, *reg_esq);
        free(*reg_esq);
        *reg_esq = novo_reg;
        return TIPO_REAL;
    }
    
    if (tipo_esq == TIPO_REAL && tipo_dir == TIPO_INTEIRO) {
        char* novo_reg = obter_novo_reg();
        fprintf(saida, "  %s = sitofp i32 %s to float\n", novo_reg, *reg_dir);
        free(*reg_dir);
        *reg_dir = novo_reg;
        return TIPO_REAL;
    }
    
    return TIPO_DESCONHECIDO;
}


/* Gera declaracoes de variaveis globais em LLVM IR
 * Insere cada variavel na tabela de simbolos e emite a diretiva global
 * Aborta com erro semantico em caso de redeclaracao*/
static void gerar_decls_globais(NoAST* lista) {
    if (!lista) return;
    for (int i = 0; i < lista->lista.quantidade; i++) {
        NoAST* decl = lista->lista.itens[i];
        NoAST* ids = decl->decl.lista_id;
        TipoExp t = decl->decl.tipo_var;
        for (int j = 0; j < ids->lista.quantidade; j++) {
            char* nome = ids->lista.itens[j]->str_id;
            NoSimb* simb = tabela_simb_inserir_var(nome, t, 0, 1);
            if (!simb) {
                fprintf(stderr, "Erro semantico: variavel global ja declarada: %s\n", nome);
                exit(1);
            }
            // Variaveis globais
            fprintf(saida, "%s = global %s 0\n", simb->nome_llvm, tipo_llvm(t));
        }
    }
}


/* Gera declaracoes de variaveis locais via alloca em LLVM IR
 * Insere cada variavel no escopo atual e emite alloca + store de zero
 * Aborta com erro semantico em caso de redeclaracao */
static void gerar_decls_locais(NoAST* lista) {
    if (!lista) return;
    for (int i = 0; i < lista->lista.quantidade; i++) {
        NoAST* decl = lista->lista.itens[i];
        NoAST* ids = decl->decl.lista_id;
        TipoExp t = decl->decl.tipo_var;
        for (int j = 0; j < ids->lista.quantidade; j++) {
            char* nome = ids->lista.itens[j]->str_id;
            NoSimb* simb = tabela_simb_inserir_var(nome, t, 0, 0);
            if (!simb) {
                fprintf(stderr, "Erro semantico: variavel local ja declarada: %s\n", nome);
                exit(1);
            }
            fprintf(saida, "  %s = alloca %s\n", simb->nome_llvm, tipo_llvm(t));
            // Inicializa com 0
            if (t == TIPO_INTEIRO) {
                fprintf(saida, "  store i32 0, ptr %s\n", simb->nome_llvm);
            } else if (t == TIPO_REAL) {
                fprintf(saida, "  store float 0.0, ptr %s\n", simb->nome_llvm);
            }
        }
    }
}


/* Gera o codigo LLVM de cada subprograma (funcao/procedimento) da lista
 * Delega a geracao de cada no para gerar_no
 * Ignora silenciosamente se a lista for nula */
static void gerar_funcoes(NoAST* lista) {
    if (!lista) return;
    for (int i = 0; i < lista->lista.quantidade; i++) {
        gerar_no(lista->lista.itens[i]);
    }
}


/* Gera codigo LLVM para uma expressao e retorna o registrador resultado
 * Preenche tipo_saida com o tipo inferido da expressao
 * O chamador e responsavel por liberar o registrador retornado */
static char* gerar_exp(NoAST* no, TipoExp* tipo_saida) {
    if (!no) {
        *tipo_saida = TIPO_VOID;
        return NULL;
    }
    
    switch (no->tipo) {
        case AST_NUM_INT: {
            char* reg = obter_novo_reg();
            fprintf(saida, "  %s = add i32 0, %d\n", reg, no->num_int);
            *tipo_saida = TIPO_INTEIRO;
            return reg;
        }
        case AST_NUM_REAL: {
            char* reg = obter_novo_reg();
            fprintf(saida, "  %s = fadd float 0.0, %e\n", reg, no->num_real);
            *tipo_saida = TIPO_REAL;
            return reg;
        }
        case AST_VARIAVEL: {
            NoSimb* simb = tabela_simb_buscar(no->variavel.id);
            if (!simb) {
                fprintf(stderr, "Erro semantico: variavel nao declarada: %s\n", no->variavel.id);
                exit(1);
            }
            char* reg = obter_novo_reg();
            if (simb->eh_ref) {
                // Parametros por referencia: carrega o ponteiro antes do valor
                char* temp_ptr = obter_novo_reg();
                fprintf(saida, "  %s = load ptr, ptr %s\n", temp_ptr, simb->nome_llvm);
                fprintf(saida, "  %s = load %s, ptr %s\n", reg, tipo_llvm(simb->tipo), temp_ptr);
                free(temp_ptr);
            } else {
                fprintf(saida, "  %s = load %s, ptr %s\n", reg, tipo_llvm(simb->tipo), simb->nome_llvm);
            }
            *tipo_saida = simb->tipo;
            return reg;
        }
        case AST_CHAMADA: {
            NoSimb* simb = tabela_simb_buscar_func(no->chamada.id);
            if (!simb) {
                fprintf(stderr, "Erro semantico: funcao nao declarada: %s\n", no->chamada.id);
                exit(1);
            }
            
            int qtd_arg = no->chamada.argumentos ? no->chamada.argumentos->lista.quantidade : 0;
            
            int qtd_param = 0;
            NoSimb* tmp_p = simb->parametros;
            while(tmp_p) { qtd_param++; tmp_p = tmp_p->prox; }
            if (qtd_arg != qtd_param) {
                fprintf(stderr, "Erro semantico: numero incorreto de argumentos para %s\n", no->chamada.id);
                exit(1);
            }
            
            char* str_args = (char*)calloc(1, 4096);
            NoSimb* param = simb->parametros;
            
            for (int i = 0; i < qtd_arg; i++) {
                NoAST* arg = no->chamada.argumentos->lista.itens[i];
                if (param && param->eh_ref) {
                    // Parametro por referencia: passa o endereco da variavel
                    if (arg->tipo != AST_VARIAVEL) {
                        fprintf(stderr, "Erro semantico: parametro de referencia exige uma variavel\n");
                        exit(1);
                    }
                    NoSimb* arg_simb = tabela_simb_buscar(arg->variavel.id);
                    if (i > 0) strcat(str_args, ", ");
                    char temp[256];
                    if (arg_simb->eh_ref) {
                        char* temp_ptr = obter_novo_reg();
                        fprintf(saida, "  %s = load ptr, ptr %s\n", temp_ptr, arg_simb->nome_llvm);
                        snprintf(temp, sizeof(temp), "ptr %s", temp_ptr);
                        free(temp_ptr);
                    } else {
                        snprintf(temp, sizeof(temp), "ptr %s", arg_simb->nome_llvm);
                    }
                    strcat(str_args, temp);
                } else {
                    TipoExp arg_t;
                    char* reg_arg = gerar_exp(arg, &arg_t);
                    if (param && param->tipo == TIPO_REAL && arg_t == TIPO_INTEIRO) {
                        // Cast implicito de inteiro para real no argumento
                        char* reg_cast = obter_novo_reg();
                        fprintf(saida, "  %s = sitofp i32 %s to float\n", reg_cast, reg_arg);
                        free(reg_arg);
                        reg_arg = reg_cast;
                        arg_t = TIPO_REAL;
                    }
                    if (i > 0) strcat(str_args, ", ");
                    char temp[256];
                    snprintf(temp, sizeof(temp), "%s %s", tipo_llvm(arg_t), reg_arg);
                    strcat(str_args, temp);
                    free(reg_arg);
                }
                if (param) param = param->prox;
            }
            
            if (simb->tipo == TIPO_VOID) {
                fprintf(saida, "  call void @%s(%s)\n", simb->nome, str_args);
                *tipo_saida = TIPO_VOID;
                free(str_args);
                return NULL;
            } else {
                char* reg = obter_novo_reg();
                fprintf(saida, "  %s = call %s @%s(%s)\n", reg, tipo_llvm(simb->tipo), simb->nome, str_args);
                *tipo_saida = simb->tipo;
                free(str_args);
                return reg;
            }
        }
        case AST_OP_BINARIA: {
            TipoExp t1, t2;
            char* r1 = gerar_exp(no->op_binaria.esq, &t1);
            char* r2 = gerar_exp(no->op_binaria.dir, &t2);
            TipoExp t = unificar_tipos(&r1, t1, &r2, t2);
            
            char* reg = obter_novo_reg();
            const char* str_op = "";
            
            if (t == TIPO_REAL) {
                switch(no->op_binaria.op) {
                    case OP_SOMA: str_op = "fadd"; break;
                    case OP_SUBTRACAO: str_op = "fsub"; break;
                    case OP_MULT: str_op = "fmul"; break;
                    case OP_DIV_REAL: str_op = "fdiv"; break;
                    case OP_IGUAL: str_op = "fcmp oeq"; break;
                    case OP_DIF: str_op = "fcmp one"; break;
                    case OP_MAIOR: str_op = "fcmp ogt"; break;
                    case OP_MAIOR_IGUAL: str_op = "fcmp oge"; break;
                    case OP_MENOR: str_op = "fcmp olt"; break;
                    case OP_MENOR_IGUAL: str_op = "fcmp ole"; break;
                    default: break;
                }
            } else {
                switch(no->op_binaria.op) {
                    case OP_SOMA: str_op = "add nsw"; break;
                    case OP_SUBTRACAO: str_op = "sub nsw"; break;
                    case OP_MULT: str_op = "mul nsw"; break;
                    case OP_DIV_INT: str_op = "sdiv"; break;
                    case OP_MOD: str_op = "srem"; break;
                    case OP_E: str_op = "and"; break;
                    case OP_OU: str_op = "or"; break;
                    case OP_IGUAL: str_op = "icmp eq"; break;
                    case OP_DIF: str_op = "icmp ne"; break;
                    case OP_MAIOR: str_op = "icmp sgt"; break;
                    case OP_MAIOR_IGUAL: str_op = "icmp sge"; break;
                    case OP_MENOR: str_op = "icmp slt"; break;
                    case OP_MENOR_IGUAL: str_op = "icmp sle"; break;
                    case OP_DIV_REAL: {
                        // Converte ambos para float e faz fdiv
                        char* fr1 = obter_novo_reg();
                        fprintf(saida, "  %s = sitofp i32 %s to float\n", fr1, r1);
                        char* fr2 = obter_novo_reg();
                        fprintf(saida, "  %s = sitofp i32 %s to float\n", fr2, r2);
                        fprintf(saida, "  %s = fdiv float %s, %s\n", reg, fr1, fr2);
                        free(r1); free(r2); free(fr1); free(fr2);
                        *tipo_saida = TIPO_REAL;
                        return reg;
                    }
                    default: break;
                }
            }
            
            fprintf(saida, "  %s = %s %s %s, %s\n", reg, str_op, tipo_llvm(t), r1, r2);
            free(r1);
            free(r2);
            
            if (no->op_binaria.op >= OP_IGUAL && no->op_binaria.op <= OP_MENOR_IGUAL) {
                // Comparacoes retornam i1 no LLVM; converte para i32 (inteiro Pascal)
                char* reg_zext = obter_novo_reg();
                fprintf(saida, "  %s = zext i1 %s to i32\n", reg_zext, reg);
                free(reg);
                reg = reg_zext;
                *tipo_saida = TIPO_INTEIRO;
            } else {
                *tipo_saida = t;
            }
            return reg;
        }
        default:
            *tipo_saida = TIPO_VOID;
            return NULL;
    }
}


/* Gera codigo LLVM para um comando da AST
 * Trata atribuicao, chamada (incluindo read/write), if e while
 * Delega expressoes para gerar_exp e blocos compostos recursivamente */
static void gerar_comando(NoAST* no) {
    if (!no) return;
    
    switch (no->tipo) {
        case AST_COMPOSTO: {
            for (int i = 0; i < no->lista.quantidade; i++) {
                gerar_comando(no->lista.itens[i]);
            }
            break;
        }
        case AST_ATRIBUICAO: {
            TipoExp tipo_r;
            char* reg_exp = gerar_exp(no->atribuicao.expr, &tipo_r);
            
            NoSimb* simb = tabela_simb_buscar(no->atribuicao.id);
            if (!simb) {
                fprintf(stderr, "Erro semantico: variavel nao declarada: %s\n", no->atribuicao.id);
                exit(1);
            }
            
            if (simb->tipo == TIPO_REAL && tipo_r == TIPO_INTEIRO) {
                // Cast implicito de inteiro para real na atribuicao
                char* reg_cast = obter_novo_reg();
                fprintf(saida, "  %s = sitofp i32 %s to float\n", reg_cast, reg_exp);
                free(reg_exp);
                reg_exp = reg_cast;
                tipo_r = TIPO_REAL;
            } else if (simb->tipo == TIPO_INTEIRO && tipo_r == TIPO_REAL) {
                fprintf(stderr, "Erro semantico: nao e possivel converter real para inteiro em atribuicao para %s\n", simb->nome);
                exit(1);
            }
            
            if (simb->eh_ref) {
                 // Parametro por referencia: desreferencia antes de armazenar
                char* ptr_temp = obter_novo_reg();
                fprintf(saida, "  %s = load ptr, ptr %s\n", ptr_temp, simb->nome_llvm);
                fprintf(saida, "  store %s %s, ptr %s\n", tipo_llvm(tipo_r), reg_exp, ptr_temp);
                free(ptr_temp);
            } else {
                fprintf(saida, "  store %s %s, ptr %s\n", tipo_llvm(tipo_r), reg_exp, simb->nome_llvm);
            }
            free(reg_exp);
            break;
        }
        case AST_CHAMADA: {
            if (strcasecmp(no->chamada.id, "read") == 0) {
                // Procedimento built-in read: lê valores via scanf para cada argumento
                for (int i = 0; i < no->chamada.argumentos->lista.quantidade; i++) {
                    NoAST* arg = no->chamada.argumentos->lista.itens[i];
                    if (arg->tipo != AST_VARIAVEL) continue;
                    NoSimb* simb = tabela_simb_buscar(arg->variavel.id);
                    if (!simb) continue;
                    
                    char* ptr_usado = simb->nome_llvm;
                    char* ptr_temp = NULL;
                    if (simb->eh_ref) {
                        ptr_temp = obter_novo_reg();
                        fprintf(saida, "  %s = load ptr, ptr %s\n", ptr_temp, simb->nome_llvm);
                        ptr_usado = ptr_temp;
                    }
                    
                    char* formato = simb->tipo == TIPO_REAL ? "@read_float" : "@read_int";
                    char* temp = obter_novo_reg();
                    fprintf(saida, "  %s = call i32 (ptr, ...) @__isoc99_scanf(ptr %s, ptr %s)\n", temp, formato, ptr_usado);
                    free(temp);
                    if (ptr_temp) free(ptr_temp);
                }
            } else if (strcasecmp(no->chamada.id, "write") == 0) {
                // Procedimento built-in write: imprime valores via printf para cada argumento
                for (int i = 0; i < no->chamada.argumentos->lista.quantidade; i++) {
                    TipoExp t;
                    char* reg = gerar_exp(no->chamada.argumentos->lista.itens[i], &t);
                    char* formato = t == TIPO_REAL ? "@write_float" : "@write_int";
                    
                    if (t == TIPO_REAL) {
                        // printf exige double: promove float com fpext
                        char* reg_cast = obter_novo_reg();
                        fprintf(saida, "  %s = fpext float %s to double\n", reg_cast, reg);
                        char* temp = obter_novo_reg();
                        fprintf(saida, "  %s = call i32 (ptr, ...) @printf(ptr %s, double %s)\n", temp, formato, reg_cast);
                        free(reg_cast);
                        free(temp);
                    } else {
                        char* temp = obter_novo_reg();
                        fprintf(saida, "  %s = call i32 (ptr, ...) @printf(ptr %s, i32 %s)\n", temp, formato, reg);
                        free(temp);
                    }
                    free(reg);
                }
            } else {
                // Chamada de funcao/procedimento usuario: delega para gerar_exp
                TipoExp dummy;
                char* reg = gerar_exp(no, &dummy);
                if (reg) free(reg);
            }
            break;
        }
        case AST_SE: {
            // Gera condicao, trunca para i1 e emite branch condicional
            TipoExp tipo_cond;
            char* reg_cond = gerar_exp(no->comando_se.condicao, &tipo_cond);
            char* reg_trunc = obter_novo_reg();
            fprintf(saida, "  %s = trunc i32 %s to i1\n", reg_trunc, reg_cond);
            free(reg_cond);
            
            char* label_entao = obter_nova_label();
            char* label_senao = obter_nova_label();
            char* label_fim = obter_nova_label();
            
            fprintf(saida, "  br i1 %s, label %%%s, label %%%s\n", reg_trunc, label_entao, label_senao);
            
            fprintf(saida, "\n%s:\n", label_entao);
            gerar_comando(no->comando_se.ramo_entao);
            fprintf(saida, "  br label %%%s\n", label_fim);
            
            fprintf(saida, "\n%s:\n", label_senao);
            gerar_comando(no->comando_se.ramo_senao);
            fprintf(saida, "  br label %%%s\n", label_fim);
            
            fprintf(saida, "\n%s:\n", label_fim);
            
            free(reg_trunc);
            free(label_entao);
            free(label_senao);
            free(label_fim);
            break;
        }
        case AST_ENQUANTO: {
            // Emite laco com label de condicao, corpo e saida
            char* label_cond = obter_nova_label();
            char* label_corpo = obter_nova_label();
            char* label_fim = obter_nova_label();
            
            fprintf(saida, "  br label %%%s\n", label_cond);
            fprintf(saida, "\n%s:\n", label_cond);
            
            TipoExp tipo_cond;
            char* reg_cond = gerar_exp(no->comando_enquanto.condicao, &tipo_cond);
            char* reg_trunc = obter_novo_reg();
            fprintf(saida, "  %s = trunc i32 %s to i1\n", reg_trunc, reg_cond);
            free(reg_cond);
            
            fprintf(saida, "  br i1 %s, label %%%s, label %%%s\n", reg_trunc, label_corpo, label_fim);
            
            fprintf(saida, "\n%s:\n", label_corpo);
            gerar_comando(no->comando_enquanto.corpo);
            fprintf(saida, "  br label %%%s\n", label_cond);
            
            fprintf(saida, "\n%s:\n", label_fim);
            
            free(reg_trunc);
            free(label_cond);
            free(label_corpo);
            free(label_fim);
            break;
        }
        default:
            break;
    }
}


/* Gera codigo LLVM para um no da AST (programa ou declaracao de funcao)
 * Para AST_PROGRAMA, emite variaveis globais, subprogramas e o main
 * Para AST_DECL_FUNC, emite a assinatura, alocas e corpo da funcao */
static void gerar_no(NoAST* no) {
    if (!no) return;
    
    switch (no->tipo) {
        case AST_PROGRAMA:
            gerar_decls_globais(no->programa.declaracoes);
            gerar_funcoes(no->programa.subprogramas);
            
            fprintf(saida, "\ndefine i32 @main() {\n");
            contador_reg = 1;
            
            gerar_comando(no->programa.composto);
            
            fprintf(saida, "  ret i32 0\n");
            fprintf(saida, "}\n");
            break;
            
        case AST_DECL_FUNC: {
            tabela_simb_entrar_escopo();
            
            NoSimb* param_inicio = NULL;
            NoSimb* param_fim = NULL;
            
            char* str_param = (char*)calloc(1, 4096);
            if (no->decl_func.parametros) {
                NoAST* plist = no->decl_func.parametros;
                for (int i = 0; i < plist->lista.quantidade; i++) {
                    NoAST* param = plist->lista.itens[i];
                    for (int j = 0; j < param->parametro.lista_id->lista.quantidade; j++) {
                        char* nome = param->parametro.lista_id->lista.itens[j]->str_id;
                        NoSimb* p = tabela_simb_criar_param(nome, param->parametro.tipo_var, param->parametro.eh_ref);
                        if (!param_inicio) {
                            param_inicio = p;
                            param_fim = p;
                        } else {
                            param_fim->prox = p;
                            param_fim = p;
                        }
                    }
                }
            }
            
            // Sai do escopo temporario para inserir a funcao no escopo pai
            tabela_simb_sair_escopo(); 
            NoSimb* func_inserida = tabela_simb_inserir_func(no->decl_func.nome, no->decl_func.tipo_retorno, param_inicio);
            if (!func_inserida) {
                fprintf(stderr, "Erro semantico: funcao ja declarada: %s\n", no->decl_func.nome);
                exit(1);
            }
            tabela_simb_entrar_escopo(); // Reentra no escopo local
            
            // Reinserir parametros e construir string de assinatura
            NoSimb* curr_param = param_inicio;
            int primeiro = 1;
            while(curr_param) {
                NoSimb* local_p = tabela_simb_inserir_var(curr_param->nome, curr_param->tipo, curr_param->eh_ref, 0);
                if (!local_p) {
                    fprintf(stderr, "Erro semantico: parametro ja declarado: %s\n", curr_param->nome);
                    exit(1);
                }
                
                char temp[256];
                if (curr_param->eh_ref) {
                    snprintf(temp, sizeof(temp), "ptr %%_arg_%s", curr_param->nome);
                } else {
                    snprintf(temp, sizeof(temp), "%s %%_arg_%s", tipo_llvm(curr_param->tipo), curr_param->nome);
                }
                
                if (!primeiro) strcat(str_param, ", ");
                strcat(str_param, temp);
                primeiro = 0;
                
                curr_param = curr_param->prox;
            }
            
            // Adiciona var local com mesmo nome da funcao para o retorno (em Pascal)
            NoSimb* var_retorno = NULL;
            if (no->decl_func.eh_funcao) {
                var_retorno = tabela_simb_inserir_var(no->decl_func.nome, no->decl_func.tipo_retorno, 0, 0);
            }
            
            fprintf(saida, "\ndefine %s @%s(%s) {\n", tipo_llvm(no->decl_func.tipo_retorno), no->decl_func.nome, str_param);
            free(str_param);
            
            contador_reg = 1;
            
            // Aloca parametros na stack
            curr_param = param_inicio;
            while(curr_param) {
                NoSimb* local_p = tabela_simb_buscar(curr_param->nome);
                if (curr_param->eh_ref) {
                    fprintf(saida, "  %s = alloca ptr\n", local_p->nome_llvm);
                    fprintf(saida, "  store ptr %%_arg_%s, ptr %s\n", curr_param->nome, local_p->nome_llvm);
                } else {
                    fprintf(saida, "  %s = alloca %s\n", local_p->nome_llvm, tipo_llvm(curr_param->tipo));
                    fprintf(saida, "  store %s %%_arg_%s, ptr %s\n", tipo_llvm(curr_param->tipo), curr_param->nome, local_p->nome_llvm);
                }
                curr_param = curr_param->prox;
            }
            
            if (var_retorno) {
                fprintf(saida, "  %s = alloca %s\n", var_retorno->nome_llvm, tipo_llvm(no->decl_func.tipo_retorno));
            }
            
            gerar_decls_locais(no->decl_func.declaracoes);
            
            gerar_comando(no->decl_func.composto);
            
            if (no->decl_func.eh_funcao) {
                char* reg_retorno = obter_novo_reg();
                fprintf(saida, "  %s = load %s, ptr %s\n", reg_retorno, tipo_llvm(no->decl_func.tipo_retorno), var_retorno->nome_llvm);
                fprintf(saida, "  ret %s %s\n", tipo_llvm(no->decl_func.tipo_retorno), reg_retorno);
                free(reg_retorno);
            } else {
                fprintf(saida, "  ret void\n");
            }
            
            fprintf(saida, "}\n");
            
            tabela_simb_sair_escopo();
            break;
        }
        default:
            break;
    }
}


/* Ponto de entrada do gerador de codigo LLVM IR
 * Inicializa tabela de simbolos, abre o arquivo de saida e emite os cabecalhos
 * Percorre a AST completa e fecha o arquivo ao terminar */
void gerador_codigo_gerar(NoAST* raiz) {
    tabela_simb_iniciar();
    
    saida = fopen("a.out", "w");
    if (!saida) {
        perror("fopen");
        exit(1);
    }
    
    fprintf(saida, "declare i32 @printf(ptr noundef, ...)\n");
    fprintf(saida, "declare i32 @__isoc99_scanf(ptr noundef, ...)\n");
    fprintf(saida, "@read_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
    fprintf(saida, "@write_int = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n");
    fprintf(saida, "@read_float = private unnamed_addr constant [3 x i8] c\"%%f\\00\", align 1\n");
    fprintf(saida, "@write_float = private unnamed_addr constant [4 x i8] c\"%%f\\0A\\00\", align 1\n\n");
    
    gerar_no(raiz);
    
    fclose(saida);
}
