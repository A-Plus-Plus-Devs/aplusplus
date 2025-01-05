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
    NODE_CHAR_LITERAL,
    NODE_IF,
    NODE_TERNARY,
    NODE_ELSE,
    NODE_ELSEIF,
    NODE_STRING_LITERAL,
    NODE_BINARY_OP,
    NODE_BOOL_LITERAL,
    NODE_FOR,
    NODE_FUNCTION_DEFINITION,
    NODE_FUNCTION_PARAMETER,
    NODE_YIELD_STATEMENT,
    NODE_FUNCTION_CALL,
    NODE_COMPOUND_ASSIGN,

    NODE_INPUT,
    NODE_TYPE_CAST
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    struct ASTNode *left;
    struct ASTNode *right;

    char *var_type;
    char *var_name;
    struct ASTNode *else_branch;
    struct ASTNode *elseif_branch;


    // Function-specific fields
    char *return_type;           // Return type for functions
    char *function_name;         // Name of the function
    struct ASTNode *parameters;  // Linked list of parameters for function definitions
    struct ASTNode *arguments;   // Linked list of arguments for function calls
    struct ASTNode *body;        // Function body
    struct ASTNode *yield_expr;  // Expression to yield

    char *value;
    struct ASTNode *next;
    int int_value;

    struct ASTNode *init;
    struct ASTNode *condition;
    struct ASTNode *increment;

    char *target_type;  // For type casting operations
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

/**
 * @brief Creates a for loop node.
 * 
 * @param init The initialization node.
 * @param condition The condition node.
 * @param increment The increment node.
 * @param body The body node.
 * @return ASTNode* A pointer to the newly created for loop node.
 */
ASTNode *create_for_node(ASTNode *init, ASTNode *condition, ASTNode *increment, ASTNode *body);

/**
 * @brief Creates a function definition node.
 * 
 * @param return_type The return type of the function.
 * @param function_name The name of the function.
 * @param parameters The list of parameters.
 * @param body The function body.
 * @return ASTNode* A pointer to the newly created function definition node.
 */
ASTNode *create_function_definition_node(char *return_type, char *function_name, ASTNode *parameters, ASTNode *body);

/**
 * @brief Creates a function parameter node.
 * 
 * @param param_type The type of the parameter.
 * @param param_name The name of the parameter.
 * @return ASTNode* A pointer to the newly created parameter node.
 */
ASTNode *create_function_parameter_node(char *param_type, char *param_name);

/**
 * @brief Creates a yield statement node.
 * 
 * @param expr The expression to yield.
 * @return ASTNode* A pointer to the newly created yield node.
 */
ASTNode *create_yield_node(ASTNode *expr);

/**
 * @brief Creates a print statement node.
 * 
 * @param expr The expression to print.
 * @return ASTNode* A pointer to the newly created print node.
 */
ASTNode *create_print_node(ASTNode *expr);

/**
 * @brief Creates a function call node.
 * 
 * @param function_name The name of the function to call.
 * @param arguments The list of arguments.
 * @return ASTNode* A pointer to the newly created function call node.
 */
ASTNode *create_function_call_node(char *function_name, ASTNode *arguments);


/**
 * @brief Creates a type cast node.
 * 
 * @param target_type The type to cast to (e.g., "int", "float", "string").
 * @param expr The expression to be cast.
 * @return ASTNode* A pointer to the newly created type cast node.
 */
ASTNode *create_type_cast_node(const char *target_type, ASTNode *expr);

/**
 * @brief Creates a compound assignment node (e.g., +=, -=, etc.).
 * 
 * @param var_name The name of the variable being modified.
 * @param right The right-hand expression.
 * @param operator The compound operator (e.g., "+=").
 * @return ASTNode* A pointer to the newly created compound assignment node.
 */
ASTNode *create_compound_assign_node(char *var_name, ASTNode *right, const char *operator);

#endif // AST_H
