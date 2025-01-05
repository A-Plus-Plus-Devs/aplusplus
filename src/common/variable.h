#ifndef VARIABLE_H
#define VARIABLE_H

#include "types.h"
#include <stdbool.h>

// This structure represents a variable in our program
typedef struct Variable {
    char *name;        // The name of the variable
    VariableType type; // The type of the variable
    union {
        int int_value;
        double float_value;
        char *string_value;
        bool bool_value;
        char char_value;
    } value;
} Variable;

// Function declaration
Variable *get_variable(const char *name);

#endif // VARIABLE_H 