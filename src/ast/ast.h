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
    NODE_ARRAY_DECLARATION,
    NODE_ARRAY_ACCESS,
    NODE_ARRAY_METHOD_CALL,
    NODE_ARRAY_LITERAL
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

    // Array-specific fields
    char *array_type;           // Type of array elements
    struct ASTNode *elements;   // Linked list of array elements
    struct ASTNode *index;      // Index for array access
    char *method_name;          // Name of array method being called

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
 * @brief Creates an array declaration node.
 * 
 * @param array_type The type of array elements (e.g., "string", "int").
 * @param var_name The name of the array variable.
 * @param elements The initial elements of the array.
 * @return ASTNode* A pointer to the newly created array declaration node.
 */
ASTNode *create_array_declaration_node(char *array_type, char *var_name, ASTNode *elements);

/**
 * @brief Creates an array access node.
 * 
 * @param array_name The name of the array being accessed.
 * @param index The index expression.
 * @return ASTNode* A pointer to the newly created array access node.
 */
ASTNode *create_array_access_node(char *array_name, ASTNode *index);

/**
 * @brief Creates an array method call node.
 * 
 * @param array_name The name of the array.
 * @param method_name The name of the method being called.
 * @param argument Optional argument for the method (can be NULL).
 * @return ASTNode* A pointer to the newly created array method call node.
 */
ASTNode *create_array_method_call_node(char *array_name, char *method_name, ASTNode *argument);

/**
 * @brief Creates an array literal node.
 * 
 * @param elements Linked list of array elements.
 * @return ASTNode* A pointer to the newly created array literal node.
 */
ASTNode *create_array_literal_node(ASTNode *elements);

#endif // AST_H
