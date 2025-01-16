/*
 * Array Implementation
 *
 * This file contains the core implementation of dynamic arrays for the A++ language,
 * providing fundamental array operations and memory management.
 *
 * Key features:
 * - Dynamic array creation and memory management
 * - Array resizing and capacity handling
 * - Element insertion and removal operations
 * - Support for multiple data types
 * - Debug utilities for array inspection
 *
 * The array implementation supports:
 * - Adding/removing elements at start/end
 * - Inserting elements at specific positions
 * - Automatic capacity management
 * - Type-safe element storage
 * - Memory cleanup and deallocation
 *
 * Original Author: Paul Kabulu
 * Created: January 2025
 *
 * Edited by:
 *
 *
 * File: src/interpreter/array.c
 */

#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG_LOG(msg, ...) printf("[DEBUG] %s:%d - " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

#define INITIAL_CAPACITY 8

ArrayValue* create_array(VariableType type, int is_mixed) {
    ArrayValue* array = malloc(sizeof(ArrayValue));
    if (!array) {
        return NULL;
    }
    
    array->elements = malloc(INITIAL_CAPACITY * sizeof(void*));
    if (!array->elements) {
        free(array);
        return NULL;
    }
    
    array->length = 0;
    array->capacity = INITIAL_CAPACITY;
    array->type = type;
    array->is_mixed = is_mixed;
    
    return array;
}

static void ensure_capacity(ArrayValue* array) {
    if (!array) {
        return;
    }
    
    if (array->length >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        
        void** new_elements = realloc(array->elements, new_capacity * sizeof(void*));
        if (!new_elements) {
            return;
        }
        
        array->elements = new_elements;
        array->capacity = new_capacity;
    }
}

void array_add_last(ArrayValue* array, void* element) {
    ensure_capacity(array);
    array->elements[array->length++] = element;
}

void* array_remove_last(ArrayValue* array) {
    if (!array || array->length == 0) {
        return NULL;
    }
    
    void* element = array->elements[--array->length];
    return element;
}

void array_add_first(ArrayValue* array, void* element) {
    ensure_capacity(array);
    memmove(&array->elements[1], array->elements, array->length * sizeof(void*));
    array->elements[0] = element;
    array->length++;
}

void* array_remove_first(ArrayValue* array) {
    if (array->length == 0) return NULL;
    void* element = array->elements[0];
    memmove(array->elements, &array->elements[1], (array->length - 1) * sizeof(void*));
    array->length--;
    return element;
}

void* array_get(ArrayValue* array, size_t index) {
    if (index >= array->length) return NULL;
    return array->elements[index];
}

void array_set(ArrayValue* array, size_t index, void* element) {
    if (index >= array->length) return;
    array->elements[index] = element;
}

void free_array(ArrayValue* array) {
    if (!array) {
        return;
    }
    
    if (array->type == STRING_TYPE) {
        for (size_t i = 0; i < array->length; i++) {
            if (array->elements[i]) {
                free(array->elements[i]);
            }
        }
    }
    
    free(array->elements);
    free(array);
}

void debug_print_array(ArrayValue* array)
{
    if (!array) {
        return;
    }

    printf("Array Info:\n");
    printf("Type: %d\n", array->type);
    printf("Length: %zu\n", array->length);
    printf("Capacity: %zu\n", array->capacity);
    
    for (size_t i = 0; i < array->length; i++) {
        void* element = array->elements[i];
        if (!element) {
            printf("[%zu]: NULL\n", i);
            continue;
        }
        
        switch (array->type) {
            case STRING_TYPE:
                printf("[%zu]: '%s'\n", i, (char*)element);
                break;
            case INT_TYPE:
                printf("[%zu]: %d\n", i, *(int*)element);
                break;
            case DECIMAL_TYPE:
                printf("[%zu]: %f\n", i, *(double*)element);
                break;
            case BOOL_TYPE:
                printf("[%zu]: %s\n", i, (*(bool*)element) ? "yup" : "nope");
                break;
            default:
                printf("[%zu]: <unknown type>\n", i);
        }
    }
    printf("=====================\n");
}

void array_insert(ArrayValue* array, size_t index, void* element) {
    if (!array) {
        return;
    }
    
    if (index > array->length) {
        return;
    }
    
    ensure_capacity(array);
    
    if (index < array->length) {
        memmove(&array->elements[index + 1], 
                &array->elements[index], 
                (array->length - index) * sizeof(void*));
    }
    
    array->elements[index] = element;
    array->length++;
}