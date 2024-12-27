#include <stdlib.h>  // This includes the standard library for functions like malloc and free
#include <stdio.h>   // This includes the standard input/output library
#include <string.h>  // This includes the string manipulation library
#include "ast.h"     // This includes our custom Abstract Syntax Tree (AST) header file

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

        free_ast(node->init);       
        free_ast(node->condition);  
        free_ast(node->increment);  
        free_ast(node->body);   

        free_ast(node->else_branch);  
        free_ast(node->elseif_branch);
        
        // Free the variable name if it exists
        free(node->var_name);
        
        // Recursively free the next node in the list
        free_ast(node->next);
        
        // Finally, free the node itself
        free(node);
    }
}
