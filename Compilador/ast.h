#ifndef AST_H
#define AST_H

typedef enum {
    TYPE_UNKNOWN,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_VOID
} ExprType;

typedef enum {
    AST_PROGRAM,
    AST_DECLARATION,
    AST_FUNC_DECL,
    AST_PARAM,
    AST_COMPOUND,
    AST_ASSIGN,
    AST_CALL,
    AST_IF,
    AST_WHILE,
    AST_BINOP,
    AST_VAR,
    AST_NUM_INT,
    AST_NUM_REAL,
    AST_STMT_LIST,
    AST_EXPR_LIST,
    AST_PARAM_LIST,
    AST_DECL_LIST,
    AST_ID_LIST
} ASTNodeType;

typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV_INT, OP_DIV_REAL, OP_MOD,
    OP_AND, OP_OR,
    OP_EQ, OP_NEQ, OP_GT, OP_GTE, OP_LT, OP_LTE
} OpType;

typedef struct ASTNode {
    ASTNodeType type;
    ExprType expr_type; // For expressions and variables
    int line_no;

    union {
        // AST_PROGRAM
        struct {
            char* name;
            struct ASTNode* id_list;
            struct ASTNode* decls;
            struct ASTNode* subprograms;
            struct ASTNode* compound;
        } program;

        // AST_DECLARATION
        struct {
            struct ASTNode* id_list;
            ExprType var_type;
        } decl;

        // AST_FUNC_DECL
        struct {
            char* name;
            int is_function; // 1 for function, 0 for procedure
            struct ASTNode* params;
            ExprType return_type;
            struct ASTNode* decls;
            struct ASTNode* compound;
        } func_decl;

        // AST_PARAM
        struct {
            struct ASTNode* id_list;
            ExprType var_type;
            int is_ref; // 1 for VAR parameter
        } param;

        // AST_COMPOUND, AST_STMT_LIST, AST_EXPR_LIST, AST_PARAM_LIST, AST_DECL_LIST, AST_ID_LIST
        struct {
            struct ASTNode** items;
            int count;
        } list;

        // AST_ASSIGN
        struct {
            char* id;
            struct ASTNode* expr;
        } assign;

        // AST_CALL
        struct {
            char* id;
            struct ASTNode* args;
        } call;

        // AST_IF
        struct {
            struct ASTNode* cond;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_stmt;

        // AST_WHILE
        struct {
            struct ASTNode* cond;
            struct ASTNode* body;
        } while_stmt;

        // AST_BINOP
        struct {
            OpType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;

        // AST_VAR
        struct {
            char* id;
        } var;

        // AST_NUM_INT
        int num_int;

        // AST_NUM_REAL
        double num_real;
        
        // Generic string for ID list element
        char* id_str;
    };
} ASTNode;

ASTNode* ast_new_program(char* name, ASTNode* id_list, ASTNode* decls, ASTNode* subprograms, ASTNode* compound, int line);
ASTNode* ast_new_decl(ASTNode* id_list, ExprType type, int line);
ASTNode* ast_new_func_decl(char* name, int is_function, ASTNode* params, ExprType ret_type, ASTNode* decls, ASTNode* compound, int line);
ASTNode* ast_new_param(ASTNode* id_list, ExprType type, int is_ref, int line);
ASTNode* ast_new_compound(ASTNode* stmt_list, int line);
ASTNode* ast_new_assign(char* id, ASTNode* expr, int line);
ASTNode* ast_new_call(char* id, ASTNode* args, int line);
ASTNode* ast_new_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch, int line);
ASTNode* ast_new_while(ASTNode* cond, ASTNode* body, int line);
ASTNode* ast_new_binop(OpType op, ASTNode* left, ASTNode* right, int line);
ASTNode* ast_new_var(char* id, int line);
ASTNode* ast_new_num_int(int val, int line);
ASTNode* ast_new_num_real(double val, int line);

// List operations
ASTNode* ast_new_list(ASTNodeType list_type, int line);
void ast_list_add(ASTNode* list, ASTNode* item);
ASTNode* ast_new_id_list_item(char* id, int line);

void ast_free(ASTNode* node);

#endif
