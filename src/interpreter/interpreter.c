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
        double float_value;
        char *string_value;
        bool bool_value;
    } value;
} Variable;

// This array stores all the variables in our program
static Variable variables[MAX_VARIABLES];
// This keeps track of how many variables we've created
static int variable_count = 0;

// Add these declarations at the top of the file, after the includes and before any functions
static int evaluate_expression(ASTNode *node);
static bool evaluate_bool_expression(ASTNode *node);

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
                printf("Debug: Updated string variable %s to %s\n", 
                       name, variables[i].value.string_value);
            }
            else if (type == BOOL_TYPE)
            {
                variables[i].value.bool_value = *(bool *)value;
                printf("Debug: Updated existing variable %s to %s\n", 
                       name, variables[i].value.bool_value ? "yup" : "nope");
            }
            else if (type == FLOAT_TYPE)
            {
                variables[i].value.float_value = *(double *)value;
                printf("Debug: Updated float variable %s to %g\n", 
                       name, variables[i].value.float_value);
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
            printf("Debug: Created new string variable %s with value %s\n", 
                   name, variables[variable_count].value.string_value);
        }
        else if (type == BOOL_TYPE)
        {
            variables[variable_count].value.bool_value = *(bool *)value;
            printf("Debug: Created new variable %s with value %s\n", 
                   name, variables[variable_count].value.bool_value ? "yup" : "nope");
        }
        else if (type == FLOAT_TYPE)
        {
            variables[variable_count].value.float_value = *(double *)value;
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
    printf("Debug: Converting '%s' to bool\n", str);
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
        printf("Debug: Null node in evaluate_expression\n");
        return 0;
    }

    printf("Debug: Evaluating node type %d\n", node->type);

    if (node->type == NODE_INT_LITERAL)
    {
        printf("Debug: Int literal value: %s\n", node->value);
        return atoi(node->value);
    }
    else if (node->type == NODE_BOOL_LITERAL)
    {
        printf("Debug: Bool literal value: %s\n", node->value);
        return strtobool(node->value);
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var)
        {
            if (var->type == INT_TYPE)
            {
                printf("Debug: Variable %s value: %d\n", node->value, var->value.int_value);
                return var->value.int_value;
            }
            else if (var->type == BOOL_TYPE)
            {
                printf("Debug: Variable %s value: %s\n", node->value, 
                       var->value.bool_value ? "yup" : "nope");
                return var->value.bool_value;
            }
        }
        printf("Error: Variable '%s' is not an integer/boolean or is undefined.\n", node->value);
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
    printf("Debug: Evaluating bool expression, node type: %d\n", node->type);
    
    if (node->type == NODE_BOOL_LITERAL)
    {
        printf("Debug: Bool literal value: '%s'\n", node->value);
        return strtobool(node->value);
    }
    else if (node->type == NODE_BINARY_OP)
    {
        // Add handling for boolean negation
        if (strcmp(node->value, "!") == 0)
        {
            bool operand = evaluate_bool_expression(node->left);
            return !operand;
        }

        int left = evaluate_expression(node->left);
        int right = evaluate_expression(node->right);
        printf("Debug: Binary op '%s', left: %d, right: %d\n", 
               node->value, left, right);

        if (strcmp(node->value, "==") == 0)
            return left == right;
        if (strcmp(node->value, "!=") == 0)
            return left != right;
        if (strcmp(node->value, ">") == 0)
            return left > right;
        if (strcmp(node->value, "<") == 0)
            return left < right;
        if (strcmp(node->value, ">=") == 0)
            return left >= right;
        if (strcmp(node->value, "<=") == 0)
            return left <= right;
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var && var->type == BOOL_TYPE)
        {
            printf("Debug: Variable bool value: %s\n", 
                   var->value.bool_value ? "yup" : "nope");
            return var->value.bool_value;
        }
    }
    
    printf("Error: Invalid boolean expression.\n");
    return false;
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
        else
        {
            printf("Error: Variable '%s' is not a string or is undefined.\n", node->value);
            return strdup("");
        }
    }

    printf("Error: Unknown string expression type.\n");
    return strdup("");
}

// Add this function to evaluate float expressions
static double evaluate_float_expression(ASTNode *node)
{
    if (node == NULL)
    {
        printf("Debug: Null node in evaluate_float_expression\n");
        return 0.0;
    }

    printf("Debug: Evaluating float expression, node type: %d\n", node->type);

    if (node->type == NODE_FLOAT_LITERAL)
    {
        double value = atof(node->value);
        printf("Debug: Float literal value: %g\n", value);
        return value;
    }
    else if (node->type == NODE_INT_LITERAL)
    {
        double value = (double)atoi(node->value);
        printf("Debug: Int literal converted to float: %g\n", value);
        return value;
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var)
        {
            if (var->type == FLOAT_TYPE)
            {
                printf("Debug: Float variable %s value: %g\n", 
                       node->value, var->value.float_value);
                return var->value.float_value;
            }
            else if (var->type == INT_TYPE)
            {
                printf("Debug: Int variable %s converted to float: %g\n", 
                       node->value, (double)var->value.int_value);
                return (double)var->value.int_value;
            }
        }
        printf("Error: Variable '%s' is not a number or is undefined\n", node->value);
        return 0.0;
    }
    else if (node->type == NODE_BINARY_OP)
    {
        double left = evaluate_float_expression(node->left);
        double right = evaluate_float_expression(node->right);

        printf("Debug: Binary op '%s', left: %g, right: %g\n", 
               node->value, left, right);

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

    printf("Error: Unknown node type in float expression: %d\n", node->type);
    return 0.0;
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
            printf("Debug: Variable declaration %s of type %s\n", node->var_name, node->var_type);
            if (strcmp(node->var_type, "float") == 0)
            {
                double value = node->left ? evaluate_float_expression(node->left) : 0.0;
                printf("Debug: Setting float variable %s to %g\n", node->var_name, value);
                set_variable(node->var_name, FLOAT_TYPE, &value);
            }
            else if (strcmp(node->var_type, "int") == 0)
            {
                int value = node->left ? evaluate_expression(node->left) : 0;
                printf("Debug: Setting variable %s to %d\n", node->var_name, value);
                set_variable(node->var_name, INT_TYPE, &value);
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
                printf("Debug: Setting boolean variable %s to %s\n", 
                       node->var_name, value ? "yup" : "nope");
                set_variable(node->var_name, BOOL_TYPE, &value);
            }
            else if (strcmp(node->var_type, "string") == 0)
            {
                char *value = node->left ? evaluate_string_expression(node->left) : strdup("");
                printf("Debug: Setting variable %s to %s\n", node->var_name, value);
                set_variable(node->var_name, STRING_TYPE, value);
                free(value);
            }
            break;
        }
        case NODE_PRINT:
        {
            printf("Debug: Print statement\n");
            if (node->left->type == NODE_FLOAT_LITERAL ||
                (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == FLOAT_TYPE))
            {
                double result = evaluate_float_expression(node->left);
                printf("%g\n", result);  // %g will automatically handle decimal places
            }
            else if (node->left->type == NODE_BOOL_LITERAL ||
                     (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == BOOL_TYPE))
            {
                bool result = evaluate_bool_expression(node->left);
                printf("%s\n", result ? "yup" : "nope");
            }
            else if (node->left->type == NODE_BINARY_OP)
            {
                if (node->left->value && (
                    strcmp(node->left->value, "==") == 0 ||
                    strcmp(node->left->value, "!=") == 0 ||
                    strcmp(node->left->value, ">") == 0 ||
                    strcmp(node->left->value, "<") == 0 ||
                    strcmp(node->left->value, ">=") == 0 ||
                    strcmp(node->left->value, "<=") == 0 ||
                    strcmp(node->left->value, "!") == 0))
                {
                    bool result = evaluate_bool_expression(node->left);
                    printf("%s\n", result ? "yup" : "nope");
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
            printf("Debug: Assignment to %s\n", node->var_name);
            if (node->left->type == NODE_BOOL_LITERAL)
            {
                bool value = strtobool(node->left->value);
                set_variable(node->var_name, BOOL_TYPE, &value);
            }
            else
            {
                int value = evaluate_expression(node->left);
                set_variable(node->var_name, INT_TYPE, &value);
            }
            break;
        }
        case NODE_IF:
        {
            printf("Debug: Evaluating if condition\n");
            bool condition = evaluate_bool_expression(node->left);
            printf("Debug: Condition result: %s\n", condition ? "yup" : "nope");
            
            if (condition)
            {
                printf("Debug: Executing if branch\n");
                interpret(node->right);
            }
            else if (node->else_branch)
            {
                printf("Debug: Executing else branch\n");
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
        default:
            printf("Error: Unknown node type in interpreter: %d\n", node->type);
            break;
        }
        // Move to the next node
        node = node->next;
    }
}
