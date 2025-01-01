#ifndef TYPES_H
#define TYPES_H

typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    BOOL_TYPE,
    CHAR_TYPE,
    NOTHING_TYPE,  // For void functions
    EMPTY_TYPE     // For null/None values
} VariableType;

#endif // TYPES_H
