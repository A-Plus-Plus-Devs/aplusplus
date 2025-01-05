#ifndef ARRAY_AST_H
#define ARRAY_AST_H

#include "../common/types.h"
#include <stdbool.h>
#include "array.h"

// Array-specific AST node types
typedef enum {
    ARRAY_NODE_DECLARATION,    // Array declaration: numbers<int> = [1, 2, 3]
    ARRAY_NODE_LITERAL,        // Array literal: [1, 2, 3]
    ARRAY_NODE_ACCESS,         // Array access: arr[0]
    ARRAY_NODE_ADD_LAST,       // arr.addLast(value)
    ARRAY_NODE_ADD_FIRST,      // arr.addFirst(value)
    ARRAY_NODE_REMOVE_LAST,    // arr.removeLast()
    ARRAY_NODE_REMOVE_FIRST,   // arr.removeFirst()
    ARRAY_NODE_LENGTH,         // arr.length
    ARRAY_NODE_ELEMENT,        // Individual element in array literal
    ARRAY_NODE_IDENTIFIER,     // Array variable name
    ARRAY_NODE_METHOD_CALL,   // For method calls
    ARRAY_NODE_PRINT,         // For print statements
    ARRAY_NODE_ASSIGNMENT     // For assignments
} ArrayNodeType;

// Value union for AST nodes
typedef union {
    int int_value;
    double float_value;
    char *string_value;
    bool bool_value;
    char char_value;
} ArrayValue;

// AST node structure
typedef struct ArrayASTNode {
    ArrayNodeType type;
    char *array_name;
    char *var_name;           // For length assignment
    VariableType element_type;
    ArrayValue value;         // Changed from void* to ArrayValue
    struct ArrayASTNode *children;
    struct ArrayASTNode *next;
    struct ArrayASTNode *index;
    struct ArrayASTNode *argument;
} ArrayASTNode;

// Node creation functions
ArrayASTNode *array_create_declaration_node(const char *name, VariableType type, ArrayASTNode *elements);
ArrayASTNode *array_create_literal_node(VariableType type, void *value);
ArrayASTNode *array_create_access_node(const char *name, ArrayASTNode *index);
ArrayASTNode *array_create_method_node(ArrayNodeType type, const char *name, ArrayASTNode *argument);
ArrayASTNode *array_create_element_node(VariableType type, void *value);
ArrayASTNode *array_create_identifier_node(const char *name);
ArrayASTNode *array_create_length_node(const char *array_name, const char *var_name);
ArrayASTNode *array_create_print_node(ArrayASTNode *expr);
ArrayASTNode *array_create_assignment_node(const char *name, ArrayASTNode *value);

// Utility functions
void array_free_ast(ArrayASTNode *node);
char *array_ast_to_string(ArrayASTNode *node);
ArrayASTNode *array_copy_ast(ArrayASTNode *node);

#endif // ARRAY_AST_H 