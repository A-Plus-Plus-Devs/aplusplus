#include "array_ops.h"
#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEBUG_LOG(msg, ...) printf("[DEBUG] %s:%d - " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

static VariableType get_array_type(const char *type_str)
{
    DEBUG_LOG("Getting array type for: %s", type_str);
    if (!type_str)
    {
        DEBUG_LOG("type_str is NULL");
        return EMPTY_TYPE;
    }

    if (strcmp(type_str, "int") == 0)
        return INT_TYPE;
    if (strcmp(type_str, "string") == 0)
        return STRING_TYPE;
    if (strcmp(type_str, "float") == 0)
        return FLOAT_TYPE;
    if (strcmp(type_str, "bool") == 0 || strcmp(type_str, "boolean") == 0)
        return BOOL_TYPE;
    if (strcmp(type_str, "any") == 0)
        return EMPTY_TYPE;

    DEBUG_LOG("Unknown type, returning EMPTY_TYPE");
    return EMPTY_TYPE;
}

void *interpret_array_literal(ASTNode *node)
{
    DEBUG_LOG("Interpreting array literal");
    if (!node)
    {
        DEBUG_LOG("Node is NULL");
        return NULL;
    }

    ArrayValue *array = create_array(EMPTY_TYPE, 1);
    if (!array)
    {
        DEBUG_LOG("Failed to create array");
        return NULL;
    }

    DEBUG_LOG("Created array with capacity %zu", array->capacity);
    debug_print_array(array);

    ASTNode *current = node->elements;
    while (current)
    {
        DEBUG_LOG("Processing array element of type %d", current->type);
        void *element = interpret_expression(current);
        if (element)
        {
            array_add_last(array, element);
            DEBUG_LOG("Added element to array");
            debug_print_array(array);
        }
        else
        {
            DEBUG_LOG("Failed to interpret element");
        }
        current = current->next;
    }

    return array;
}

void *interpret_array_declaration(ASTNode *node)
{
    DEBUG_LOG("Interpreting array declaration");
    if (!node)
    {
        DEBUG_LOG("Node is NULL");
        return NULL;
    }

    DEBUG_LOG("Array type: %s", node->array_type);
    VariableType type = get_array_type(node->array_type);
    int is_mixed = (type == EMPTY_TYPE);

    ArrayValue *array = create_array(type, is_mixed);
    DEBUG_LOG("Created array with type %d, is_mixed: %d", type, is_mixed);

    if (node->elements)
    {
        DEBUG_LOG("Processing initial elements");
        ASTNode *current = node->elements;
        while (current)
        {
            void *element = interpret_expression(current);
            if (element)
            {
                array_add_last(array, element);
                DEBUG_LOG("Added element to array, new length: %zu", array->length);
            }
            current = current->next;
        }
    }

    return array;
}

void *interpret_array_method_call(ASTNode *node, ArrayValue *array)
{
    DEBUG_LOG("Interpreting array method call");
    if (!array || !node || !node->method_name)
    {
        DEBUG_LOG("Invalid parameters: array=%p, node=%p, method_name=%s", 
                 (void*)array, (void*)node, node ? node->method_name : "NULL");
        return NULL;
    }

    DEBUG_LOG("Method: %s, Array type: %d, Length: %zu", 
             node->method_name, array->type, array->length);

    // Handle length property first (no argument needed)
    if (strcmp(node->method_name, "length") == 0)
    {
        int *length = malloc(sizeof(int));
        if (!length)
        {
            DEBUG_LOG("Failed to allocate memory for length");
            return NULL;
        }
        *length = array->length;
        DEBUG_LOG("Length method returning: %d", *length);
        return length;
    }

    // For methods that require arguments, interpret the argument first
    void *element = NULL;
    if (strcmp(node->method_name, "addFirst") == 0 || 
        strcmp(node->method_name, "addLast") == 0)
    {
        if (!node->right)
        {
            DEBUG_LOG("No argument provided for %s", node->method_name);
            return NULL;
        }

        element = interpret_expression(node->right);
        if (!element)
        {
            DEBUG_LOG("Failed to interpret argument for %s", node->method_name);
            return NULL;
        }

        // Create a copy of the element based on array type
        void *element_copy = NULL;
        switch (array->type)
        {
            case STRING_TYPE:
                element_copy = strdup((char *)element);
                break;
            case INT_TYPE:
                element_copy = malloc(sizeof(int));
                if (element_copy) *(int *)element_copy = *(int *)element;
                break;
            case FLOAT_TYPE:
                element_copy = malloc(sizeof(double));
                if (element_copy) *(double *)element_copy = *(double *)element;
                break;
            case BOOL_TYPE:
                element_copy = malloc(sizeof(bool));
                if (element_copy) *(bool *)element_copy = *(bool *)element;
                break;
            default:
                DEBUG_LOG("Unsupported array type: %d", array->type);
                free(element);
                return NULL;
        }

        if (!element_copy)
        {
            DEBUG_LOG("Failed to copy element");
            free(element);
            return NULL;
        }

        free(element);  // Free the original element
        element = element_copy;
    }

    // Execute the method
    if (strcmp(node->method_name, "addLast") == 0)
    {
        array_add_last(array, element);
        DEBUG_LOG("Added element to end of array");
        return NULL;
    }

    if (strcmp(node->method_name, "addFirst") == 0)
    {
        array_add_first(array, element);
        DEBUG_LOG("Added element to start of array");
        return NULL;
    }

    if (strcmp(node->method_name, "removeLast") == 0)
    {
        if (array->length == 0)
        {
            DEBUG_LOG("Cannot remove from empty array");
            return NULL;
        }
        return array_remove_last(array);
    }

    if (strcmp(node->method_name, "removeFirst") == 0)
    {
        if (array->length == 0)
        {
            DEBUG_LOG("Cannot remove from empty array");
            return NULL;
        }
        return array_remove_first(array);
    }

    if (strcmp(node->method_name, "insert") == 0)
    {
        if (!node->right || !node->right->next)
        {
            DEBUG_LOG("Insert requires two arguments: index and value");
            return NULL;
        }

        // Get index
        void *index_result = interpret_expression(node->right);
        if (!index_result)
        {
            DEBUG_LOG("Failed to interpret index argument");
            return NULL;
        }
        int index = *(int *)index_result;
        free(index_result);

        // Get value
        void *value = interpret_expression(node->right->next);
        if (!value)
        {
            DEBUG_LOG("Failed to interpret value argument");
            return NULL;
        }

        // Create a copy of the element based on array type
        void *element_copy = NULL;
        switch (array->type)
        {
            case STRING_TYPE:
                element_copy = strdup((char *)value);
                break;
            case INT_TYPE:
                element_copy = malloc(sizeof(int));
                if (element_copy) *(int *)element_copy = *(int *)value;
                break;
            case FLOAT_TYPE:
                element_copy = malloc(sizeof(double));
                if (element_copy) *(double *)element_copy = *(double *)value;
                break;
            case BOOL_TYPE:
                element_copy = malloc(sizeof(bool));
                if (element_copy) *(bool *)element_copy = *(bool *)value;
                break;
            default:
                DEBUG_LOG("Unsupported array type: %d", array->type);
                free(value);
                return NULL;
        }

        if (!element_copy)
        {
            DEBUG_LOG("Failed to copy element");
            free(value);
            return NULL;
        }

        free(value);
        array_insert(array, index, element_copy);
        return NULL;
    }

    DEBUG_LOG("Unknown array method: %s", node->method_name);
    if (element) free(element);
    return NULL;
}

void *interpret_array_access(ASTNode *node, ArrayValue *array)
{
    DEBUG_LOG("Interpreting array access");
    if (!node || !array)
    {
        DEBUG_LOG("Node or array is NULL");
        return NULL;
    }

    // Get the index value
    void *index_result = interpret_expression(node->index);
    if (!index_result)
    {
        DEBUG_LOG("Failed to evaluate index expression");
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
        index = *(int *)index_result;
        free(index_result);
    }

    DEBUG_LOG("Accessing array at index: %d", index);

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
        DEBUG_LOG("Element at index %d is NULL", index);
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
        printf("DEBUG: Retrieved int value: %d\n", *copy);

        break;
    }
    case STRING_TYPE:
        result = strdup((char *)element);
        printf("DEBUG: Retrieved string value: %s\n", (char *)result);

        break;
    case FLOAT_TYPE:
    {
        double *copy = malloc(sizeof(double));
        *copy = *(double *)element;
        result = copy;
        printf("DEBUG: Retrieved float value: %f\n", *copy);
        break;
    }
    case BOOL_TYPE:
    {
        bool *copy = malloc(sizeof(bool));
        *copy = *(bool *)element;
        result = copy;
        printf("DEBUG: Retrieved boolean value: %d\n", *copy);
        break;
    }
    default:
        printf("ERROR: Unsupported array type: %d\n", array->type);

        result = element;
    }

    DEBUG_LOG("Successfully retrieved element at index %d", index);
    return result;
}