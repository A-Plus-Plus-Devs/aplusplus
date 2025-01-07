#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEBUG_LOG(msg, ...) printf("[DEBUG] %s:%d - " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

#define INITIAL_CAPACITY 8

ArrayValue* create_array(VariableType type, int is_mixed) {
    DEBUG_LOG("Creating array of type %d, is_mixed: %d", type, is_mixed);
    
    ArrayValue* array = malloc(sizeof(ArrayValue));
    if (!array) {
        DEBUG_LOG("Failed to allocate ArrayValue struct");
        return NULL;
    }
    
    array->elements = malloc(INITIAL_CAPACITY * sizeof(void*));
    if (!array->elements) {
        DEBUG_LOG("Failed to allocate elements array");
        free(array);
        return NULL;
    }
    
    array->length = 0;
    array->capacity = INITIAL_CAPACITY;
    array->type = type;
    array->is_mixed = is_mixed;
    
    DEBUG_LOG("Successfully created array at %p", (void*)array);
    return array;
}

static void ensure_capacity(ArrayValue* array) {
    DEBUG_LOG("Checking capacity for array at %p", (void*)array);
    if (!array) {
        DEBUG_LOG("Array is NULL!");
        return;
    }
    
    if (array->length >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        DEBUG_LOG("Expanding capacity from %zu to %zu", array->capacity, new_capacity);
        
        void** new_elements = realloc(array->elements, new_capacity * sizeof(void*));
        if (!new_elements) {
            DEBUG_LOG("Failed to reallocate array");
            return;
        }
        
        array->elements = new_elements;
        array->capacity = new_capacity;
        DEBUG_LOG("Successfully expanded array");
    }
}

void array_add_last(ArrayValue* array, void* element) {
    DEBUG_LOG("Adding element %p to end of array %p", element, (void*)array);
    if (!array) {
        DEBUG_LOG("Array is NULL!");
        return;
    }
    
    ensure_capacity(array);
    array->elements[array->length] = element;
    array->length++;
    DEBUG_LOG("Successfully added element, new length: %zu", array->length);
}

void* array_remove_last(ArrayValue* array) {
    DEBUG_LOG("Removing last element from array %p", (void*)array);
    if (!array || array->length == 0) {
        DEBUG_LOG("Array is NULL or empty");
        return NULL;
    }
    
    void* element = array->elements[--array->length];
    DEBUG_LOG("Removed element %p, new length: %zu", element, array->length);
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
    DEBUG_LOG("Freeing array at %p", (void*)array);
    if (!array) {
        DEBUG_LOG("Array is NULL");
        return;
    }
    
    if (array->type == STRING_TYPE) {
        DEBUG_LOG("Freeing string elements");
        for (size_t i = 0; i < array->length; i++) {
            if (array->elements[i]) {
                DEBUG_LOG("Freeing string at index %zu", i);
                free(array->elements[i]);
            }
        }
    }
    
    DEBUG_LOG("Freeing elements array");
    free(array->elements);
    DEBUG_LOG("Freeing array struct");
    free(array);
    DEBUG_LOG("Array freed successfully");
}

void debug_print_array(ArrayValue* array) {
    if (!array) {
        printf("[DEBUG] Array is NULL\n");
        return;
    }
    
    printf("[DEBUG] Array at %p:\n", (void*)array);
    printf("  Type: %d\n", array->type);
    printf("  Is Mixed: %d\n", array->is_mixed);
    printf("  Length: %zu\n", array->length);
    printf("  Capacity: %zu\n", array->capacity);
    printf("  Elements:\n");
    
    for (size_t i = 0; i < array->length; i++) {
        printf("    [%zu] = %p", i, array->elements[i]);
        if (array->type == STRING_TYPE && array->elements[i]) {
            printf(" (\"%s\")", (char*)array->elements[i]);
        } else if (array->type == INT_TYPE && array->elements[i]) {
            printf(" (%d)", *(int*)array->elements[i]);
        }
        printf("\n");
    }
}

void array_insert(ArrayValue* array, size_t index, void* element) {
    DEBUG_LOG("Inserting element at index %zu", index);
    
    if (!array) {
        DEBUG_LOG("Array is NULL!");
        return;
    }
    
    if (index > array->length) {
        DEBUG_LOG("Index out of bounds: %zu (length: %zu)", index, array->length);
        return;
    }
    
    ensure_capacity(array);
    
    // Shift elements to make room for new element
    if (index < array->length) {
        memmove(&array->elements[index + 1], 
                &array->elements[index], 
                (array->length - index) * sizeof(void*));
    }
    
    array->elements[index] = element;
    array->length++;
    
    DEBUG_LOG("Successfully inserted element, new length: %zu", array->length);
} 