#include "interpreter.h"
#include "common/types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// This defines the maximum number of variables our program can handle
#define MAX_VARIABLES 100

// This structure represents a variable in our program
typedef struct
{
    char *name;        // The name of the variable
    VariableType type; // The type of the variable (int or string)
    union
    {
        int int_value; // If it's an int, store the value here
        double float_value;
        char *string_value;
        bool bool_value;
        char char_value;  // For character literals
    } value;
} Variable;

// This array stores all the variables in our program
static Variable variables[MAX_VARIABLES];
// This keeps track of how many variables we've created
static int variable_count = 0;

// Add these declarations at the top of the file, after the includes and before any functions
static int evaluate_expression(ASTNode *node);
static bool evaluate_bool_expression(ASTNode *node);
static double evaluate_float_expression(ASTNode *node);
static char* evaluate_string_expression(ASTNode *node);

// This function sets the value of a variable
static void set_variable(const char *name, VariableType type, void *value)
{
    // First, we check if the variable already exists
    for (int i = 0; i < variable_count; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            // If it exists, we update its value
            variables[i].type = type;
            if (type == INT_TYPE)
            {
                // If it's an int, we store the int value
                variables[i].value.int_value = *(int *)value;
            }
            else if (type == STRING_TYPE)
            {
                // If it's a string, we free the old string and store the new one
                free(variables[i].value.string_value);
                variables[i].value.string_value = strdup((char *)value);
            }
            else if (type == BOOL_TYPE)
            {
                variables[i].value.bool_value = *(bool *)value;
            }
            else if (type == FLOAT_TYPE)
            {
                variables[i].value.float_value = *(double *)value;
            }
            else if (type == CHAR_TYPE)
            {
                variables[i].value.char_value = *(char *)value;
            }
            return;
        }
    }

    // If the variable doesn't exist, we create a new one
    if (variable_count < MAX_VARIABLES)
    {
        // We store the name of the variable
        variables[variable_count].name = strdup(name);
        variables[variable_count].type = type;
        if (type == INT_TYPE)
        {
            // If it's an int, we store the int value
            variables[variable_count].value.int_value = *(int *)value;
        }
        else if (type == STRING_TYPE)
        {
            // If it's a string, we store a copy of the string
            variables[variable_count].value.string_value = strdup((char *)value);
        }
        else if (type == BOOL_TYPE)
        {
            variables[variable_count].value.bool_value = *(bool *)value;
        }
        else if (type == FLOAT_TYPE)
        {
            variables[variable_count].value.float_value = *(double *)value;
        }
        else if (type == CHAR_TYPE)
        {
            variables[variable_count].value.char_value = *(char *)value;
        }
        variable_count++;
    }
    else
    {
        // If we've reached the maximum number of variables, we print an error
        printf("Error: Maximum number of variables reached.\n");
    }
}

static bool strtobool(const char *str)
{
    if (str == NULL) return false;
    return (strcmp(str, "yup") == 0);
}

// This function gets the value of a variable
static Variable *get_variable(const char *name)
{
    // We loop through all variables
    for (int i = 0; i < variable_count; i++)
    {
        // If we find a variable with the given name, we return it
        if (strcmp(variables[i].name, name) == 0)
        {
            return &variables[i];
        }
    }
    // If we didn't find the variable, we return NULL
    return NULL;
}

// This function evaluates an expression (currently only supports basic operations)
static int evaluate_expression(ASTNode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    if (node->type == NODE_INT_LITERAL) {
        return atoi(node->value);
    }
    else if (node->type == NODE_FLOAT_LITERAL)
    {
        return (int)atof(node->value);
    }
    else if (node->type == NODE_BOOL_LITERAL)
    {
        return strtobool(node->value);
    }
    else if (node->type == NODE_CHAR_LITERAL)
    {
        return node->value[0];
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var)
        {
            if (var->type == INT_TYPE)
            {
                return var->value.int_value;
            }
            else if (var->type == FLOAT_TYPE)
            {
                return (int)var->value.float_value;
            }
            else if (var->type == BOOL_TYPE)
            {
                return var->value.bool_value;
            }
            else if (var->type == CHAR_TYPE)
            {
                return var->value.char_value;
            }
        }
        return 0;
    }
    else if (node->type == NODE_BINARY_OP)
    {
        // Handle boolean negation specially
        if (strcmp(node->value, "!") == 0)
        {
            bool operand = evaluate_bool_expression(node->left);
            return !operand;
        }

        // Check if either operand is a float
        bool has_float = false;
        if (node->left && (
            node->left->type == NODE_FLOAT_LITERAL ||
            (node->left->type == NODE_LITERAL && 
             get_variable(node->left->value) && 
             get_variable(node->left->value)->type == FLOAT_TYPE)))
        {
            has_float = true;
        }
        if (node->right && (
            node->right->type == NODE_FLOAT_LITERAL ||
            (node->right->type == NODE_LITERAL && 
             get_variable(node->right->value) && 
             get_variable(node->right->value)->type == FLOAT_TYPE)))
        {
            has_float = true;
        }

        if (has_float)
        {
            return (int)evaluate_float_expression(node);
        }

        int left = evaluate_expression(node->left);
        int right = evaluate_expression(node->right);

        if (strcmp(node->value, "+") == 0)
            return left + right;
        if (strcmp(node->value, "-") == 0)
            return left - right;
        if (strcmp(node->value, "*") == 0)
            return left * right;
        if (strcmp(node->value, "/") == 0)
        {
            if (right == 0)
            {
                printf("Error: Division by zero\n");
                return 0;
            }
            return left / right;
        }
        if (strcmp(node->value, "**") == 0)
            return (int)pow(left, right);
        if (strcmp(node->value, "%") == 0)
        {
            if (right == 0)
            {
                printf("Error: Modulus by zero\n");
                return 0;
            }
            return left % right;
        }
    }

    return 0;
}

static bool evaluate_bool_expression(ASTNode *node)
{
    if (node == NULL)
    {
        // printf("DEBUG: evaluate_bool_expression received NULL node\n");
        return false;
    }

    // printf("DEBUG: evaluate_bool_expression - node type: %d\n", node->type);
    if (node->value)
        // printf("DEBUG: evaluate_bool_expression - node value: %s\n", node->value);

    if (node->type == NODE_BINARY_OP)
    {
        // printf("DEBUG: Processing binary operation: %s\n", node->value);
        
        // For comparison operators
        if (strcmp(node->value, ">") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            // printf("DEBUG: Comparing %d > %d\n", left, right);
            return left > right;
        }
        else if (strcmp(node->value, "<") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            // printf("DEBUG: Comparing %d < %d\n", left, right);
            return left < right;
        }
        else if (strcmp(node->value, "<=") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            return left <= right;
        }
        else if (strcmp(node->value, ">=") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            return left >= right;
        }
        else if (strcmp(node->value, "==") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            // printf("DEBUG: Comparing %d == %d\n", left, right);
            return left == right;
        }
        else if (strcmp(node->value, "&&") == 0)
        {
            // printf("DEBUG: Processing AND operation\n");
            bool left = evaluate_bool_expression(node->left);
            // printf("DEBUG: Left side of AND is %d\n", left);
            if (!left) return false; // Short circuit
            bool right = evaluate_bool_expression(node->right);
            // printf("DEBUG: Right side of AND is %d\n", right);
            return right;
        }
        else if (strcmp(node->value, "||") == 0)
        {
            // printf("DEBUG: Processing OR operation\n");
            bool left = evaluate_bool_expression(node->left);
            // printf("DEBUG: Left side of OR is %d\n", left);
            if (left) return true; // Short circuit
            bool right = evaluate_bool_expression(node->right);
            // printf("DEBUG: Right side of OR is %d\n", right);
            return right;
        }
    }
    else if (node->type == NODE_LITERAL)
    {
        // printf("DEBUG: Processing literal: %s\n", node->value);
        Variable *var = get_variable(node->value);
        if (var)
        {
            // printf("DEBUG: Found variable %s of type %d\n", node->value, var->type);
            if (var->type == BOOL_TYPE)
            {
                return var->value.bool_value;
            }
            else if (var->type == INT_TYPE)
            {
                // printf("DEBUG: Int value is %d\n", var->value.int_value);
                return var->value.int_value != 0;
            }
            else if (var->type == CHAR_TYPE)
            {
                // printf("DEBUG: Char value is %c\n", var->value.char_value);
                return var->value.char_value != '\0';
            }
        }
        // else
        // {
        //     printf("DEBUG: Variable %s not found\n", node->value);
        // }
    }
    else if (node->type == NODE_INT_LITERAL)
    {
        // printf("DEBUG: Processing int literal: %s\n", node->value);
        return atoi(node->value) != 0;
    }

    // For any other expression, evaluate it and convert to boolean
    int result = evaluate_expression(node);
    // printf("DEBUG: Evaluated expression result: %d\n", result);
    return result != 0;
}

static char *evaluate_string_expression(ASTNode *node)
{
    if (node == NULL) return strdup("");

    if (node->type == NODE_STRING_LITERAL)
    {
        return strdup(node->value);
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var && var->type == STRING_TYPE)
        {
            return strdup(var->value.string_value);
        }
        else if (var && var->type == INT_TYPE)
        {
            // Convert int to string
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d", var->value.int_value);
            return strdup(buffer);
        }
        else if (var && var->type == FLOAT_TYPE)
        {
            // Convert float to string
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%g", var->value.float_value);
            return strdup(buffer);
        }
        else if (var && var->type == BOOL_TYPE)
        {
            // Convert bool to string
            return strdup(var->value.bool_value ? "yup" : "nope");
        }
        else if (var && var->type == CHAR_TYPE)
        {
            // Convert char to string
            char buffer[2];
            buffer[0] = var->value.char_value;
            buffer[1] = '\0';
            return strdup(buffer);
        }
        else
        {
            return strdup("");
        }
    }
    else if (node->type == NODE_BINARY_OP && strcmp(node->value, "+") == 0)
    {
        // Handle string concatenation
        char *left = evaluate_string_expression(node->left);
        char *right = evaluate_string_expression(node->right);
        
        // Allocate space for concatenated string
        char *result = malloc(strlen(left) + strlen(right) + 1);
        if (result)
        {
            strcpy(result, left);
            strcat(result, right);
        }
        
        // Free temporary strings
        free(left);
        free(right);
        
        return result ? result : strdup("");
    }
    else if (node->type == NODE_INT_LITERAL)
    {
        // Convert int literal to string
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", atoi(node->value));
        return strdup(buffer);
    }
    else if (node->type == NODE_FLOAT_LITERAL)
    {
        // Convert float literal to string
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%g", atof(node->value));
        return strdup(buffer);
    }
    else if (node->type == NODE_BOOL_LITERAL)
    {
        // Convert bool literal to string
        return strdup(strcmp(node->value, "yup") == 0 ? "yup" : "nope");
    }
    else if (node->type == NODE_CHAR_LITERAL)
    {
        // Convert char literal to string
        char buffer[2];
        buffer[0] = node->value[0];
        buffer[1] = '\0';
        return strdup(buffer);
    }

    return strdup("");
}

static double evaluate_float_expression(ASTNode *node)
{
    if (node == NULL)
    {
        return 0.0;
    }

    // Handle float literals
    if (node->type == NODE_FLOAT_LITERAL)
    {
        return atof(node->value);
    }
    // Promote int literals to float
    else if (node->type == NODE_INT_LITERAL)
    {
        return (double)atoi(node->value);
    }
    // Handle variables
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var)
        {
            if (var->type == FLOAT_TYPE)
            {
                return var->value.float_value;
            }
            else if (var->type == INT_TYPE)
            {
                return (double)var->value.int_value;
            }
            else
            {
                printf("Error: Cannot convert %s to float\n", node->value);
                return 0.0;
            }
        }
        printf("Error: Undefined variable %s\n", node->value);
        return 0.0;
    }
    // Handle binary operations
    else if (node->type == NODE_BINARY_OP)
    {
        // Check if either operand is a float or needs float evaluation
        bool left_is_float = node->left && (
            node->left->type == NODE_FLOAT_LITERAL ||
            (node->left->type == NODE_LITERAL && 
             get_variable(node->left->value) && 
             get_variable(node->left->value)->type == FLOAT_TYPE) ||
            node->left->type == NODE_BINARY_OP);

        bool right_is_float = node->right && (
            node->right->type == NODE_FLOAT_LITERAL ||
            (node->right->type == NODE_LITERAL && 
             get_variable(node->right->value) && 
             get_variable(node->right->value)->type == FLOAT_TYPE) ||
            node->right->type == NODE_BINARY_OP);

        double left = left_is_float ? 
            evaluate_float_expression(node->left) : 
            (double)evaluate_expression(node->left);

        double right = right_is_float ? 
            evaluate_float_expression(node->right) : 
            (double)evaluate_expression(node->right);

        if (strcmp(node->value, "+") == 0)
            return left + right;
        if (strcmp(node->value, "-") == 0)
            return left - right;
        if (strcmp(node->value, "*") == 0)
            return left * right;
        if (strcmp(node->value, "/") == 0)
        {
            if (right == 0.0)
            {
                printf("Error: Division by zero\n");
                return 0.0;
            }
            return left / right;
        }
        if (strcmp(node->value, "**") == 0)
            return pow(left, right);
        if (strcmp(node->value, "%") == 0)
        {
            if (right == 0.0)
            {
                printf("Error: Modulus by zero\n");
                return 0.0;
            }
            return fmod(left, right);
        }
    }

    return 0.0;
}

// This is the main function that interprets our AST
void interpret(ASTNode *node)
{
    while (node != NULL)
    {
        switch (node->type)
        {
            case NODE_VAR_DECLARATION:
            {
                if (strcmp(node->var_type, "float") == 0)
                {
                    double value = node->left ? evaluate_float_expression(node->left) : 0.0;
                    set_variable(node->var_name, FLOAT_TYPE, &value);
                }
                else if (strcmp(node->var_type, "int") == 0)
                {
                    // Check if the expression contains any float operations
                    bool has_float = false;
                    if (node->left)
                    {
                        has_float = node->left->type == NODE_FLOAT_LITERAL ||
                                  (node->left->type == NODE_LITERAL && 
                                   get_variable(node->left->value) && 
                                   get_variable(node->left->value)->type == FLOAT_TYPE);
                        
                        // Check for binary operations involving floats
                        if (node->left->type == NODE_BINARY_OP)
                        {
                            if (node->left->left && (
                                node->left->left->type == NODE_FLOAT_LITERAL ||
                                (node->left->left->type == NODE_LITERAL && 
                                 get_variable(node->left->left->value) && 
                                 get_variable(node->left->left->value)->type == FLOAT_TYPE)))
                            {
                                has_float = true;
                            }
                            if (node->left->right && (
                                node->left->right->type == NODE_FLOAT_LITERAL ||
                                (node->left->right->type == NODE_LITERAL && 
                                 get_variable(node->left->right->value) && 
                                 get_variable(node->left->right->value)->type == FLOAT_TYPE)))
                            {
                                has_float = true;
                            }
                        }
                    }
                    
                    if (has_float)
                    {
                        printf("Warning: Possible loss of precision assigning float to int\n");
                        int value = (int)evaluate_float_expression(node->left);
                        set_variable(node->var_name, INT_TYPE, &value);
                    }
                    else
                    {
                        int value = node->left ? evaluate_expression(node->left) : 0;
                        set_variable(node->var_name, INT_TYPE, &value);
                    }
                }
                else if (strcmp(node->var_type, "boolean") == 0)
                {
                    bool value;
                    if (node->left && node->left->type == NODE_BOOL_LITERAL)
                    {
                        value = strtobool(node->left->value);
                    }
                    else
                    {
                        value = false;
                    }
                    set_variable(node->var_name, BOOL_TYPE, &value);
                }
                else if (strcmp(node->var_type, "string") == 0)
                {
                    char *value = node->left ? evaluate_string_expression(node->left) : strdup("");
                    set_variable(node->var_name, STRING_TYPE, value);
                    free(value);
                }
                else if (strcmp(node->var_type, "char") == 0)
                {
                    char value = node->left ? node->left->value[0] : '\0';
                    set_variable(node->var_name, CHAR_TYPE, &value);
                }
                break;
            }
            case NODE_PRINT:
            {
                if (node->left->type == NODE_FLOAT_LITERAL ||
                    (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == FLOAT_TYPE))
                {
                    double result = evaluate_float_expression(node->left);
                    printf("%g\n", result);
                }
                else if (node->left->type == NODE_BOOL_LITERAL ||
                         (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == BOOL_TYPE))
                {
                    bool result = evaluate_bool_expression(node->left);
                    printf("%s\n", result ? "yup" : "nope");
                }
                else if (node->left->type == NODE_CHAR_LITERAL ||
                         (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == CHAR_TYPE))
                {
                    char result = evaluate_expression(node->left);
                    printf("%c\n", result);
                }
                else if (node->left->type == NODE_BINARY_OP)
                {
                    // Check if any operand is a float
                    bool has_float = false;
                    if (node->left->left && (
                        node->left->left->type == NODE_FLOAT_LITERAL ||
                        (node->left->left->type == NODE_LITERAL && 
                         get_variable(node->left->left->value) && 
                         get_variable(node->left->left->value)->type == FLOAT_TYPE)))
                    {
                        has_float = true;
                    }
                    if (node->left->right && (
                        node->left->right->type == NODE_FLOAT_LITERAL ||
                        (node->left->right->type == NODE_LITERAL && 
                         get_variable(node->left->right->value) && 
                         get_variable(node->left->right->value)->type == FLOAT_TYPE)))
                    {
                        has_float = true;
                    }

                    // Check if it's a string concatenation operation
                    if (node->left->value && strcmp(node->left->value, "+") == 0 &&
                        (node->left->left->type == NODE_STRING_LITERAL || 
                         node->left->right->type == NODE_STRING_LITERAL ||
                         (node->left->left->type == NODE_LITERAL && 
                          get_variable(node->left->left->value)->type == STRING_TYPE) ||
                         (node->left->right->type == NODE_LITERAL && 
                          get_variable(node->left->right->value)->type == STRING_TYPE)))
                    {
                        char *result = evaluate_string_expression(node->left);
                        printf("%s\n", result);
                        free(result);
                    }
                    // Keep existing boolean expression handling
                    else if (node->left->value && (
                        strcmp(node->left->value, "==") == 0 ||
                        strcmp(node->left->value, "!=") == 0 ||
                        strcmp(node->left->value, ">") == 0  ||
                        strcmp(node->left->value, "<") == 0  ||
                        strcmp(node->left->value, ">=") == 0 ||
                        strcmp(node->left->value, "<=") == 0 ||
                        strcmp(node->left->value, "!") == 0))
                    {
                        bool result = evaluate_bool_expression(node->left);
                        printf("%s\n", result ? "yup" : "nope");
                    }
                    else if (has_float)
                    {
                        double result = evaluate_float_expression(node->left);
                        printf("%g\n", result);
                    }
                    else
                    {
                        int result = evaluate_expression(node->left);
                        printf("%d\n", result);
                    }
                }
                else if (node->left->type == NODE_STRING_LITERAL ||
                         (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == STRING_TYPE))
                {
                    char *result = evaluate_string_expression(node->left);
                    printf("%s\n", result);
                    free(result);
                }
                else
                {
                    int result = evaluate_expression(node->left);
                    printf("%d\n", result);
                }
                break;
            }
            case NODE_ASSIGNMENT:
            {
                // Get the variable being assigned to
                Variable *var = get_variable(node->var_name);
                if (!var)
                {
                    printf("Error: Undefined variable %s\n", node->var_name);
                    break;
                }

                if (var->type == FLOAT_TYPE)
                {
                    double value = evaluate_float_expression(node->left);
                    set_variable(node->var_name, FLOAT_TYPE, &value);
                }
                else if (var->type == INT_TYPE)
                {
                    // Check if the expression contains any float operations
                    bool has_float = false;
                    if (node->left)
                    {
                        // Check for direct float values
                        has_float = node->left->type == NODE_FLOAT_LITERAL ||
                                  (node->left->type == NODE_LITERAL && 
                                   get_variable(node->left->value) && 
                                   get_variable(node->left->value)->type == FLOAT_TYPE);
                        
                        // Check for binary operations involving floats
                        if (node->left->type == NODE_BINARY_OP)
                        {
                            if (node->left->left && (
                                node->left->left->type == NODE_FLOAT_LITERAL ||
                                (node->left->left->type == NODE_LITERAL && 
                                 get_variable(node->left->left->value) && 
                                 get_variable(node->left->left->value)->type == FLOAT_TYPE)))
                            {
                                has_float = true;
                            }
                            if (node->left->right && (
                                node->left->right->type == NODE_FLOAT_LITERAL ||
                                (node->left->right->type == NODE_LITERAL && 
                                 get_variable(node->left->right->value) && 
                                 get_variable(node->left->right->value)->type == FLOAT_TYPE)))
                            {
                                has_float = true;
                            }
                        }
                    }
                    
                    if (has_float)
                    {
                        printf("Warning: Possible loss of precision assigning float to int\n");
                        int value = (int)evaluate_float_expression(node->left);
                        set_variable(node->var_name, INT_TYPE, &value);
                    }
                    else
                    {
                        int value = evaluate_expression(node->left);
                        set_variable(node->var_name, INT_TYPE, &value);
                    }
                }
                else if (var->type == BOOL_TYPE)
                {
                    bool value;
                    if (node->left && node->left->type == NODE_BOOL_LITERAL)
                    {
                        value = strtobool(node->left->value);
                    }
                    else
                    {
                        value = false;
                    }
                    set_variable(node->var_name, BOOL_TYPE, &value);
                }
                else if (var->type == STRING_TYPE)
                {
                    char *value = node->left ? evaluate_string_expression(node->left) : strdup("");
                    set_variable(node->var_name, STRING_TYPE, value);
                    free(value);
                }
                else if (var->type == CHAR_TYPE)
                {
                    char value = node->left ? node->left->value[0] : '\0';
                    set_variable(node->var_name, CHAR_TYPE, &value);
                }
                break;
            }
            case NODE_IF:
            {
                bool condition = evaluate_bool_expression(node->left);
                
                if (condition)
                {
                    interpret(node->right);
                }
                else if (node->else_branch)
                {
                    interpret(node->else_branch);
                }
                break;
            }
            case NODE_TERNARY:
            {
                bool condition = evaluate_bool_expression(node->left);
                if (condition)
                {
                    interpret(node->right);  // true expression
                }
                else if (node->else_branch)
                {
                    interpret(node->else_branch);  // false expression
                }
                break;
            }
            case NODE_FOR:
            {
                // Execute initialisation
                interpret(node->init);

                // Loop while condition is true
                while (evaluate_bool_expression(node->condition))
                {
                    // Execute body
                    interpret(node->body);
                    
                    // Execute increment
                    interpret(node->increment);
                }
                break;
            }
            default:
                break;
        }
        node = node->next;
    }
}
