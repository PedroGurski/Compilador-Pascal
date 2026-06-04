#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode* ast_new_node(ASTNodeType type, int line) {
    ASTNode* node = (ASTNode*)calloc(1, sizeof(ASTNode));
    node->type = type;
    node->line_no = line;
    node->expr_type = TYPE_UNKNOWN;
    return node;
}

ASTNode* ast_new_program(char* name, ASTNode* id_list, ASTNode* decls, ASTNode* subprograms, ASTNode* compound, int line) {
    ASTNode* n = ast_new_node(AST_PROGRAM, line);
    n->program.name = strdup(name);
    n->program.id_list = id_list;
    n->program.decls = decls;
    n->program.subprograms = subprograms;
    n->program.compound = compound;
    return n;
}

ASTNode* ast_new_decl(ASTNode* id_list, ExprType type, int line) {
    ASTNode* n = ast_new_node(AST_DECLARATION, line);
    n->decl.id_list = id_list;
    n->decl.var_type = type;
    return n;
}

ASTNode* ast_new_func_decl(char* name, int is_function, ASTNode* params, ExprType ret_type, ASTNode* decls, ASTNode* compound, int line) {
    ASTNode* n = ast_new_node(AST_FUNC_DECL, line);
    n->func_decl.name = strdup(name);
    n->func_decl.is_function = is_function;
    n->func_decl.params = params;
    n->func_decl.return_type = ret_type;
    n->func_decl.decls = decls;
    n->func_decl.compound = compound;
    return n;
}

ASTNode* ast_new_param(ASTNode* id_list, ExprType type, int is_ref, int line) {
    ASTNode* n = ast_new_node(AST_PARAM, line);
    n->param.id_list = id_list;
    n->param.var_type = type;
    n->param.is_ref = is_ref;
    return n;
}

ASTNode* ast_new_compound(ASTNode* stmt_list, int line) {
    ASTNode* n = ast_new_node(AST_COMPOUND, line);
    n->list.items = stmt_list ? stmt_list->list.items : NULL;
    n->list.count = stmt_list ? stmt_list->list.count : 0;
    if (stmt_list) {
        free(stmt_list); // Free the list node itself
    }
    return n;
}

ASTNode* ast_new_assign(char* id, ASTNode* expr, int line) {
    ASTNode* n = ast_new_node(AST_ASSIGN, line);
    n->assign.id = strdup(id);
    n->assign.expr = expr;
    return n;
}

ASTNode* ast_new_call(char* id, ASTNode* args, int line) {
    ASTNode* n = ast_new_node(AST_CALL, line);
    n->call.id = strdup(id);
    n->call.args = args;
    return n;
}

ASTNode* ast_new_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch, int line) {
    ASTNode* n = ast_new_node(AST_IF, line);
    n->if_stmt.cond = cond;
    n->if_stmt.then_branch = then_branch;
    n->if_stmt.else_branch = else_branch;
    return n;
}

ASTNode* ast_new_while(ASTNode* cond, ASTNode* body, int line) {
    ASTNode* n = ast_new_node(AST_WHILE, line);
    n->while_stmt.cond = cond;
    n->while_stmt.body = body;
    return n;
}

ASTNode* ast_new_binop(OpType op, ASTNode* left, ASTNode* right, int line) {
    ASTNode* n = ast_new_node(AST_BINOP, line);
    n->binop.op = op;
    n->binop.left = left;
    n->binop.right = right;
    return n;
}

ASTNode* ast_new_var(char* id, int line) {
    ASTNode* n = ast_new_node(AST_VAR, line);
    n->var.id = strdup(id);
    return n;
}

ASTNode* ast_new_num_int(int val, int line) {
    ASTNode* n = ast_new_node(AST_NUM_INT, line);
    n->num_int = val;
    n->expr_type = TYPE_INTEGER;
    return n;
}

ASTNode* ast_new_num_real(double val, int line) {
    ASTNode* n = ast_new_node(AST_NUM_REAL, line);
    n->num_real = val;
    n->expr_type = TYPE_REAL;
    return n;
}

ASTNode* ast_new_list(ASTNodeType list_type, int line) {
    ASTNode* n = ast_new_node(list_type, line);
    n->list.items = NULL;
    n->list.count = 0;
    return n;
}

void ast_list_add(ASTNode* list, ASTNode* item) {
    if (!list || !item) return;
    list->list.count++;
    list->list.items = (ASTNode**)realloc(list->list.items, sizeof(ASTNode*) * list->list.count);
    list->list.items[list->list.count - 1] = item;
}

ASTNode* ast_new_id_list_item(char* id, int line) {
    ASTNode* n = ast_new_node(AST_ID_LIST, line);
    n->id_str = strdup(id);
    return n;
}

void ast_free(ASTNode* node) {
    if (!node) return;
    switch(node->type) {
        case AST_PROGRAM:
            free(node->program.name);
            ast_free(node->program.id_list);
            ast_free(node->program.decls);
            ast_free(node->program.subprograms);
            ast_free(node->program.compound);
            break;
        case AST_DECLARATION:
            ast_free(node->decl.id_list);
            break;
        case AST_FUNC_DECL:
            free(node->func_decl.name);
            ast_free(node->func_decl.params);
            ast_free(node->func_decl.decls);
            ast_free(node->func_decl.compound);
            break;
        case AST_PARAM:
            ast_free(node->param.id_list);
            break;
        case AST_ASSIGN:
            free(node->assign.id);
            ast_free(node->assign.expr);
            break;
        case AST_CALL:
            free(node->call.id);
            ast_free(node->call.args);
            break;
        case AST_IF:
            ast_free(node->if_stmt.cond);
            ast_free(node->if_stmt.then_branch);
            ast_free(node->if_stmt.else_branch);
            break;
        case AST_WHILE:
            ast_free(node->while_stmt.cond);
            ast_free(node->while_stmt.body);
            break;
        case AST_BINOP:
            ast_free(node->binop.left);
            ast_free(node->binop.right);
            break;
        case AST_VAR:
            free(node->var.id);
            break;
        case AST_ID_LIST:
            free(node->id_str);
            break;
        case AST_COMPOUND:
        case AST_STMT_LIST:
        case AST_EXPR_LIST:
        case AST_PARAM_LIST:
        case AST_DECL_LIST:
            for (int i = 0; i < node->list.count; i++) {
                ast_free(node->list.items[i]);
            }
            free(node->list.items);
            break;
        default:
            break;
    }
    free(node);
}
