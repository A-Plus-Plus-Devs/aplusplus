#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../ast/ast.h"
#include "../array/array_interpreter.h"

// Value type definition
typedef struct Value {
    enum {
        VALUE_NULL,
        VALUE_INT,
        VALUE_FLOAT,
        VALUE_STRING,
        VALUE_BOOL,
        VALUE_CHAR,
        VALUE_ARRAY
    } type;
    
    union {
        int int_value;
        double float_value;
        char *string_value;
        bool bool_value;
        char char_value;
        Array *array_value;
    } data;
} Value;

// Interpreter structure
typedef struct {
    // ... existing interpreter fields ...
    ArrayInterpreter *array_interpreter; // Add this field
} Interpreter;

// Function declarations
Value create_null_value(void);
Value convert_array_result_to_value(ArrayInterpretResult result);

/**
 * @brief Interprets and executes the given Abstract Syntax Tree.
 * 
 * This function walks through the AST, executing each node according to its type.
 * It handles variable declarations, assignments, and print statements.
 * 
 * @param node The root node of the AST to be interpreted.
 */
void interpret(ASTNode *node);

#endif // INTERPRETER_H
