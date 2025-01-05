#ifndef ARRAY_INTERPRETER_H
#define ARRAY_INTERPRETER_H

#include "array_ast.h"
#include "array.h"

// Result structure for interpreter operations
typedef struct {
    bool success;
    union {
        Array *array_value;
        int int_value;
        double float_value;
        char *string_value;
        bool bool_value;
        char char_value;
        ArrayElement element;
    } value;
} ArrayInterpretResult;

// Interpreter structure to maintain state
typedef struct {
    Array **arrays;           // Array of array pointers
    char **array_names;       // Array of array names
    size_t array_count;       // Number of arrays
    size_t array_capacity;    // Capacity of arrays array
} ArrayInterpreter;

// Interpreter functions
ArrayInterpreter *array_interpreter_init(void);
void array_interpreter_free(ArrayInterpreter *interpreter);
ArrayInterpretResult array_interpret(ArrayInterpreter *interpreter, ArrayASTNode *node);

// Error handling
void array_interpreter_error(const char *message);

#endif // ARRAY_INTERPRETER_H 