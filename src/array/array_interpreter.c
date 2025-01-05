#include "array_interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ARRAY_CAPACITY 16

// Forward declarations of helper functions
static ArrayInterpretResult interpret_declaration(ArrayInterpreter *interpreter, ArrayASTNode *node);
static ArrayInterpretResult interpret_literal(ArrayInterpreter *interpreter, ArrayASTNode *node);
static ArrayInterpretResult interpret_method_call(ArrayInterpreter *interpreter, ArrayASTNode *node);
static ArrayInterpretResult interpret_access(ArrayInterpreter *interpreter, ArrayASTNode *node);
static Array *find_array(ArrayInterpreter *interpreter, const char *name);
static bool register_array(ArrayInterpreter *interpreter, const char *name, Array *array);
static ArrayElement convert_ast_to_element(ArrayASTNode *node);

ArrayInterpreter *array_interpreter_init(void) {
    ArrayInterpreter *interpreter = malloc(sizeof(ArrayInterpreter));
    if (!interpreter) return NULL;

    interpreter->arrays = malloc(INITIAL_ARRAY_CAPACITY * sizeof(Array *));
    interpreter->array_names = malloc(INITIAL_ARRAY_CAPACITY * sizeof(char *));
    
    if (!interpreter->arrays || !interpreter->array_names) {
        free(interpreter->arrays);
        free(interpreter->array_names);
        free(interpreter);
        return NULL;
    }

    interpreter->array_count = 0;
    interpreter->array_capacity = INITIAL_ARRAY_CAPACITY;
    return interpreter;
}

void array_interpreter_free(ArrayInterpreter *interpreter) {
    if (!interpreter) return;

    for (size_t i = 0; i < interpreter->array_count; i++) {
        array_free(interpreter->arrays[i]);
        free(interpreter->array_names[i]);
    }

    free(interpreter->arrays);
    free(interpreter->array_names);
    free(interpreter);
}

ArrayInterpretResult array_interpret(ArrayInterpreter *interpreter, ArrayASTNode *node) {
    ArrayInterpretResult error_result = {.success = false};
    
    if (!interpreter || !node) {
        array_interpreter_error("Invalid interpreter or AST node");
        return error_result;
    }

    switch (node->type) {
        case ARRAY_NODE_DECLARATION:
            return interpret_declaration(interpreter, node);
        
        case ARRAY_NODE_LITERAL:
            return interpret_literal(interpreter, node);
        
        case ARRAY_NODE_ADD_LAST:
        case ARRAY_NODE_ADD_FIRST:
        case ARRAY_NODE_REMOVE_LAST:
        case ARRAY_NODE_REMOVE_FIRST:
        case ARRAY_NODE_LENGTH:
            return interpret_method_call(interpreter, node);
        
        case ARRAY_NODE_ACCESS:
            return interpret_access(interpreter, node);
        
        default:
            array_interpreter_error("Unknown node type");
            return error_result;
    }
}

static ArrayInterpretResult interpret_declaration(ArrayInterpreter *interpreter, ArrayASTNode *node) {
    ArrayInterpretResult result = {.success = false};
    
    Array *array = array_create(node->element_type);
    if (!array) {
        array_interpreter_error("Failed to create array");
        return result;
    }

    ArrayASTNode *current = node->children;
    while (current) {
        ArrayElement element = convert_ast_to_element(current);
        if (!array_add_last(array, element)) {
            array_free(array);
            array_interpreter_error("Failed to add element to array");
            return result;
        }
        current = current->next;
    }

    if (!register_array(interpreter, node->array_name, array)) {
        array_free(array);
        array_interpreter_error("Failed to register array");
        return result;
    }

    result.success = true;
    result.value.array_value = array;
    return result;
}

static ArrayInterpretResult interpret_literal(ArrayInterpreter *interpreter, ArrayASTNode *node) {
    ArrayInterpretResult result = {.success = false};
    
    // Create temporary array for literal
    Array *array = array_create(node->element_type);
    if (!array) {
        array_interpreter_error("Failed to create array literal");
        return result;
    }

    // Add all elements
    ArrayASTNode *current = node->children;
    while (current) {
        ArrayElement element = convert_ast_to_element(current);
        if (!array_add_last(array, element)) {
            array_free(array);
            array_interpreter_error("Failed to add element to array literal");
            return result;
        }
        current = current->next;
    }

    result.success = true;
    result.value.array_value = array;
    return result;
}

static ArrayInterpretResult interpret_method_call(ArrayInterpreter *interpreter, ArrayASTNode *node) {
    ArrayInterpretResult result = {.success = false};
    
    Array *array = find_array(interpreter, node->array_name);
    if (!array) {
        array_interpreter_error("Array not found");
        return result;
    }

    switch (node->type) {
        case ARRAY_NODE_ADD_LAST: {
            ArrayElement element = convert_ast_to_element(node->argument);
            result.success = array_add_last(array, element);
            break;
        }
        
        case ARRAY_NODE_ADD_FIRST: {
            ArrayElement element = convert_ast_to_element(node->argument);
            result.success = array_add_first(array, element);
            break;
        }
        
        case ARRAY_NODE_REMOVE_LAST:
            result.success = array_remove_last(array);
            break;
        
        case ARRAY_NODE_REMOVE_FIRST:
            result.success = array_remove_first(array);
            break;
        
        case ARRAY_NODE_LENGTH:
            result.success = true;
            result.value.int_value = array_length(array);
            break;
        
        default:
            array_interpreter_error("Unknown array method");
            return result;
    }

    return result;
}

static ArrayInterpretResult interpret_access(ArrayInterpreter *interpreter, ArrayASTNode *node) {
    ArrayInterpretResult result = {.success = false};
    
    Array *array = find_array(interpreter, node->array_name);
    if (!array) {
        array_interpreter_error("Array not found");
        return result;
    }

    // Evaluate index
    ArrayInterpretResult index_result = array_interpret(interpreter, node->index);
    if (!index_result.success) {
        array_interpreter_error("Failed to evaluate array index");
        return result;
    }

    size_t index = index_result.value.int_value;
    if (index >= array_length(array)) {
        array_interpreter_error("Array index out of bounds");
        return result;
    }

    result.success = true;
    result.value.element = array_get(array, index);
    return result;
}

static Array *find_array(ArrayInterpreter *interpreter, const char *name) {
    for (size_t i = 0; i < interpreter->array_count; i++) {
        if (strcmp(interpreter->array_names[i], name) == 0) {
            return interpreter->arrays[i];
        }
    }
    return NULL;
}

static bool register_array(ArrayInterpreter *interpreter, const char *name, Array *array) {
    // Check if we need to resize
    if (interpreter->array_count >= interpreter->array_capacity) {
        size_t new_capacity = interpreter->array_capacity * 2;
        Array **new_arrays = realloc(interpreter->arrays, new_capacity * sizeof(Array *));
        char **new_names = realloc(interpreter->array_names, new_capacity * sizeof(char *));
        
        if (!new_arrays || !new_names) {
            free(new_arrays);
            free(new_names);
            return false;
        }

        interpreter->arrays = new_arrays;
        interpreter->array_names = new_names;
        interpreter->array_capacity = new_capacity;
    }

    // Add the new array
    interpreter->arrays[interpreter->array_count] = array;
    interpreter->array_names[interpreter->array_count] = strdup(name);
    interpreter->array_count++;

    return true;
}

static ArrayElement convert_ast_to_element(ArrayASTNode *node) {
    ArrayElement element = {0};
    
    switch (node->element_type) {
        case INT_TYPE:
            element.int_value = node->value.int_value;
            break;
        case FLOAT_TYPE:
            element.float_value = node->value.float_value;
            break;
        case STRING_TYPE:
            element.string_value = strdup(node->value.string_value);
            break;
        case BOOL_TYPE:
            element.bool_value = node->value.bool_value;
            break;
        case CHAR_TYPE:
            element.char_value = node->value.char_value;
            break;
        default:
            // Handle error or set default value
            element.int_value = 0;
    }

    return element;
}

void array_interpreter_error(const char *message) {
    fprintf(stderr, "Array Interpreter Error: %s\n", message);
} 