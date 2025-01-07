#include <stdlib.h>  // This includes the standard library for functions like malloc and free
#include <stdio.h>   // This includes the standard input/output library
#include <string.h>  // This includes the string manipulation library
#include "ast.h"     // This includes our custom Abstract Syntax Tree (AST) header file

/*
 * Abstract Syntax Tree Implementation
 * 
 * This file implements the Abstract Syntax Tree (AST) data structure used to
 * represent the syntactic structure of A++ programs.
 * 
 * Key features:
 * - Node creation for various language constructs
 * - Memory management for AST nodes
 * - Support for expressions, statements, and declarations
 * - Tree traversal and manipulation utilities
 *
 * Original Author: Paul Kabulu
 * Created: August 2024
 * 
 * Edited by:
 *
 * File: src/ast/ast.c
 */

// This function creates a new AST node
ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right, const char *value)
{
    // Allocate memory for a new ASTNode
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    
    // Set the type of the node (e.g., variable declaration, print statement, etc.)
    node->type = type;
    
    // Set the left child of the node
    node->left = left;
    
    // Set the right child of the node
    node->right = right;

    node->else_branch = NULL;
    node->elseif_branch = NULL;
    
    // If a value was provided, make a copy of it and store it in the node
    // If no value was provided, set it to NULL
    node->value = value ? strdup(value) : NULL;
    
    // Return the newly created node
    return node;
}

// This function creates a node specifically for variable declarations
ASTNode *create_var_declaration_node(char *type, char *var_name, ASTNode *value)
{
    // Allocate memory for a new ASTNode
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    
    // Set the type of the node to variable declaration
    node->type = NODE_VAR_DECLARATION;
    
    // Store the type of the variable (e.g., "int", "string")
    node->var_type = type;
    
    // Store the name of the variable
    node->var_name = var_name;
    
    // The left child will hold the value/expression assigned to the variable
    node->left = value;
    
    // There's no right child for a variable declaration
    node->right = NULL;

    // Return the newly created node
    return node;
}

// This function creates a node specifically for assignment statements
ASTNode *create_assignment_node(char *var_name, ASTNode *value)
{
    // Allocate memory for a new ASTNode
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    
    // Set the type of the node to assignment
    node->type = NODE_ASSIGNMENT;
    
    // Make a copy of the variable name and store it
    node->var_name = strdup(var_name);
    
    // The left child will hold the value/expression being assigned to the variable
    node->left = value;
    
    // There's no right child for an assignment
    node->right = NULL;

    // Return the newly created node
    return node;
}

ASTNode *create_for_node(ASTNode *init, ASTNode *condition, ASTNode *increment, ASTNode *body)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FOR;
    node->init = init;
    node->condition = condition;
    node->increment = increment;
    node->body = body;
    node->left = NULL;
    node->right = NULL;
    node->value = NULL;
    node->next = NULL;
    return node;
}

// Create a function definition node
ASTNode *create_function_definition_node(char *return_type, char *function_name, ASTNode *parameters, ASTNode *body)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) return NULL;

    node->type = NODE_FUNCTION_DEFINITION;
    node->return_type = return_type ? strdup(return_type) : NULL;
    node->function_name = function_name ? strdup(function_name) : NULL;
    node->parameters = parameters;
    node->body = body;
    node->left = NULL;
    node->right = NULL;
    node->value = NULL;
    node->next = NULL;

    return node;
}

// Create a function parameter node
ASTNode *create_function_parameter_node(char *param_type, char *param_name)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_FUNCTION_PARAMETER;
    node->var_type = strdup(param_type);
    node->var_name = strdup(param_name);
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    node->value = NULL;
    
    return node;
}

// Create a yield statement node
ASTNode *create_yield_node(ASTNode *expr)
{
    ASTNode *node = create_node(NODE_YIELD_STATEMENT, NULL, NULL, NULL);
    node->yield_expr = expr;
    return node;
}

// Creates a function call node
ASTNode *create_function_call_node(char *function_name, ASTNode *arguments)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FUNCTION_CALL;
    node->function_name = strdup(function_name);
    node->arguments = arguments;  // Use arguments field for function call arguments
    node->parameters = NULL;      // Parameters is only for function definitions
    node->left = NULL;
    node->right = NULL;
    node->value = NULL;
    node->next = NULL;
    return node;
}

// Create a print statement node
ASTNode *create_print_node(ASTNode *expr)
{
    ASTNode *node = create_node(NODE_PRINT, expr, NULL, NULL);
    return node;
}

// Create an array declaration node
ASTNode *create_array_declaration_node(char *array_type, char *var_name, ASTNode *elements)
{
    ASTNode *node = create_node(NODE_ARRAY_DECLARATION, NULL, NULL, NULL);
    node->array_type = strdup(array_type);
    node->var_name = strdup(var_name);
    node->elements = elements;
    return node;
}

// Create an array access node
ASTNode *create_array_access_node(char *array_name, ASTNode *index)
{
    ASTNode *node = create_node(NODE_ARRAY_ACCESS, NULL, NULL, NULL);
    node->var_name = strdup(array_name);
    node->index = index;
    return node;
}

// Create an array method call node
ASTNode *create_array_method_call_node(char *array_name, char *method_name, ASTNode *argument)
{
    ASTNode *node = create_node(NODE_ARRAY_METHOD_CALL, NULL, NULL, NULL);
    node->var_name = strdup(array_name);
    node->method_name = strdup(method_name);
    node->right = argument;  // Store argument in right child
    return node;
}

// Create an array literal node
ASTNode *create_array_literal_node(ASTNode *elements)
{
    ASTNode *node = create_node(NODE_ARRAY_LITERAL, NULL, NULL, NULL);
    node->elements = elements;
    return node;
}

// This function frees the memory allocated for an AST
void free_ast(ASTNode *node)
{
    // If the node is NULL, there's nothing to free
    if (node)
    {
        // Recursively free the left subtree
        free_ast(node->left);
        
        // Recursively free the right subtree
        free_ast(node->right);
        
        // Free the value if it exists
        free(node->value);
        
        // Free the variable type if it exists
        free(node->var_type);

        // Free function-specific fields
        free(node->return_type);
        free(node->function_name);
        free_ast(node->parameters);
        free_ast(node->body);
        free_ast(node->yield_expr);

        free_ast(node->init);       
        free_ast(node->condition);  
        free_ast(node->increment);  

        free_ast(node->else_branch);  
        free_ast(node->elseif_branch);
        
        // Free the variable name if it exists
        free(node->var_name);
        
        // Recursively free the next node in the list
        free_ast(node->next);
        
        // Free array-specific fields
        free(node->array_type);
        free_ast(node->elements);
        free(node->method_name);
        free_ast(node->index);
        
        // Finally, free the node itself
        free(node);
    }
}

ASTNode *create_type_cast_node(const char *target_type, ASTNode *expr)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_TYPE_CAST;
    node->target_type = target_type ? strdup(target_type) : NULL;
    node->left = expr;
    node->right = NULL;
    node->value = NULL;
    node->next = NULL;
    
    return node;
}

ASTNode *create_compound_assign_node(char *var_name, ASTNode *right, const char *operator)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_COMPOUND_ASSIGN;
    node->var_name = strdup(var_name);
    node->right = right;
    node->value = strdup(operator);  // Store the operator type
    node->left = NULL;
    node->next = NULL;
    
    return node;
}
