// ast.h
#ifndef AST_H
#define AST_H

#include "common/types.h"
#include <stdbool.h>

typedef enum
{
    NODE_PROGRAM,
    NODE_PRINT,
    NODE_EXPRESSION,
    NODE_ASSIGNMENT,
    NODE_VAR_DECLARATION,
    NODE_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_INT_LITERAL,
    NODE_STRING_LITERAL,
    NODE_BINARY_OP,
    NODE_BOOL_LITERAL
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    struct ASTNode *left;
    struct ASTNode *right;

    char *var_type;
    char *var_name;
    char *value;
    struct ASTNode *next;
    int int_value;
} ASTNode;

/**
 * @brief Creates a new AST node.
 * 
 * @param type The type of the node.
 * @param left The left child node.
 * @param right The right child node.
 * @param value The value associated with the node (if any).
 * @return ASTNode* A pointer to the newly created ASTNode.
 */
ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right, const char *value);

/**
 * @brief Creates a variable declaration node.
 * 
 * @param type The type of the variable (e.g., "int", "string").
 * @param var_name The name of the variable.
 * @param value The initial value of the variable (if any).
 * @return ASTNode* A pointer to the newly created variable declaration node.
 */
ASTNode *create_var_declaration_node(char *type, char *var_name, ASTNode *value);

/**
 * @brief Creates an assignment node.
 * 
 * @param var_name The name of the variable being assigned.
 * @param value The value being assigned to the variable.
 * @return ASTNode* A pointer to the newly created assignment node.
 */
ASTNode *create_assignment_node(char *var_name, ASTNode *value);

/**
 * @brief Frees the memory allocated for an AST.
 * 
 * @param node The root node of the AST to be freed.
 */
void free_ast(ASTNode *node);

#endif // AST_H
