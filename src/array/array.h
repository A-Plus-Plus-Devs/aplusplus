#ifndef ARRAY_H
#define ARRAY_H

#include "../common/types.h"
#include <stddef.h>
#include <stdbool.h>

// Initial capacity for new arrays
#define ARRAY_INITIAL_CAPACITY 8

// Array element union to support multiple types
typedef union {
    int int_value;
    double float_value;
    char *string_value;
    char char_value;
    bool bool_value;
} ArrayElement;

// Array structure
typedef struct {
    ArrayElement *elements;     // Dynamic array of elements
    VariableType element_type;  // Type of elements in the array
    size_t length;             // Current number of elements
    size_t capacity;           // Current capacity
} Array;

// Core array functions
Array *array_create(VariableType type);
void array_free(Array *arr);
bool array_resize(Array *arr, size_t new_capacity);

// Array operations
bool array_add_last(Array *arr, ArrayElement element);
bool array_add_first(Array *arr, ArrayElement element);
bool array_remove_last(Array *arr);
bool array_remove_first(Array *arr);
size_t array_length(Array *arr);
ArrayElement array_get(Array *arr, size_t index);
bool array_set(Array *arr, size_t index, ArrayElement value);

// Type checking and conversion
bool array_is_valid_type(VariableType type);
ArrayElement array_convert_value(VariableType from_type, VariableType to_type, ArrayElement value);

#endif // ARRAY_H 