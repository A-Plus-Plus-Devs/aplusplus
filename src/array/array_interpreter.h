#ifndef ARRAY_INTERPRETER_H
#define ARRAY_INTERPRETER_H

#include "array_ast.h"
#include "array.h"

// Result structure for interpreter operations
typedef struct {
    bool success;
    union {
        int int_value;
        float float_value;
        bool bool_value;
        char char_value;
        char *string_value;
        Array *array_value;
        ArrayElement element;
    } value;
} ArrayInterpretResult;

// Define the actual structure (not typedef since it's already forward declared in array.h)
struct ArrayInterpreter {
    Array **arrays;           // Array of array pointers
    char **array_names;       // Array of array names
    size_t array_count;       // Number of arrays
    size_t array_capacity;    // Capacity of arrays array
};

// Interpreter functions
ArrayInterpreter *array_interpreter_init(void);
void array_interpreter_free(ArrayInterpreter *interpreter);
ArrayInterpretResult array_interpret(ArrayInterpreter *interpreter, ArrayASTNode *node);

// Error handling
void array_interpreter_error(const char *message);

#endif // ARRAY_INTERPRETER_H 