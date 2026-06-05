#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Scope* current_scope = NULL;

void symtab_init() {
    current_scope = NULL;
    symtab_enter_scope(); // Global scope
}

void symtab_enter_scope() {
    Scope* s = (Scope*)malloc(sizeof(Scope));
    s->locals = NULL;
    s->parent = current_scope;
    current_scope = s;
}

void symtab_exit_scope() {
    if (!current_scope) return;
    Scope* s = current_scope;
    current_scope = s->parent;
    
    // Free locals
    SymNode* curr = s->locals;
    while (curr) {
        SymNode* next = curr->next;
        free(curr->name);
        if (curr->llvm_name) free(curr->llvm_name);
        
        // Free params if function
        SymNode* p = curr->params;
        while(p) {
            SymNode* pnext = p->next;
            free(p->name);
            free(p);
            p = pnext;
        }
        
        free(curr);
        curr = next;
    }
    free(s);
}

SymNode* symtab_insert_var(char* name, ExprType type, int is_ref, int is_global) {
    if (!current_scope) return NULL;
    
    // Check if exists in current scope
    SymNode* curr = current_scope->locals;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return NULL; // Already exists
        }
        curr = curr->next;
    }
    
    SymNode* n = (SymNode*)calloc(1, sizeof(SymNode));
    n->name = strdup(name);
    n->type = type;
    n->is_ref = is_ref;
    n->is_func = 0;
    n->is_global = is_global;
    
    // Generate simple llvm name (can be updated by codegen later if needed)
    char buf[256];
    if (is_global) {
        snprintf(buf, sizeof(buf), "@%s", name);
    } else {
        snprintf(buf, sizeof(buf), "%%_%s", name);
    }
    n->llvm_name = strdup(buf);
    
    n->next = current_scope->locals;
    current_scope->locals = n;
    
    return n;
}

SymNode* symtab_insert_func(char* name, ExprType ret_type, SymNode* params) {
    if (!current_scope) return NULL;
    
    SymNode* curr = current_scope->locals;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return NULL; // Already exists
        }
        curr = curr->next;
    }
    
    SymNode* n = (SymNode*)calloc(1, sizeof(SymNode));
    n->name = strdup(name);
    n->type = ret_type;
    n->is_func = 1;
    n->params = params;
    n->is_global = 1; // Functions are global
    
    char buf[256];
    snprintf(buf, sizeof(buf), "@%s", name);
    n->llvm_name = strdup(buf);
    
    n->next = current_scope->locals;
    current_scope->locals = n;
    
    return n;
}

SymNode* symtab_lookup(char* name) {
    Scope* s = current_scope;
    while (s) {
        SymNode* curr = s->locals;
        while (curr) {
            if (strcmp(curr->name, name) == 0) {
                return curr;
            }
            curr = curr->next;
        }
        s = s->parent;
    }
    return NULL;
}

SymNode* symtab_lookup_func(char* name) {
    Scope* s = current_scope;
    while (s) {
        SymNode* curr = s->locals;
        while (curr) {
            if (strcmp(curr->name, name) == 0 && curr->is_func) {
                return curr;
            }
            curr = curr->next;
        }
        s = s->parent;
    }
    return NULL;
}

SymNode* symtab_create_param(char* name, ExprType type, int is_ref) {
    SymNode* n = (SymNode*)calloc(1, sizeof(SymNode));
    n->name = strdup(name);
    n->type = type;
    n->is_ref = is_ref;
    return n;
}
