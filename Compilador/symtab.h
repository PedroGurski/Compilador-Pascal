#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

typedef struct SymNode {
    char* name;
    ExprType type;
    int is_func;
    int is_ref;
    
    // For functions
    struct SymNode* params; // Linked list of parameters
    
    // LLVM specific
    int is_global;
    char* llvm_name; // e.g. "%x" or "@x"
    
    struct SymNode* next; // Next in scope
} SymNode;

typedef struct Scope {
    SymNode* locals;
    struct Scope* parent;
} Scope;

void symtab_init();
void symtab_enter_scope();
void symtab_exit_scope();

SymNode* symtab_insert_var(char* name, ExprType type, int is_ref, int is_global);
SymNode* symtab_insert_func(char* name, ExprType ret_type, SymNode* params);
SymNode* symtab_lookup(char* name);
SymNode* symtab_lookup_func(char* name);

// Helper for function params
SymNode* symtab_create_param(char* name, ExprType type, int is_ref);

#endif
