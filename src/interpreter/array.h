#ifndef ARRAY_H
#define ARRAY_H

#include "common/types.h"
#include <stddef.h>

typedef struct ArrayValue {
    void **elements;        // Dynamic array of element pointers
    size_t length;         // Current number of elements
    size_t capacity;       // Current capacity
    VariableType type;     // Type of array elements (INT_TYPE, STRING_TYPE, etc.)
    int is_mixed;          // Flag for any[] arrays
} ArrayValue;

// Array operations
ArrayValue* create_array(VariableType type, int is_mixed);
void array_add_last(ArrayValue* array, void* element);
void* array_remove_last(ArrayValue* array);
void array_add_first(ArrayValue* array, void* element);
void* array_remove_first(ArrayValue* array);
void* array_get(ArrayValue* array, size_t index);
void array_set(ArrayValue* array, size_t index, void* element);
void free_array(ArrayValue* array);
void debug_print_array(ArrayValue* array);
void array_insert(ArrayValue* array, size_t index, void* element);

#endif // ARRAY_H 