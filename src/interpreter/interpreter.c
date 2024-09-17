#include "interpreter.h"
#include "common/types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Add this at the top of the file for the pow() function

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
        char *string_value;
        bool bool_value;
    } value;
} Variable;

// This array stores all the variables in our program
static Variable variables[MAX_VARIABLES];
// This keeps track of how many variables we've created
static int variable_count = 0;

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
    return (strcmp(str, "true") == 0 || strcmp(str, "1") == 0);
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
        printf("Debug: Null node in evaluate_expression\n");
        return 0;
    }

    printf("Debug: Evaluating node type %d\n", node->type);

    if (node->type == NODE_INT_LITERAL)
    {
        printf("Debug: Int literal value: %s\n", node->value);
        return atoi(node->value);
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var && var->type == INT_TYPE)
        {
            printf("Debug: Variable %s value: %d\n", node->value, var->value.int_value);
            return var->value.int_value;
        }
        else
        {
            printf("Error: Variable '%s' is not an integer or is undefined.\n", node->value);
            return 0;
        }
    }
    else if (node->type == NODE_BOOL_LITERAL)
    {
        printf("Debug: Bool literal value: %s\n", node->value);
        return strtobool(node->value);
    }
    else if (node->type == NODE_BINARY_OP)
    {
        int left = evaluate_expression(node->left);
        int right = evaluate_expression(node->right);

        printf("Debug: Binary op %s, left: %d, right: %d\n", node->value, left, right);

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

    printf("Error: Unknown expression type: %d\n", node->type);
    return 0;
}

static bool evaluate_bool_expression(ASTNode *node)
{
    if (node->type == NODE_BOOL_LITERAL)
    {
        return strtobool(node->value);
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var && var->type == BOOL_TYPE)
        {
            return var->value.bool_value;
        }
        else
        {
            printf("Error: Variable '%s' is not a boolean or is undefined.\n", node->value);
            return false;
        }
    }
    printf("Error: Unknown boolean expression type.\n");
    return false;
}

static char *evaluate_string_expression(ASTNode *node)
{
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
        else
        {
            printf("Error: Variable '%s' is not a string or is undefined.\n", node->value);
            return strdup("");
        }
    }

    printf("Error: Unknown string expression type.\n");
    return strdup("");
}

// This is the main function that interprets our AST
void interpret(ASTNode *node)
{
    // We loop through each node in our AST
    while (node != NULL)
    {
        printf("Debug: Interpreting node type %d\n", node->type);

        switch (node->type)
        {
        case NODE_VAR_DECLARATION:
        {
            printf("Debug: Variable declaration %s\n", node->var_name);
            if (strcmp(node->var_type, "int") == 0)
            {
                int value = node->left ? evaluate_expression(node->left) : 0;
                printf("Debug: Setting variable %s to %d\n", node->var_name, value);
                set_variable(node->var_name, INT_TYPE, &value);
            }
            else if (strcmp(node->var_type, "string") == 0)
            {
                char *value = node->left ? evaluate_string_expression(node->left) : strdup("");
                printf("Debug: Setting variable %s to %s\n", node->var_name, value);
                set_variable(node->var_name, STRING_TYPE, value);
                free(value);
            }
            else if (strcmp(node->var_type, "bool") == 0)
            {
                bool value = node->left ? evaluate_bool_expression(node->left) : false;
                printf("Debug: Setting variable %s to %s\n", node->var_name, value ? "true" : "false");
                set_variable(node->var_name, BOOL_TYPE, &value);
            }
            break;
        }
        case NODE_PRINT:
        {
            printf("Debug: Print statement\n");
            if (node->left->type == NODE_INT_LITERAL ||
                (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == INT_TYPE) ||
                node->left->type == NODE_BINARY_OP)
            {
                int result = evaluate_expression(node->left);
                printf("%d\n", result);
            }
            else if (node->left->type == NODE_BOOL_LITERAL ||
                     (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == BOOL_TYPE))
            {
                bool result = evaluate_bool_expression(node->left);
                printf("%s\n", result ? "true" : "false");
            }
            else
            {
                char *result = evaluate_string_expression(node->left);
                printf("%s\n", result);
                free(result);
            }
            break;
        }
        case NODE_ASSIGNMENT:
        {
             printf("Debug: Assignment to %s\n", node->var_name);
            Variable *var = get_variable(node->var_name);
            if (var == NULL)
            {
                printf("Error: Undefined variable %s\n", node->var_name);
                break;
            }
            if (var->type == BOOL_TYPE)
            {
                bool value = evaluate_bool_expression(node->left);
                set_variable(node->var_name, BOOL_TYPE, &value);
            }
            else if (var->type == INT_TYPE)
            {
                int value = evaluate_expression(node->left);
                set_variable(node->var_name, INT_TYPE, &value);
            }
            else if (var->type == STRING_TYPE)
            {
                char *value = evaluate_string_expression(node->left);
                set_variable(node->var_name, STRING_TYPE, value);
                free(value);
            }
            break;
        }
        default:
            printf("Error: Unknown node type in interpreter: %d\n", node->type);
            break;
        }
        // Move to the next node
        node = node->next;
    }
}
