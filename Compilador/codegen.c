#include "codegen.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* out;
static int reg_counter = 1;
static int label_counter = 1;

static void gen_node(ASTNode* node);
static char* gen_expr(ASTNode* node, ExprType* out_type);
static void gen_stmt(ASTNode* node);

static char* get_new_reg() {
    char* buf = (char*)malloc(32);
    sprintf(buf, "%%%d", reg_counter++);
    return buf;
}

static char* get_new_label() {
    char* buf = (char*)malloc(32);
    sprintf(buf, "L%d", label_counter++);
    return buf;
}

static const char* llvm_type(ExprType t) {
    if (t == TYPE_INTEGER) return "i32";
    if (t == TYPE_REAL) return "float";
    return "void";
}

// Ensure both operands of a binop have the same type, returns the common type
// Also emits cast instructions if necessary
static ExprType unify_types(char** reg_left, ExprType type_left, char** reg_right, ExprType type_right) {
    if (type_left == type_right) {
        return type_left;
    }
    
    if (type_left == TYPE_INTEGER && type_right == TYPE_REAL) {
        char* new_reg = get_new_reg();
        fprintf(out, "  %s = sitofp i32 %s to float\n", new_reg, *reg_left);
        free(*reg_left);
        *reg_left = new_reg;
        return TYPE_REAL;
    }
    
    if (type_left == TYPE_REAL && type_right == TYPE_INTEGER) {
        char* new_reg = get_new_reg();
        fprintf(out, "  %s = sitofp i32 %s to float\n", new_reg, *reg_right);
        free(*reg_right);
        *reg_right = new_reg;
        return TYPE_REAL;
    }
    
    // Should not happen if type checker is perfect
    return TYPE_UNKNOWN;
}

static void gen_global_decls(ASTNode* list) {
    if (!list) return;
    for (int i = 0; i < list->list.count; i++) {
        ASTNode* decl = list->list.items[i];
        ASTNode* ids = decl->decl.id_list;
        ExprType t = decl->decl.var_type;
        for (int j = 0; j < ids->list.count; j++) {
            char* name = ids->list.items[j]->id_str;
            SymNode* sym = symtab_insert_var(name, t, 0, 1);
            if (!sym) {
                fprintf(stderr, "Erro semantico: variavel global ja declarada: %s\n", name);
                exit(1);
            }
            // Global vars
            fprintf(out, "%s = global %s 0\n", sym->llvm_name, llvm_type(t));
        }
    }
}

static void gen_local_decls(ASTNode* list) {
    if (!list) return;
    for (int i = 0; i < list->list.count; i++) {
        ASTNode* decl = list->list.items[i];
        ASTNode* ids = decl->decl.id_list;
        ExprType t = decl->decl.var_type;
        for (int j = 0; j < ids->list.count; j++) {
            char* name = ids->list.items[j]->id_str;
            SymNode* sym = symtab_insert_var(name, t, 0, 0);
            if (!sym) {
                fprintf(stderr, "Erro semantico: variavel local ja declarada: %s\n", name);
                exit(1);
            }
            fprintf(out, "  %s = alloca %s\n", sym->llvm_name, llvm_type(t));
            // initialize to 0
            if (t == TYPE_INTEGER) {
                fprintf(out, "  store i32 0, ptr %s\n", sym->llvm_name);
            } else if (t == TYPE_REAL) {
                fprintf(out, "  store float 0.0, ptr %s\n", sym->llvm_name);
            }
        }
    }
}

static void gen_functions(ASTNode* list) {
    if (!list) return;
    for (int i = 0; i < list->list.count; i++) {
        gen_node(list->list.items[i]);
    }
}

static char* gen_expr(ASTNode* node, ExprType* out_type) {
    if (!node) {
        *out_type = TYPE_VOID;
        return NULL;
    }
    
    switch (node->type) {
        case AST_NUM_INT: {
            char* reg = get_new_reg();
            fprintf(out, "  %s = add i32 0, %d\n", reg, node->num_int);
            *out_type = TYPE_INTEGER;
            return reg;
        }
        case AST_NUM_REAL: {
            char* reg = get_new_reg();
            fprintf(out, "  %s = fadd float 0.0, %e\n", reg, node->num_real);
            *out_type = TYPE_REAL;
            return reg;
        }
        case AST_VAR: {
            SymNode* sym = symtab_lookup(node->var.id);
            if (!sym) {
                fprintf(stderr, "Undeclared variable: %s\n", node->var.id);
                exit(1);
            }
            char* reg = get_new_reg();
            if (sym->is_ref) {
                // It's a pointer to a pointer in LLVM? 
                // Wait, if it's passed as reference, it's already a ptr
                char* temp_ptr = get_new_reg();
                fprintf(out, "  %s = load ptr, ptr %s\n", temp_ptr, sym->llvm_name);
                fprintf(out, "  %s = load %s, ptr %s\n", reg, llvm_type(sym->type), temp_ptr);
                free(temp_ptr);
            } else {
                fprintf(out, "  %s = load %s, ptr %s\n", reg, llvm_type(sym->type), sym->llvm_name);
            }
            *out_type = sym->type;
            return reg;
        }
        case AST_CALL: {
            // Check if function or just variable mis-parsed (not likely)
            SymNode* sym = symtab_lookup_func(node->call.id);
            if (!sym) {
                fprintf(stderr, "Undeclared function: %s\n", node->call.id);
                exit(1);
            }
            
            int arg_count = node->call.args ? node->call.args->list.count : 0;
            
            int param_count = 0;
            SymNode* tmp_p = sym->params;
            while(tmp_p) { param_count++; tmp_p = tmp_p->next; }
            if (arg_count != param_count) {
                fprintf(stderr, "Erro semantico: numero incorreto de argumentos para %s\n", node->call.id);
                exit(1);
            }
            
            char* args_str = (char*)calloc(1, 4096);
            SymNode* param = sym->params;
            
            for (int i = 0; i < arg_count; i++) {
                ASTNode* arg = node->call.args->list.items[i];
                if (param && param->is_ref) {
                    if (arg->type != AST_VAR) {
                        fprintf(stderr, "Reference parameter must be a variable\n");
                        exit(1);
                    }
                    SymNode* arg_sym = symtab_lookup(arg->var.id);
                    if (i > 0) strcat(args_str, ", ");
                    char temp[256];
                    if (arg_sym->is_ref) {
                        char* temp_ptr = get_new_reg();
                        fprintf(out, "  %s = load ptr, ptr %s\n", temp_ptr, arg_sym->llvm_name);
                        snprintf(temp, sizeof(temp), "ptr %s", temp_ptr);
                        free(temp_ptr);
                    } else {
                        snprintf(temp, sizeof(temp), "ptr %s", arg_sym->llvm_name);
                    }
                    strcat(args_str, temp);
                } else {
                    ExprType arg_t;
                    char* arg_reg = gen_expr(arg, &arg_t);
                    if (param && param->type == TYPE_REAL && arg_t == TYPE_INTEGER) {
                        char* cast_reg = get_new_reg();
                        fprintf(out, "  %s = sitofp i32 %s to float\n", cast_reg, arg_reg);
                        free(arg_reg);
                        arg_reg = cast_reg;
                        arg_t = TYPE_REAL;
                    }
                    if (i > 0) strcat(args_str, ", ");
                    char temp[256];
                    snprintf(temp, sizeof(temp), "%s %s", llvm_type(arg_t), arg_reg);
                    strcat(args_str, temp);
                    free(arg_reg);
                }
                if (param) param = param->next;
            }
            
            if (sym->type == TYPE_VOID) {
                fprintf(out, "  call void @%s(%s)\n", sym->name, args_str);
                *out_type = TYPE_VOID;
                free(args_str);
                return NULL;
            } else {
                char* reg = get_new_reg();
                fprintf(out, "  %s = call %s @%s(%s)\n", reg, llvm_type(sym->type), sym->name, args_str);
                *out_type = sym->type;
                free(args_str);
                return reg;
            }
        }
        case AST_BINOP: {
            ExprType t1, t2;
            char* r1 = gen_expr(node->binop.left, &t1);
            char* r2 = gen_expr(node->binop.right, &t2);
            ExprType t = unify_types(&r1, t1, &r2, t2);
            
            char* reg = get_new_reg();
            const char* op_str = "";
            
            if (t == TYPE_REAL) {
                switch(node->binop.op) {
                    case OP_ADD: op_str = "fadd"; break;
                    case OP_SUB: op_str = "fsub"; break;
                    case OP_MUL: op_str = "fmul"; break;
                    case OP_DIV_REAL: op_str = "fdiv"; break;
                    // others error
                    case OP_EQ: op_str = "fcmp oeq"; break;
                    case OP_NEQ: op_str = "fcmp one"; break;
                    case OP_GT: op_str = "fcmp ogt"; break;
                    case OP_GTE: op_str = "fcmp oge"; break;
                    case OP_LT: op_str = "fcmp olt"; break;
                    case OP_LTE: op_str = "fcmp ole"; break;
                    default: break;
                }
            } else {
                switch(node->binop.op) {
                    case OP_ADD: op_str = "add nsw"; break;
                    case OP_SUB: op_str = "sub nsw"; break;
                    case OP_MUL: op_str = "mul nsw"; break;
                    case OP_DIV_INT: op_str = "sdiv"; break;
                    case OP_MOD: op_str = "srem"; break;
                    case OP_AND: op_str = "and"; break;
                    case OP_OR: op_str = "or"; break;
                    case OP_EQ: op_str = "icmp eq"; break;
                    case OP_NEQ: op_str = "icmp ne"; break;
                    case OP_GT: op_str = "icmp sgt"; break;
                    case OP_GTE: op_str = "icmp sge"; break;
                    case OP_LT: op_str = "icmp slt"; break;
                    case OP_LTE: op_str = "icmp sle"; break;
                    case OP_DIV_REAL: {
                        // Cast both to float and fdiv
                        char* fr1 = get_new_reg();
                        fprintf(out, "  %s = sitofp i32 %s to float\n", fr1, r1);
                        char* fr2 = get_new_reg();
                        fprintf(out, "  %s = sitofp i32 %s to float\n", fr2, r2);
                        fprintf(out, "  %s = fdiv float %s, %s\n", reg, fr1, fr2);
                        free(r1); free(r2); free(fr1); free(fr2);
                        *out_type = TYPE_REAL;
                        return reg;
                    }
                    default: break;
                }
            }
            
            fprintf(out, "  %s = %s %s %s, %s\n", reg, op_str, llvm_type(t), r1, r2);
            free(r1);
            free(r2);
            
            if (node->binop.op >= OP_EQ && node->binop.op <= OP_LTE) {
                // boolean result is i1, convert to i32 to match pascal integers
                char* zext_reg = get_new_reg();
                fprintf(out, "  %s = zext i1 %s to i32\n", zext_reg, reg);
                free(reg);
                reg = zext_reg;
                *out_type = TYPE_INTEGER;
            } else {
                *out_type = t;
            }
            return reg;
        }
        default:
            *out_type = TYPE_VOID;
            return NULL;
    }
}

static void gen_stmt(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_COMPOUND: {
            for (int i = 0; i < node->list.count; i++) {
                gen_stmt(node->list.items[i]);
            }
            break;
        }
        case AST_ASSIGN: {
            ExprType rtype;
            char* expr_reg = gen_expr(node->assign.expr, &rtype);
            
            SymNode* sym = symtab_lookup(node->assign.id);
            if (!sym) {
                fprintf(stderr, "Undeclared variable: %s\n", node->assign.id);
                exit(1);
            }
            
            if (sym->type == TYPE_REAL && rtype == TYPE_INTEGER) {
                char* cast_reg = get_new_reg();
                fprintf(out, "  %s = sitofp i32 %s to float\n", cast_reg, expr_reg);
                free(expr_reg);
                expr_reg = cast_reg;
                rtype = TYPE_REAL;
            } else if (sym->type == TYPE_INTEGER && rtype == TYPE_REAL) {
                fprintf(stderr, "Cannot implicitly cast real to integer for %s\n", sym->name);
                exit(1);
            }
            
            if (sym->is_ref) {
                char* temp_ptr = get_new_reg();
                fprintf(out, "  %s = load ptr, ptr %s\n", temp_ptr, sym->llvm_name);
                fprintf(out, "  store %s %s, ptr %s\n", llvm_type(rtype), expr_reg, temp_ptr);
                free(temp_ptr);
            } else {
                fprintf(out, "  store %s %s, ptr %s\n", llvm_type(rtype), expr_reg, sym->llvm_name);
            }
            free(expr_reg);
            break;
        }
        case AST_CALL: {
            if (strcasecmp(node->call.id, "read") == 0) {
                for (int i = 0; i < node->call.args->list.count; i++) {
                    ASTNode* arg = node->call.args->list.items[i];
                    if (arg->type != AST_VAR) continue;
                    SymNode* sym = symtab_lookup(arg->var.id);
                    if (!sym) continue;
                    
                    char* ptr_to_use = sym->llvm_name;
                    char* temp_ptr = NULL;
                    if (sym->is_ref) {
                        temp_ptr = get_new_reg();
                        fprintf(out, "  %s = load ptr, ptr %s\n", temp_ptr, sym->llvm_name);
                        ptr_to_use = temp_ptr;
                    }
                    
                    char* format = sym->type == TYPE_REAL ? "@read_float" : "@read_int";
                    char* temp = get_new_reg();
                    fprintf(out, "  %s = call i32 (ptr, ...) @__isoc99_scanf(ptr %s, ptr %s)\n", temp, format, ptr_to_use);
                    free(temp);
                    if (temp_ptr) free(temp_ptr);
                }
            } else if (strcasecmp(node->call.id, "write") == 0) {
                for (int i = 0; i < node->call.args->list.count; i++) {
                    ExprType t;
                    char* reg = gen_expr(node->call.args->list.items[i], &t);
                    char* format = t == TYPE_REAL ? "@write_float" : "@write_int";
                    
                    if (t == TYPE_REAL) {
                        char* cast_reg = get_new_reg();
                        fprintf(out, "  %s = fpext float %s to double\n", cast_reg, reg);
                        char* temp = get_new_reg();
                        fprintf(out, "  %s = call i32 (ptr, ...) @printf(ptr %s, double %s)\n", temp, format, cast_reg);
                        free(cast_reg);
                        free(temp);
                    } else {
                        char* temp = get_new_reg();
                        fprintf(out, "  %s = call i32 (ptr, ...) @printf(ptr %s, i32 %s)\n", temp, format, reg);
                        free(temp);
                    }
                    free(reg);
                }
            } else {
                ExprType dummy;
                char* reg = gen_expr(node, &dummy);
                if (reg) free(reg);
            }
            break;
        }
        case AST_IF: {
            ExprType cond_t;
            char* cond_reg = gen_expr(node->if_stmt.cond, &cond_t);
            char* trunc_reg = get_new_reg();
            fprintf(out, "  %s = trunc i32 %s to i1\n", trunc_reg, cond_reg);
            free(cond_reg);
            
            char* label_then = get_new_label();
            char* label_else = get_new_label();
            char* label_end = get_new_label();
            
            fprintf(out, "  br i1 %s, label %%%s, label %%%s\n", trunc_reg, label_then, label_else);
            
            fprintf(out, "\n%s:\n", label_then);
            gen_stmt(node->if_stmt.then_branch);
            fprintf(out, "  br label %%%s\n", label_end);
            
            fprintf(out, "\n%s:\n", label_else);
            gen_stmt(node->if_stmt.else_branch);
            fprintf(out, "  br label %%%s\n", label_end);
            
            fprintf(out, "\n%s:\n", label_end);
            
            free(trunc_reg);
            free(label_then);
            free(label_else);
            free(label_end);
            break;
        }
        case AST_WHILE: {
            char* label_cond = get_new_label();
            char* label_body = get_new_label();
            char* label_end = get_new_label();
            
            fprintf(out, "  br label %%%s\n", label_cond);
            fprintf(out, "\n%s:\n", label_cond);
            
            ExprType cond_t;
            char* cond_reg = gen_expr(node->while_stmt.cond, &cond_t);
            char* trunc_reg = get_new_reg();
            fprintf(out, "  %s = trunc i32 %s to i1\n", trunc_reg, cond_reg);
            free(cond_reg);
            
            fprintf(out, "  br i1 %s, label %%%s, label %%%s\n", trunc_reg, label_body, label_end);
            
            fprintf(out, "\n%s:\n", label_body);
            gen_stmt(node->while_stmt.body);
            fprintf(out, "  br label %%%s\n", label_cond);
            
            fprintf(out, "\n%s:\n", label_end);
            
            free(trunc_reg);
            free(label_cond);
            free(label_body);
            free(label_end);
            break;
        }
        default:
            break;
    }
}

static void gen_node(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            gen_global_decls(node->program.decls);
            gen_functions(node->program.subprograms);
            
            // Main function
            fprintf(out, "\ndefine i32 @main() {\n");
            reg_counter = 1;
            
            // Initialize globals in main if needed (not needed for simple vars)
            
            gen_stmt(node->program.compound);
            
            fprintf(out, "  ret i32 0\n");
            fprintf(out, "}\n");
            break;
            
        case AST_FUNC_DECL: {
            symtab_enter_scope();
            
            // Collect params
            SymNode* param_head = NULL;
            SymNode* param_tail = NULL;
            
            char* param_str = (char*)calloc(1, 4096);
            if (node->func_decl.params) {
                ASTNode* plist = node->func_decl.params;
                for (int i = 0; i < plist->list.count; i++) {
                    ASTNode* param = plist->list.items[i];
                    for (int j = 0; j < param->param.id_list->list.count; j++) {
                        char* name = param->param.id_list->list.items[j]->id_str;
                        SymNode* p = symtab_create_param(name, param->param.var_type, param->param.is_ref);
                        if (!param_head) {
                            param_head = p;
                            param_tail = p;
                        } else {
                            param_tail->next = p;
                            param_tail = p;
                        }
                    }
                }
            }
            
            // Insert function to parent scope
            symtab_exit_scope(); // Exit the new scope temporarily
            SymNode* inserted_func = symtab_insert_func(node->func_decl.name, node->func_decl.return_type, param_head);
            if (!inserted_func) {
                fprintf(stderr, "Erro semantico: funcao ja declarada: %s\n", node->func_decl.name);
                exit(1);
            }
            symtab_enter_scope(); // Re-enter
            
            // Re-insert params into local scope and build param string
            SymNode* curr_param = param_head;
            int first = 1;
            int p_idx = 0;
            while(curr_param) {
                // Insert param into local scope
                // Variables in LLVM function args are %arg_x
                SymNode* local_p = symtab_insert_var(curr_param->name, curr_param->type, curr_param->is_ref, 0);
                if (!local_p) {
                    fprintf(stderr, "Erro semantico: parametro ja declarado: %s\n", curr_param->name);
                    exit(1);
                }
                
                char temp[256];
                if (curr_param->is_ref) {
                    snprintf(temp, sizeof(temp), "ptr %%_arg_%s", curr_param->name);
                } else {
                    snprintf(temp, sizeof(temp), "%s %%_arg_%s", llvm_type(curr_param->type), curr_param->name);
                }
                
                if (!first) strcat(param_str, ", ");
                strcat(param_str, temp);
                first = 0;
                
                curr_param = curr_param->next;
            }
            
            // Add implicitly the function name as a local variable for return value
            SymNode* ret_var = NULL;
            if (node->func_decl.is_function) {
                ret_var = symtab_insert_var(node->func_decl.name, node->func_decl.return_type, 0, 0);
            }
            
            fprintf(out, "\ndefine %s @%s(%s) {\n", llvm_type(node->func_decl.return_type), node->func_decl.name, param_str);
            free(param_str);
            
            reg_counter = 1;
            
            // Allocate parameters on stack
            curr_param = param_head;
            while(curr_param) {
                SymNode* local_p = symtab_lookup(curr_param->name);
                if (curr_param->is_ref) {
                    fprintf(out, "  %s = alloca ptr\n", local_p->llvm_name);
                    fprintf(out, "  store ptr %%_arg_%s, ptr %s\n", curr_param->name, local_p->llvm_name);
                } else {
                    fprintf(out, "  %s = alloca %s\n", local_p->llvm_name, llvm_type(curr_param->type));
                    fprintf(out, "  store %s %%_arg_%s, ptr %s\n", llvm_type(curr_param->type), curr_param->name, local_p->llvm_name);
                }
                curr_param = curr_param->next;
            }
            
            if (ret_var) {
                fprintf(out, "  %s = alloca %s\n", ret_var->llvm_name, llvm_type(node->func_decl.return_type));
            }
            
            // Allocate locals
            gen_local_decls(node->func_decl.decls);
            
            gen_stmt(node->func_decl.compound);
            
            if (node->func_decl.is_function) {
                char* ret_reg = get_new_reg();
                fprintf(out, "  %s = load %s, ptr %s\n", ret_reg, llvm_type(node->func_decl.return_type), ret_var->llvm_name);
                fprintf(out, "  ret %s %s\n", llvm_type(node->func_decl.return_type), ret_reg);
                free(ret_reg);
            } else {
                fprintf(out, "  ret void\n");
            }
            
            fprintf(out, "}\n");
            
            symtab_exit_scope();
            break;
        }
        default:
            break;
    }
}

void codegen_generate(ASTNode* root) {
    symtab_init();
    
    out = fopen("a.out", "w");
    if (!out) {
        perror("fopen");
        exit(1);
    }
    
    fprintf(out, "declare i32 @printf(ptr noundef, ...)\n");
    fprintf(out, "declare i32 @__isoc99_scanf(ptr noundef, ...)\n");
    fprintf(out, "@read_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
    fprintf(out, "@write_int = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n");
    fprintf(out, "@read_float = private unnamed_addr constant [3 x i8] c\"%%f\\00\", align 1\n");
    fprintf(out, "@write_float = private unnamed_addr constant [4 x i8] c\"%%f\\0A\\00\", align 1\n\n");
    
    gen_node(root);
    
    fclose(out);
}
