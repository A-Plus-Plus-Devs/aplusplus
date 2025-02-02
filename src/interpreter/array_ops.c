/*
 * Array Operations Implementation
 *
 * This file implements the high-level array operations for the A++ language,
 * handling array manipulation through the interpreter.
 *
 * Key features:
 * - Array literal interpretation
 * - Array declaration processing
 * - Array method calls (addFirst, addLast, etc.)
 * - Array element access and modification
 * - Type conversion and validation
 * - Error handling for array operations
 *
 * Supported array operations:
 * - Creation from literals and declarations
 * - Element access and bounds checking
 * - Method calls (length, add, remove, insert)
 * - Type-safe element handling
 * - Mixed-type array support
 *
 * Original Author: Paul Kabulu
 * Created: January 2025
 *
 * Edited by:
 *
 *
 * File: src/interpreter/array_ops.c
 */

#include "array_ops.h"
#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Forward declarations
// static Variable *get_variable(const char *name);

#define DEBUG_LOG(msg, ...) printf("[DEBUG] %s:%d - " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

static VariableType get_array_type(const char *type_str)
{
    // DEBUG_LOG("Getting array type for: %s", type_str);
    if (!type_str)
    {
        // DEBUG_LOG("type_str is NULL");
        return EMPTY_TYPE;
    }

    if (strcmp(type_str, "integer") == 0)
        return INT_TYPE;
    if (strcmp(type_str, "text") == 0)
        return TEXT_TYPE;
    if (strcmp(type_str, "decimal") == 0)
        return DECIMAL_TYPE;
    if (strcmp(type_str, "bool") == 0 || strcmp(type_str, "boolean") == 0)
        return BOOL_TYPE;
    if (strcmp(type_str, "any") == 0)
        return EMPTY_TYPE;

    // DEBUG_LOG("Unknown type, returning EMPTY_TYPE");
    return EMPTY_TYPE;
}

void *interpret_array_literal(ASTNode *node)
{
    // DEBUG_LOG("Interpreting array literal");
    if (!node)
    {
        //  DEBUG_LOG("Node is NULL");
        return NULL;
    }

    ArrayValue *array = create_array(EMPTY_TYPE, 1);
    if (!array)
    {
        // DEBUG_LOG("Failed to create array");
        return NULL;
    }

    // DEBUG_LOG("Created array with capacity %zu", array->capacity);
    // debug_print_array(array);

    ASTNode *current = node->elements;
    while (current)
    {
        // DEBUG_LOG("Processing array element of type %d", current->type);
        void *element = interpret_expression(current);
        if (element)
        {
            array_add_last(array, element);
            // DEBUG_LOG("Added element to array");
            // debug_print_array(array);
        }
        else
        {
            // DEBUG_LOG("Failed to interpret element");
        }
        current = current->next;
    }

    return array;
}

void *interpret_array_declaration(ASTNode *node)
{
    // DEBUG_LOG("Interpreting array declaration");
    if (!node)
    {
        // DEBUG_LOG("Node is NULL");
        return NULL;
    }

    // DEBUG_LOG("Array type: %s", node->array_type);
    VariableType type = get_array_type(node->array_type);
    int is_mixed = (type == EMPTY_TYPE);

    ArrayValue *array = create_array(type, is_mixed);
    // DEBUG_LOG("Created array with type %d, is_mixed: %d", type, is_mixed);

    if (node->elements)
    {
        // DEBUG_LOG("Processing initial elements");
        ASTNode *current = node->elements;
        while (current)
        {
            void *element = interpret_expression(current);
            if (element)
            {
                array_add_last(array, element);
                // DEBUG_LOG("Added element to array, new length: %zu", array->length);
            }
            current = current->next;
        }
    }

    return array;
}

void *interpret_array_method_call(ASTNode *node, ArrayValue *array)
{
    if (!array || !node || !node->method_name)
    {
        return NULL;
    }

    if (strcmp(node->method_name, "length") == 0)
    {
        int *length = malloc(sizeof(int));
        if (!length)
        {
            return NULL;
        }
        *length = array->length;
        return length;
    }

    void *element = NULL;
    if (strcmp(node->method_name, "addFirst") == 0 || 
        strcmp(node->method_name, "addLast") == 0)
    {
        if (!node->right)
        {
            return NULL;
        }

        element = interpret_expression(node->right);
        if (!element)
        {
            return NULL;
        }

        if (array->type == TEXT_TYPE && element)
        {
            char *str_copy = strdup((char*)element);
            if (!str_copy)
            {
                free(element);
                return NULL;
            }
            free(element);
            element = str_copy;
        }
        
        if (strcmp(node->method_name, "addLast") == 0)
        {
            array_add_last(array, element);
        }
        else
        {
            array_add_first(array, element);
        }
        return NULL;
    }

    if (strcmp(node->method_name, "removeLast") == 0)
    {
        if (array->length == 0)
        {
            return NULL;
        }
        return array_remove_last(array);
    }

    if (strcmp(node->method_name, "removeFirst") == 0)
    {
        if (array->length == 0)
        {
            return NULL;
        }
        return array_remove_first(array);
    }

    if (strcmp(node->method_name, "insert") == 0)
    {
        if (!node->right || !node->right->next)
        {
            return NULL;
        }

        // Get index
        void *index_result = interpret_expression(node->right);
        if (!index_result)
        {
            return NULL;
        }
        int index = *(int *)index_result;
        free(index_result);

        // Get value
        void *value = interpret_expression(node->right->next);
        if (!value)
        {
            return NULL;
        }

        // Create a copy of the element based on array type
        void *element_copy = NULL;
        switch (array->type)
        {
            case TEXT_TYPE:
                element_copy = strdup((char *)value);
                if (!element_copy) {
                    return NULL;
                }
                break;
            case INT_TYPE:
                element_copy = malloc(sizeof(int));
                if (element_copy) *(int *)element_copy = *(int *)value;
                break;
            case DECIMAL_TYPE:
                element_copy = malloc(sizeof(double));
                if (element_copy) *(double *)element_copy = *(double *)value;
                break;
            case BOOL_TYPE:
                element_copy = malloc(sizeof(bool));
                if (element_copy) *(bool *)element_copy = *(bool *)value;
                break;
            default:
                return NULL;
        }

        if (!element_copy)
        {
            return NULL;
        }

        free(value);
        array_insert(array, index, element_copy);
        return NULL;
    }

    return NULL;
}

void *interpret_array_access(ASTNode *node, ArrayValue *array)
{
    if (!node || !array)
    {
        return NULL;
    }

    // Get the index value
    void *index_result = interpret_expression(node->index);
    if (!index_result)
    {
        return NULL;
    }

    // Convert index to integer
    int index;
    if (node->index->type == NODE_INT_LITERAL)
    {
        index = atoi(node->index->value);
    }
    else
    {
        // For all other cases (including variables), the interpret_expression should 
        // have already given us the resolved value
        index = *(int *)index_result;
        free(index_result);
    }

    // Check bounds
    if (index < 0 || index >= array->length)
    {
        printf("Error: Array index out of bounds: %d\n", index);
        return NULL;
    }

    // Get the element
    void *element = array->elements[index];
    if (!element)
    {
        return NULL;
    }

    // Return a copy of the element
    void *result = NULL;
    switch (array->type)
    {
    case INT_TYPE:
    {
        int *copy = malloc(sizeof(int));
        *copy = *(int *)element;
        result = copy;
        break;
    }
    case TEXT_TYPE:
        result = strdup((char *)element);
        break;
    case DECIMAL_TYPE:
    {
        double *copy = malloc(sizeof(double));
        *copy = *(double *)element;
        result = copy;
        break;
    }
    case BOOL_TYPE:
    {
        bool *copy = malloc(sizeof(bool));
        *copy = *(bool *)element;
        result = copy;
        break;
    }
    default:
        printf("ERROR: Unsupported array type: %d\n", array->type);

        result = element;
    }

    return result;
}