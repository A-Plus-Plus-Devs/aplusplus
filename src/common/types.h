#ifndef TYPES_H
#define TYPES_H

typedef enum {
    INT_TYPE,
    DECIMAL_TYPE,
    STRING_TYPE,
    BOOL_TYPE,
    CHAR_TYPE,
    NOTHING_TYPE,  // For void functions
    EMPTY_TYPE,    // For null/None values
    ARRAY_TYPE     // Add this line
} VariableType;

#endif // TYPES_H
