#include "array_ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ArrayASTNode *array_create_declaration_node(const char *name, VariableType type, ArrayASTNode *elements) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_DECLARATION;
    node->array_name = strdup(name);
    node->element_type = type;
    node->children = elements;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;

    return node;
}

ArrayASTNode *array_create_literal_node(VariableType type, void *value) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_LITERAL;
    node->element_type = type;
    node->array_name = NULL;
    node->children = NULL;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;

    // Copy the value based on type
    switch (type) {
        case INT_TYPE:
            node->value.int_value = *(int *)value;
            break;
        case FLOAT_TYPE:
            node->value.float_value = *(double *)value;
            break;
        case STRING_TYPE:
            node->value.string_value = strdup((char *)value);
            break;
        case BOOL_TYPE:
            node->value.bool_value = *(bool *)value;
            break;
        case CHAR_TYPE:
            node->value.char_value = *(char *)value;
            break;
        default:
            free(node);
            return NULL;
    }

    return node;
}

ArrayASTNode *array_create_access_node(const char *array_name, ArrayASTNode *index) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_ACCESS;
    node->array_name = strdup(array_name);
    node->index = index;
    node->children = NULL;
    node->next = NULL;
    node->argument = NULL;

    return node;
}

ArrayASTNode *array_create_method_node(ArrayNodeType method_type, const char *array_name, ArrayASTNode *argument) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = method_type;
    node->array_name = strdup(array_name);
    node->argument = argument;
    node->children = NULL;
    node->next = NULL;
    node->index = NULL;

    return node;
}

ArrayASTNode *array_create_element_node(VariableType type, void *value) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_ELEMENT;
    node->element_type = type;
    node->array_name = NULL;
    node->var_name = NULL;
    node->children = NULL;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;

    // Copy the value based on type
    switch (type) {
        case INT_TYPE:
            node->value.int_value = *(int *)value;
            break;
        case FLOAT_TYPE:
            node->value.float_value = *(double *)value;
            break;
        case STRING_TYPE:
            node->value.string_value = strdup((char *)value);
            break;
        case BOOL_TYPE:
            node->value.bool_value = *(bool *)value;
            break;
        case CHAR_TYPE:
            node->value.char_value = *(char *)value;
            break;
        default:
            free(node);
            return NULL;
    }

    return node;
}

ArrayASTNode *array_create_identifier_node(const char *name) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_IDENTIFIER;
    node->array_name = strdup(name);
    node->children = NULL;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;

    return node;
}

ArrayASTNode *array_create_length_node(const char *array_name, const char *var_name) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_LENGTH;
    node->array_name = strdup(array_name);
    node->var_name = strdup(var_name);
    node->children = NULL;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;
    node->element_type = INT_TYPE;  // Changed from TYPE_INT to INT_TYPE
    memset(&node->value, 0, sizeof(ArrayValue));  // Initialize value union to 0

    return node;
}

ArrayASTNode *array_create_print_node(ArrayASTNode *expr) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_PRINT;
    node->array_name = NULL;
    node->var_name = NULL;
    node->children = expr;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;
    memset(&node->value, 0, sizeof(ArrayValue));

    return node;
}

ArrayASTNode *array_create_assignment_node(const char *name, ArrayASTNode *value) {
    ArrayASTNode *node = malloc(sizeof(ArrayASTNode));
    if (!node) return NULL;

    node->type = ARRAY_NODE_ASSIGNMENT;
    node->array_name = strdup(name);
    node->var_name = NULL;
    node->children = value;
    node->next = NULL;
    node->index = NULL;
    node->argument = NULL;
    memset(&node->value, 0, sizeof(ArrayValue));

    return node;
}

void array_free_ast(ArrayASTNode *node) {
    if (!node) return;

    array_free_ast(node->children);
    array_free_ast(node->next);
    array_free_ast(node->index);
    array_free_ast(node->argument);

    if (node->array_name) free(node->array_name);
    if (node->var_name) free(node->var_name);
    if (node->type == ARRAY_NODE_ELEMENT && 
        node->element_type == STRING_TYPE && 
        node->value.string_value) {
        free(node->value.string_value);
    }

    free(node);
}

char *array_ast_to_string(ArrayASTNode *node) {
    if (!node) return strdup("NULL");

    char buffer[1024];
    
    switch (node->type) {
        case ARRAY_NODE_DECLARATION:
            snprintf(buffer, sizeof(buffer), "ArrayDeclaration(%s, type=%d)", 
                    node->array_name, node->element_type);
            break;
        case ARRAY_NODE_LITERAL:
            snprintf(buffer, sizeof(buffer), "ArrayLiteral[...]");
            break;
        case ARRAY_NODE_ACCESS:
            snprintf(buffer, sizeof(buffer), "ArrayAccess(%s[...])", 
                    node->array_name);
            break;
        case ARRAY_NODE_ADD_LAST:
            snprintf(buffer, sizeof(buffer), "AddLast(%s, ...)", 
                    node->array_name);
            break;
        case ARRAY_NODE_ADD_FIRST:
            snprintf(buffer, sizeof(buffer), "AddFirst(%s, ...)", 
                    node->array_name);
            break;
        case ARRAY_NODE_REMOVE_LAST:
            snprintf(buffer, sizeof(buffer), "RemoveLast(%s)", 
                    node->array_name);
            break;
        case ARRAY_NODE_REMOVE_FIRST:
            snprintf(buffer, sizeof(buffer), "RemoveFirst(%s)", 
                    node->array_name);
            break;
        case ARRAY_NODE_LENGTH:
            snprintf(buffer, sizeof(buffer), "Length(%s)", 
                    node->array_name);
            break;
        case ARRAY_NODE_METHOD_CALL:
            snprintf(buffer, sizeof(buffer), "MethodCall(%s)", 
                    node->array_name);
            break;
        case ARRAY_NODE_PRINT:
            snprintf(buffer, sizeof(buffer), "Print(...)");
            break;
        case ARRAY_NODE_ASSIGNMENT:
            snprintf(buffer, sizeof(buffer), "Assignment(%s)", 
                    node->array_name);
            break;
        default:
            snprintf(buffer, sizeof(buffer), "Unknown node type");
            break;
    }

    return strdup(buffer);
}

ArrayASTNode *array_copy_ast(ArrayASTNode *node) {
    if (!node) return NULL;

    ArrayASTNode *new_node = malloc(sizeof(ArrayASTNode));
    if (!new_node) return NULL;

    // Copy basic fields
    *new_node = *node;

    // Deep copy strings and pointers
    if (node->array_name) {
        new_node->array_name = strdup(node->array_name);
    }
    if (node->type == ARRAY_NODE_ELEMENT && 
        node->element_type == STRING_TYPE && 
        node->value.string_value) {
        new_node->value.string_value = strdup(node->value.string_value);
    }

    // Recursively copy child nodes
    new_node->children = array_copy_ast(node->children);
    new_node->next = array_copy_ast(node->next);
    new_node->index = array_copy_ast(node->index);
    new_node->argument = array_copy_ast(node->argument);

    return new_node;
} 