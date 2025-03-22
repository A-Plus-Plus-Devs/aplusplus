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

#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include "ast.h"   


ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right, const char *value)
{

    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    

    node->type = type;
    

    node->left = left;
    

    node->right = right;

    node->else_branch = NULL;
    node->elseif_branch = NULL;
    


    node->value = value ? strdup(value) : NULL;
    

    return node;
}

ASTNode *create_var_declaration_node(char *type, char *var_name, ASTNode *value)
{

    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    

    node->type = NODE_VAR_DECLARATION;
    

    node->var_type = type;
    

    node->var_name = var_name;
    

    node->left = value;
    

    node->right = NULL;


    return node;
}

ASTNode *create_assignment_node(char *var_name, ASTNode *value)
{

    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    

    node->type = NODE_ASSIGNMENT;
    

    node->var_name = strdup(var_name);
    

    node->left = value;
    

    node->right = NULL;


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
ASTNode *create_yield_node(ASTNode *expr)
{
    ASTNode *node = create_node(NODE_YIELD_STATEMENT, NULL, NULL, NULL);
    node->yield_expr = expr;
    return node;
}
ASTNode *create_function_call_node(char *function_name, ASTNode *arguments)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FUNCTION_CALL;
    node->function_name = strdup(function_name);
    node->arguments = arguments;
    node->parameters = NULL;    
    node->left = NULL;
    node->right = NULL;
    node->value = NULL;
    node->next = NULL;
    return node;
}

ASTNode *create_print_node(ASTNode *expr)
{
    ASTNode *node = create_node(NODE_PRINT, expr, NULL, NULL);
    return node;
}

ASTNode *create_array_declaration_node(char *array_type, char *var_name, ASTNode *elements)
{
    ASTNode *node = create_node(NODE_ARRAY_DECLARATION, NULL, NULL, NULL);
    node->array_type = strdup(array_type);
    node->var_name = strdup(var_name);
    node->elements = elements;
    return node;
}

ASTNode *create_array_access_node(char *array_name, ASTNode *index)
{
    ASTNode *node = create_node(NODE_ARRAY_ACCESS, NULL, NULL, NULL);
    node->var_name = strdup(array_name);
    node->index = index;
    return node;
}

ASTNode *create_array_method_call_node(char *array_name, char *method_name, ASTNode *argument)
{
    ASTNode *node = create_node(NODE_ARRAY_METHOD_CALL, NULL, NULL, NULL);
    node->var_name = strdup(array_name);
    node->method_name = strdup(method_name);
    node->right = argument; 
    return node;
}

ASTNode *create_array_literal_node(ASTNode *elements)
{
    ASTNode *node = create_node(NODE_ARRAY_LITERAL, NULL, NULL, NULL);
    node->elements = elements;
    return node;
}

void free_ast(ASTNode *node)
{
    if (node)
    {
        free_ast(node->left);
        
        free_ast(node->right);
        
        free(node->value);
        
        free(node->var_type);

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
        
        free(node->var_name);
        
        free_ast(node->next);
        
        free(node->array_type);
        free_ast(node->elements);
        free(node->method_name);
        free_ast(node->index);
        
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
    node->value = strdup(operator);
    node->left = NULL;
    node->next = NULL;
    
    return node;
}
