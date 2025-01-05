#include "array.h"
#include "array_lexer.h"
#include "array_parser.h"
#include "array_interpreter.h"
#include "array_ast.h"
#include <stdlib.h>
#include <string.h>

Array *array_create(VariableType type) {
    Array *arr = malloc(sizeof(Array));
    if (!arr) return NULL;

    arr->elements = malloc(ARRAY_INITIAL_CAPACITY * sizeof(ArrayElement));
    if (!arr->elements) {
        free(arr);
        return NULL;
    }

    arr->element_type = type;
    arr->length = 0;
    arr->capacity = ARRAY_INITIAL_CAPACITY;
    return arr;
}

void array_free(Array *arr) {
    if (!arr) return;

    if (arr->element_type == STRING_TYPE) {
        // Free all string elements
        for (size_t i = 0; i < arr->length; i++) {
            free(arr->elements[i].string_value);
        }
    }
    
    free(arr->elements);
    free(arr);
}

bool array_resize(Array *arr, size_t new_capacity) {
    if (!arr || new_capacity < arr->length) return false;

    ArrayElement *new_elements = realloc(arr->elements, new_capacity * sizeof(ArrayElement));
    if (!new_elements) return false;

    arr->elements = new_elements;
    arr->capacity = new_capacity;
    return true;
}

bool array_add_last(Array *arr, ArrayElement element) {
    if (!arr) return false;

    if (arr->length >= arr->capacity) {
        if (!array_resize(arr, arr->capacity * 2)) {
            return false;
        }
    }

    if (arr->element_type == STRING_TYPE && element.string_value) {
        arr->elements[arr->length].string_value = strdup(element.string_value);
        if (!arr->elements[arr->length].string_value) return false;
    } else {
        arr->elements[arr->length] = element;
    }

    arr->length++;
    return true;
}

bool array_add_first(Array *arr, ArrayElement element) {
    if (!arr) return false;

    if (arr->length >= arr->capacity) {
        if (!array_resize(arr, arr->capacity * 2)) {
            return false;
        }
    }

    // Shift elements right
    for (size_t i = arr->length; i > 0; i--) {
        arr->elements[i] = arr->elements[i - 1];
    }

    if (arr->element_type == STRING_TYPE && element.string_value) {
        arr->elements[0].string_value = strdup(element.string_value);
        if (!arr->elements[0].string_value) return false;
    } else {
        arr->elements[0] = element;
    }

    arr->length++;
    return true;
}

bool array_remove_last(Array *arr) {
    if (!arr || arr->length == 0) return false;

    if (arr->element_type == STRING_TYPE) {
        free(arr->elements[arr->length - 1].string_value);
    }

    arr->length--;

    // Shrink array if it's too empty
    if (arr->length < arr->capacity / 4 && arr->capacity > ARRAY_INITIAL_CAPACITY) {
        array_resize(arr, arr->capacity / 2);
    }

    return true;
}

bool array_remove_first(Array *arr) {
    if (!arr || arr->length == 0) return false;

    if (arr->element_type == STRING_TYPE) {
        free(arr->elements[0].string_value);
    }

    // Shift elements left
    for (size_t i = 0; i < arr->length - 1; i++) {
        arr->elements[i] = arr->elements[i + 1];
    }

    arr->length--;

    // Shrink array if it's too empty
    if (arr->length < arr->capacity / 4 && arr->capacity > ARRAY_INITIAL_CAPACITY) {
        array_resize(arr, arr->capacity / 2);
    }

    return true;
}

size_t array_length(Array *arr) {
    return arr ? arr->length : 0;
}

ArrayElement array_get(Array *arr, size_t index) {
    ArrayElement empty = {0};
    if (!arr || index >= arr->length) return empty;
    return arr->elements[index];
}

bool array_set(Array *arr, size_t index, ArrayElement value) {
    if (!arr || index >= arr->length) return false;

    if (arr->element_type == STRING_TYPE) {
        free(arr->elements[index].string_value);
        if (value.string_value) {
            arr->elements[index].string_value = strdup(value.string_value);
            return arr->elements[index].string_value != NULL;
        }
    }

    arr->elements[index] = value;
    return true;
}

bool array_process_file(const char *source) {
    // Initialize components
    ArrayLexer *array_lexer = array_lexer_init(source);
    if (!array_lexer) return false;
    
    ArrayParser *array_parser = array_parser_init(array_lexer);
    if (!array_parser) {
        array_lexer_free(array_lexer);
        return false;
    }
    
    ArrayInterpreter *array_interpreter = array_interpreter_init();
    if (!array_interpreter) {
        array_parser_free(array_parser);
        array_lexer_free(array_lexer);
        return false;
    }
    
    // Parse and interpret
    ArrayASTNode *array_ast = array_parse(array_parser);
    if (array_ast) {
        ArrayInterpretResult result = array_interpret(array_interpreter, array_ast);
        
        // Cleanup
        array_free_ast(array_ast);
        array_interpreter_free(array_interpreter);
        array_parser_free(array_parser);
        array_lexer_free(array_lexer);
        
        return result.success;
    }
    
    return false;
} 