#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum NodeType {
    NODE_CONST,
    NODE_IF_STMT,
    NODE_STATEMENT
};

struct ASTNode {
    enum NodeType type;
    int const_value;                  // For NODE_CONST
    char statement_name[32];          // For NODE_STATEMENT
    struct ASTNode *cond_link;        // Link to condition expression
    struct ASTNode *then_link;        // Link to then-branch
    struct ASTNode *else_link;        // Link to else-branch
};

struct ASTNode* create_ast_node(enum NodeType type) {
    struct ASTNode *p = (struct ASTNode*)malloc(sizeof(struct ASTNode));
    p->type = type;
    p->const_value = 0;
    p->statement_name[0] = '\0';
    p->cond_link = NULL;
    p->then_link = NULL;
    p->else_link = NULL;
    return p;
}

void free_ast(struct ASTNode *p) {
    if (p == NULL) return;
    free_ast(p->cond_link);
    free_ast(p->then_link);
    free_ast(p->else_link);
    free(p);
}

// Optimization pass: Eliminates unreachable branches from IF statements
struct ASTNode* ast_optimize_dead_code(struct ASTNode *p) {
    if (p == NULL) return NULL;

    p->cond_link = ast_optimize_dead_code(p->cond_link);
    p->then_link = ast_optimize_dead_code(p->then_link);
    p->else_link = ast_optimize_dead_code(p->else_link);

    if (p->type == NODE_IF_STMT && p->cond_link != NULL && p->cond_link->type == NODE_CONST) {
        if (p->cond_link->const_value == 0) {
            printf("  [AST Pass] Eliminating Dead Code: Branch condition is ALWAYS FALSE (0)\n");
            printf("     └─ Pruning unreachable THEN-branch and promoting ELSE-branch\n");
            struct ASTNode *retained_branch = p->else_link;
            
            // Free condition and unreachable then-branch
            free_ast(p->cond_link);
            free_ast(p->then_link);
            free(p);
            return retained_branch;
        }
    }
    return p;
}

void print_ast(struct ASTNode *p, int depth) {
    if (p == NULL) return;
    for (int i = 0; i < depth; i++) printf("  ");

    if (p->type == NODE_CONST) {
        printf("├─ CONST (%d)\n", p->const_value);
    } else if (p->type == NODE_STATEMENT) {
        printf("├─ EXECUTE (%s)\n", p->statement_name);
    } else if (p->type == NODE_IF_STMT) {
        printf("├─ IF_STATEMENT\n");
        print_ast(p->cond_link, depth + 1);
        print_ast(p->then_link, depth + 1);
        print_ast(p->else_link, depth + 1);
    }
}

int main() {
    printf("=== AST Dead Code Elimination & Compiler Optimization Pass (ANSI C) ===\n");

    // Constructing AST representing: IF (0) { STATEMENT("Dead Code"); } ELSE { STATEMENT("Active Code"); }
    struct ASTNode *root = create_ast_node(NODE_IF_STMT);
    
    struct ASTNode *cond = create_ast_node(NODE_CONST);
    cond->const_value = 0; // Always false
    
    struct ASTNode *then_stmt = create_ast_node(NODE_STATEMENT);
    strncpy(then_stmt->statement_name, "Unreachable_Debug_Log", 31);
    
    struct ASTNode *else_stmt = create_ast_node(NODE_STATEMENT);
    strncpy(else_stmt->statement_name, "Active_Production_Kernel", 31);

    root->cond_link = cond;
    root->then_link = then_stmt;
    root->else_link = else_stmt;

    printf("Original Abstract Syntax Tree:\n");
    print_ast(root, 1);

    printf("\nRunning Static Analysis & Dead Code Pruning Pass:\n");
    root = ast_optimize_dead_code(root);

    printf("\nOptimized Abstract Syntax Tree:\n");
    print_ast(root, 1);

    free_ast(root);
    printf("[PASS] Compiler Optimization Pass execution completed cleanly.\n");
    return 0;
}