#include "interpreter.h"
#include "common/types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// This defines the maximum number of variables our program can handle
#define MAX_VARIABLES 100
#define MAX_FUNCTIONS 100

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

// Function definition structure
typedef struct {
    char *name;
    char *return_type;
    ASTNode *parameters;
    ASTNode *body;
} Function;

// Function declarations
static int evaluate_expression(ASTNode *node);
static bool evaluate_bool_expression(ASTNode *node);
static double evaluate_float_expression(ASTNode *node);
static char *evaluate_string_expression(ASTNode *node);
static VariableType get_type_from_string(const char *type_str);
static Function *find_function(const char *name);
static char *execute_function(const char *name, ASTNode *arguments);
static void set_variable(const char *name, VariableType type, void *value);
static Variable *get_variable(const char *name);

static Function *find_function(const char *name);
static char *execute_function(const char *name, ASTNode *arguments);
static void set_variable(const char *name, VariableType type, void *value);
static Variable *get_variable(const char *name);
static char *execute_function_body(const char *return_type, ASTNode *body);

// Global variables
static Function functions[MAX_FUNCTIONS];
static int function_count = 0;
static Variable variables[MAX_VARIABLES];
static int variable_count = 0;

// Helper function to convert type string to VariableType
static VariableType get_type_from_string(const char *type_str) {
    if (strcmp(type_str, "int") == 0) {
        return INT_TYPE;
    } else if (strcmp(type_str, "float") == 0) {
        return FLOAT_TYPE;
    } else if (strcmp(type_str, "string") == 0) {
        return STRING_TYPE;
    } else if (strcmp(type_str, "boolean") == 0) {
        return BOOL_TYPE;
    } else if (strcmp(type_str, "char") == 0) {
        return CHAR_TYPE;
    }
    return INT_TYPE; // Default to INT_TYPE
}

static VariableType get_function_return_type(const char *return_type_str) {
    if (strcmp(return_type_str, "int") == 0) {
        return INT_TYPE;
    } else if (strcmp(return_type_str, "float") == 0) {
        return FLOAT_TYPE;
    } else if (strcmp(return_type_str, "string") == 0) {
        return STRING_TYPE;
    } else if (strcmp(return_type_str, "boolean") == 0) {
        return BOOL_TYPE;
    } else if (strcmp(return_type_str, "char") == 0) {
        return CHAR_TYPE;
    }
    return INT_TYPE; // Default
}

// Register a function definition
void register_function(char *name, char *return_type, ASTNode *parameters, ASTNode *body) {
    if (function_count >= MAX_FUNCTIONS) {
        printf("Error: Maximum number of functions reached\n");
        return;
    }
    
    // Check if function already exists
    for (int i = 0; i < function_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            printf("Error: Function '%s' already defined\n", name);
            return;
        }
    }
    
    functions[function_count].name = strdup(name);
    functions[function_count].return_type = strdup(return_type);
    functions[function_count].parameters = parameters;
    functions[function_count].body = body;
    function_count++;
}

// Find a function by name
Function *find_function(const char *name) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return &functions[i];
        }
    }
    return NULL;
}

// Execute a function and return its result as a string
char* execute_function(const char *name, ASTNode *arguments) {
    Function *func = find_function(name);
    if (!func) {
        printf("Error: Function '%s' not found\n", name);
        return NULL;
    }
    
    // Save current variable count to restore after function execution
    int saved_var_count = variable_count;
    
    // Create a new scope for function parameters
    ASTNode *param = func->parameters;
    ASTNode *arg = arguments;
    
    while (param && arg) {
        // Evaluate argument
        void *value = NULL;
        int int_val;
        double float_val;
        bool bool_val;
        char char_val;
        char *str_val;
        
        if (strcmp(param->var_type, "int") == 0) {
            int_val = evaluate_expression(arg);
            value = &int_val;
        } else if (strcmp(param->var_type, "float") == 0) {
            float_val = evaluate_float_expression(arg);
            value = &float_val;
        } else if (strcmp(param->var_type, "string") == 0) {
            str_val = evaluate_string_expression(arg);
            value = str_val;
        } else if (strcmp(param->var_type, "boolean") == 0) {
            bool_val = evaluate_bool_expression(arg);
            value = &bool_val;
        } else if (strcmp(param->var_type, "char") == 0) {
            char_val = (char)evaluate_expression(arg);
            value = &char_val;
        }
        
        // Set parameter value
        if (value) {
            set_variable(param->var_name, get_type_from_string(param->var_type), value);
            if (strcmp(param->var_type, "string") == 0) {
                free(str_val);
            }
        }
        
        param = param->next;
        arg = arg->next;
    }
    
    // Execute function body
    ASTNode *current = func->body;
    char *result = NULL;
    
    while (current) {
        if (current->type == NODE_YIELD_STATEMENT) {
            if (!current->left) {
                return strdup("");
            }
            
            // Handle yield statement based on return type
            if (strcmp(func->return_type, "int") == 0) {
                int val = evaluate_expression(current->left);
                char buf[32];
                snprintf(buf, sizeof(buf), "%d", val);
                result = strdup(buf);
            } else if (strcmp(func->return_type, "float") == 0) {
                double val = evaluate_float_expression(current->left);
                char buf[32];
                snprintf(buf, sizeof(buf), "%g", val);
                result = strdup(buf);
            } else if (strcmp(func->return_type, "string") == 0) {
                if (current->left->type == NODE_STRING_LITERAL) {
                    result = strdup(current->left->value);
                } else {
                    result = evaluate_string_expression(current->left);
                }
                if (!result) {
                    result = strdup("");
                }
            } else if (strcmp(func->return_type, "boolean") == 0) {
                bool val = evaluate_bool_expression(current->left);
                result = strdup(val ? "yup" : "nope");
            } else if (strcmp(func->return_type, "char") == 0) {
                char val = (char)evaluate_expression(current->left);
                result = (char *)malloc(2);
                result[0] = val;
                result[1] = '\0';
            }
            break;
        } else if (current->type == NODE_IF) {
            bool condition = evaluate_bool_expression(current->left);
            if (condition) {
                char *if_result = execute_function_body(func->return_type, current->right);
                if (if_result) {
                    result = if_result;
                    break;
                }
            } else if (current->else_branch) {
                char *else_result = execute_function_body(func->return_type, current->else_branch);
                if (else_result) {
                    result = else_result;
                    break;
                }
            }
            current = current->next;
        } else {
            interpret(current);
            current = current->next;
        }
    }
    
    // Restore variable count (clean up function scope)
    while (variable_count > saved_var_count) {
        variable_count--;
        free(variables[variable_count].name);
        if (variables[variable_count].type == STRING_TYPE) {
            free(variables[variable_count].value.string_value);
        }
    }
    
    if (func->return_type && strcmp(func->return_type, "nothing") == 0) {
        // For void functions, just execute the body and return NULL
        return NULL;  // Don't execute the body here
    }
    
    return result;
}

static char *execute_function_body(const char *return_type, ASTNode *body) {
    ASTNode *current = body;
    char *result = NULL;
    
    while (current) {
        if (current->type == NODE_YIELD_STATEMENT) {
            if (!current->left) {
                return strdup("");
            }
            
            // Handle yield statement based on return type
            if (strcmp(return_type, "int") == 0) {
                int val = evaluate_expression(current->left);
                char buf[32];
                snprintf(buf, sizeof(buf), "%d", val);
                result = strdup(buf);
            } else if (strcmp(return_type, "float") == 0) {
                double val = evaluate_float_expression(current->left);
                char buf[32];
                snprintf(buf, sizeof(buf), "%g", val);
                result = strdup(buf);
            } else if (strcmp(return_type, "string") == 0) {
                if (current->left->type == NODE_STRING_LITERAL) {
                    result = strdup(current->left->value);
                } else {
                    result = evaluate_string_expression(current->left);
                }
                if (!result) {
                    result = strdup("");
                }
            } else if (strcmp(return_type, "boolean") == 0) {
                bool val = evaluate_bool_expression(current->left);
                result = strdup(val ? "yup" : "nope");
            } else if (strcmp(return_type, "char") == 0) {
                char val = (char)evaluate_expression(current->left);
                result = (char *)malloc(2);
                result[0] = val;
                result[1] = '\0';
            }
            break;
        } else {
            interpret(current);
            current = current->next;
        }
    }
    
    return result;
}

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
            else if (var->type == STRING_TYPE)
            {
                return 0;
            }
        }
        return 0;
    }
    else if (node->type == NODE_FUNCTION_CALL)
    {
        char *result = execute_function(node->function_name, node->arguments);
        if (result) {
            int value = atoi(result);
            free(result);
            return value;
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

        // Get values for both operands, handling variables specially
        int left = 0, right = 0;
        
        if (node->left->type == NODE_LITERAL)
        {
            Variable *var = get_variable(node->left->value);
            if (var && var->type == INT_TYPE)
            {
                left = var->value.int_value;
            }
            else
            {
                left = evaluate_expression(node->left);
            }
        }
        else
        {
            left = evaluate_expression(node->left);
        }

        if (node->right->type == NODE_LITERAL)
        {
            Variable *var = get_variable(node->right->value);
            if (var && var->type == INT_TYPE)
            {
                right = var->value.int_value;
            }
            else
            {
                right = evaluate_expression(node->right);
            }
        }
        else
        {
            right = evaluate_expression(node->right);
        }

        // printf("[DEBUG] Binary operation: %s, left=%d, right=%d\n", node->value, left, right);

        // If either operand is a float, use float evaluation
        if (has_float)
        {
            return (int)evaluate_float_expression(node);
        }

        int result = 0;
        // Otherwise proceed with integer operations
        if (strcmp(node->value, "+") == 0)
            result = left + right;
        else if (strcmp(node->value, "-") == 0)
            result = left - right;
        else if (strcmp(node->value, "*") == 0)
            result = left * right;
        else if (strcmp(node->value, "/") == 0)
        {
            if (right == 0)
            {
                printf("Error: Division by zero\n");
                return 0;
            }
            result = left / right;
        }
        else if (strcmp(node->value, "**") == 0)
            result = (int)pow(left, right);
        else if (strcmp(node->value, "%") == 0)
        {
            if (right == 0)
            {
                printf("Error: Modulus by zero\n");
                return 0;
            }
            result = left % right;
        }
        // printf("[DEBUG] Binary operation result: %d\n", result);
        return result;
    }

    return 0;
}

static bool evaluate_bool_expression(ASTNode *node)
{
    if (node == NULL)
    {
        return false;
    }

    if (node->type == NODE_BINARY_OP)
    {
        // For comparison operators
        if (strcmp(node->value, ">") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            return left > right;
        }
        else if (strcmp(node->value, "<") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
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
            return left == right;
        }
        else if (strcmp(node->value, "!=") == 0)
        {
            int left = evaluate_expression(node->left);
            int right = evaluate_expression(node->right);
            return left != right;
        }
        else if (strcmp(node->value, "&&") == 0)
        {
            bool left = evaluate_bool_expression(node->left);
            if (!left) return false; // Short circuit
            bool right = evaluate_bool_expression(node->right);
            return right;
        }
        else if (strcmp(node->value, "||") == 0)
        {
            bool left = evaluate_bool_expression(node->left);
            if (left) return true; // Short circuit
            bool right = evaluate_bool_expression(node->right);
            return right;
        }
    }
    else if (node->type == NODE_LITERAL)
    {
        Variable *var = get_variable(node->value);
        if (var)
        {
            if (var->type == BOOL_TYPE)
            {
                return var->value.bool_value;
            }
            else if (var->type == INT_TYPE)
            {
                return var->value.int_value != 0;
            }
            else if (var->type == CHAR_TYPE)
            {
                return var->value.char_value != '\0';
            }
        }
    }
    else if (node->type == NODE_INT_LITERAL)
    {
        return atoi(node->value) != 0;
    }

    // For any other expression, evaluate it and convert to boolean
    int result = evaluate_expression(node);
    return result != 0;
}

char *evaluate_string_expression(ASTNode *node)
{
    if (node == NULL) return strdup("");

    if (node->type == NODE_STRING_LITERAL)
    {
        return strdup(node->value);
    }
    else if (node->type == NODE_FUNCTION_CALL)
    {
        char *result = execute_function(node->function_name, node->arguments);
        return result ? result : strdup("");
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

static bool can_implicitly_convert(VariableType from_type, VariableType to_type) {
    switch (to_type) {
        case FLOAT_TYPE:
            return from_type == INT_TYPE || from_type == CHAR_TYPE;
        case INT_TYPE:
            return from_type == CHAR_TYPE || from_type == BOOL_TYPE;
        case STRING_TYPE:
            return false;  // No implicit conversion to string
        case CHAR_TYPE:
            return false;  // No implicit conversion to char
        case BOOL_TYPE:
            return false;  // No implicit conversion to bool
        default:
            return false;
    }
}

static const char* type_to_string(VariableType type) {
    switch (type) {
        case INT_TYPE: return "int";
        case FLOAT_TYPE: return "float";
        case STRING_TYPE: return "string";
        case BOOL_TYPE: return "boolean";
        case CHAR_TYPE: return "char";
        default: return "unknown";
    }
}

// This is the main function that interprets our AST
void interpret(ASTNode *node)
{
    while (node)
    {
        switch (node->type)
        {
            case NODE_FUNCTION_CALL:
            {
                // Only execute the function, don't print the result
                char *result = execute_function(node->function_name, node->arguments);
                if (result) {
                    free(result);
                }
                break;
            }
            case NODE_PRINT:
            {
                if (node->left->type == NODE_FUNCTION_CALL)
                {
                    Function *func = find_function(node->left->function_name);
                    if (!func) {
                        printf("Error: Undefined function '%s'\n", node->left->function_name);
                        exit(1);
                    }

                    char *result = execute_function(node->left->function_name, node->left->arguments);
                    if (result) {
                        printf("%s\n", result);
                        free(result);
                    }
                }
                else if (node->left->type == NODE_FLOAT_LITERAL ||
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
                        strcmp(node->left->value, "&&") == 0 ||
                        strcmp(node->left->value, "||") == 0 ||
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

                if (node->left->type == NODE_FUNCTION_CALL)
                {
                    Function *func = find_function(node->left->function_name);
                    if (!func) {
                        printf("Error: Undefined function '%s'\n", node->left->function_name);
                        exit(1);
                    }

                    // Get function's return type
                    VariableType func_return_type = get_function_return_type(func->return_type);
                    
                    // Check if return type is compatible with variable type
                    if (func_return_type != var->type && !can_implicitly_convert(func_return_type, var->type)) {
                        printf("Error: Type mismatch - Cannot assign return value of function '%s' (%s) to variable '%s' (%s)\n",
                               node->left->function_name, type_to_string(func_return_type), 
                               node->var_name, type_to_string(var->type));
                        exit(1);
                    }

                    char *result = execute_function(node->left->function_name, node->left->arguments);
                    if (result)
                    {
                        if (var->type == STRING_TYPE)
                        {
                            set_variable(node->var_name, STRING_TYPE, result);
                        }
                        else if (var->type == INT_TYPE)
                        {
                            int value = atoi(result);
                            set_variable(node->var_name, INT_TYPE, &value);
                        }
                        else if (var->type == FLOAT_TYPE)
                        {
                            double value = atof(result);
                            set_variable(node->var_name, FLOAT_TYPE, &value);
                        }
                        else if (var->type == BOOL_TYPE)
                        {
                            bool value = strtobool(result);
                            set_variable(node->var_name, BOOL_TYPE, &value);
                        }
                        else if (var->type == CHAR_TYPE)
                        {
                            char value = result[0];
                            set_variable(node->var_name, CHAR_TYPE, &value);
                        }
                        free(result);
                    }
                }
                else if (var->type == FLOAT_TYPE)
                {
                    VariableType expr_type;
                    
                    // Determine expression type
                    if (node->left->type == NODE_STRING_LITERAL) {
                        expr_type = STRING_TYPE;
                    } else if (node->left->type == NODE_FLOAT_LITERAL) {
                        expr_type = FLOAT_TYPE;
                    } else if (node->left->type == NODE_BOOL_LITERAL) {
                        expr_type = BOOL_TYPE;
                    } else if (node->left->type == NODE_CHAR_LITERAL) {
                        expr_type = CHAR_TYPE;
                    } else if (node->left->type == NODE_LITERAL && get_variable(node->left->value)) {
                        expr_type = get_variable(node->left->value)->type;
                    } else {
                        expr_type = INT_TYPE;  // Default for numeric expressions
                    }

                    // Check type compatibility
                    if (expr_type != FLOAT_TYPE && !can_implicitly_convert(expr_type, FLOAT_TYPE)) {
                        printf("Error: Type mismatch - Cannot convert from '%s' to 'float' for variable '%s'\n", 
                               type_to_string(expr_type), node->var_name);
                        exit(1);
                    }

                    // Handle the conversion
                    if (expr_type == INT_TYPE) {
                        int int_val = evaluate_expression(node->left);
                        double value = (double)int_val;
                        set_variable(node->var_name, FLOAT_TYPE, &value);
                    } else {
                        double value = evaluate_float_expression(node->left);
                        set_variable(node->var_name, FLOAT_TYPE, &value);
                    }
                }
                else if (var->type == INT_TYPE)
                {
                    VariableType expr_type;
                    
                    // Determine expression type
                    if (node->left->type == NODE_STRING_LITERAL) {
                        expr_type = STRING_TYPE;
                    } else if (node->left->type == NODE_FLOAT_LITERAL) {
                        expr_type = FLOAT_TYPE;
                    } else if (node->left->type == NODE_BOOL_LITERAL) {
                        expr_type = BOOL_TYPE;
                    } else if (node->left->type == NODE_CHAR_LITERAL) {
                        expr_type = CHAR_TYPE;
                    } else if (node->left->type == NODE_LITERAL && get_variable(node->left->value)) {
                        expr_type = get_variable(node->left->value)->type;
                    } else {
                        expr_type = INT_TYPE;  // Default for numeric expressions
                    }

                    // Check type compatibility
                    if (expr_type != INT_TYPE && !can_implicitly_convert(expr_type, INT_TYPE)) {
                        printf("Error: Type mismatch - Cannot convert from '%s' to 'int' for variable '%s'\n", 
                               type_to_string(expr_type), node->var_name);
                        exit(1);
                    }

                    // Handle the conversion
                    if (expr_type == BOOL_TYPE) {
                        bool bool_val = evaluate_bool_expression(node->left);
                        int value = bool_val ? 1 : 0;
                        set_variable(node->var_name, INT_TYPE, &value);
                    } else if (expr_type == CHAR_TYPE) {
                        char char_val = node->left->value[0];
                        int value = (int)char_val;
                        set_variable(node->var_name, INT_TYPE, &value);
                    } else {
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
                    if (node->left->type != NODE_STRING_LITERAL && 
                        !(node->left->type == NODE_LITERAL && 
                          get_variable(node->left->value)->type == STRING_TYPE) &&
                        !(node->left->type == NODE_BINARY_OP && strcmp(node->left->value, "+") == 0)) {
                        printf("Error: Type mismatch - Cannot convert to string for variable '%s'. Use string concatenation (+) for conversion.\n", 
                               node->var_name);
                        exit(1);
                    }
                    char *value = evaluate_string_expression(node->left);
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

                // Save variable count before loop
                int saved_var_count = variable_count;
                int loop_var_count = variable_count;

                // Loop while condition is true
                while (evaluate_bool_expression(node->condition))
                {
                    // Execute body
                    interpret(node->body);
                    
                    // Execute increment
                    interpret(node->increment);

                    // Restore loop variables to their state after initialization
                    // but keep any variables created inside the loop
                    for (int i = saved_var_count; i < loop_var_count; i++)
                    {
                        Variable *var = &variables[i];
                        // Find the original variable
                        for (int j = loop_var_count; j < variable_count; j++)
                        {
                            if (strcmp(variables[j].name, var->name) == 0)
                            {
                                // Copy the value back
                                if (var->type == INT_TYPE)
                                {
                                    var->value.int_value = variables[j].value.int_value;
                                }
                                else if (var->type == FLOAT_TYPE)
                                {
                                    var->value.float_value = variables[j].value.float_value;
                                }
                                else if (var->type == BOOL_TYPE)
                                {
                                    var->value.bool_value = variables[j].value.bool_value;
                                }
                                else if (var->type == CHAR_TYPE)
                                {
                                    var->value.char_value = variables[j].value.char_value;
                                }
                                else if (var->type == STRING_TYPE)
                                {
                                    free(var->value.string_value);
                                    var->value.string_value = strdup(variables[j].value.string_value);
                                }
                                break;
                            }
                        }
                    }

                    // Clean up variables created inside the loop
                    while (variable_count > loop_var_count)
                    {
                        variable_count--;
                        free(variables[variable_count].name);
                        if (variables[variable_count].type == STRING_TYPE)
                        {
                            free(variables[variable_count].value.string_value);
                        }
                    }
                }

                // Restore variable count after loop (clean up loop scope)
                while (variable_count > saved_var_count)
                {
                    variable_count--;
                    free(variables[variable_count].name);
                    if (variables[variable_count].type == STRING_TYPE)
                    {
                        free(variables[variable_count].value.string_value);
                    }
                }
                break;
            }
            case NODE_FUNCTION_DEFINITION:
                register_function(node->function_name, node->return_type, node->parameters, node->body);
                break;
                
            case NODE_VAR_DECLARATION:
            {
                if (node->left && node->left->type == NODE_FUNCTION_CALL)
                {
                    Function *func = find_function(node->left->function_name);
                    if (!func) {
                        printf("Error: Undefined function '%s'\n", node->left->function_name);
                        exit(1);
                    }

                    // Get function's return type
                    VariableType func_return_type = get_function_return_type(func->return_type);
                    
                    // Check if return type is compatible with variable type
                    if (func_return_type != get_type_from_string(node->var_type) && !can_implicitly_convert(func_return_type, get_type_from_string(node->var_type))) {
                        printf("Error: Type mismatch - Cannot assign return value of function '%s' (%s) to variable '%s' (%s)\n",
                               node->left->function_name, type_to_string(func_return_type), 
                               node->var_name, type_to_string(get_type_from_string(node->var_type)));
                        exit(1);
                    }

                    char *result = execute_function(node->left->function_name, node->left->arguments);
                    if (result)
                    {
                        if (strcmp(node->var_type, "string") == 0)
                        {
                            set_variable(node->var_name, STRING_TYPE, result);
                        }
                        else if (strcmp(node->var_type, "int") == 0)
                        {
                            int value = atoi(result);
                            set_variable(node->var_name, INT_TYPE, &value);
                        }
                        else if (strcmp(node->var_type, "float") == 0)
                        {
                            double value = atof(result);
                            set_variable(node->var_name, FLOAT_TYPE, &value);
                        }
                        else if (strcmp(node->var_type, "boolean") == 0)
                        {
                            bool value = strtobool(result);
                            set_variable(node->var_name, BOOL_TYPE, &value);
                        }
                        else if (strcmp(node->var_type, "char") == 0)
                        {
                            char value = result[0];
                            set_variable(node->var_name, CHAR_TYPE, &value);
                        }
                        free(result);
                    }
                }
                else if (strcmp(node->var_type, "float") == 0)
                {
                    VariableType expr_type;
                    
                    // Determine expression type
                    if (node->left->type == NODE_STRING_LITERAL) {
                        expr_type = STRING_TYPE;
                    } else if (node->left->type == NODE_FLOAT_LITERAL) {
                        expr_type = FLOAT_TYPE;
                    } else if (node->left->type == NODE_BOOL_LITERAL) {
                        expr_type = BOOL_TYPE;
                    } else if (node->left->type == NODE_CHAR_LITERAL) {
                        expr_type = CHAR_TYPE;
                    } else if (node->left->type == NODE_LITERAL && get_variable(node->left->value)) {
                        expr_type = get_variable(node->left->value)->type;
                    } else {
                        expr_type = INT_TYPE;  // Default for numeric expressions
                    }

                    // Check type compatibility
                    if (expr_type != FLOAT_TYPE && !can_implicitly_convert(expr_type, FLOAT_TYPE)) {
                        printf("Error: Type mismatch - Cannot convert from '%s' to 'float' for variable '%s'\n", 
                               type_to_string(expr_type), node->var_name);
                        exit(1);
                    }

                    // Handle the conversion
                    if (expr_type == INT_TYPE) {
                        int int_val = evaluate_expression(node->left);
                        double value = (double)int_val;
                        set_variable(node->var_name, FLOAT_TYPE, &value);
                    } else {
                        double value = evaluate_float_expression(node->left);
                        set_variable(node->var_name, FLOAT_TYPE, &value);
                    }
                }
                else if (strcmp(node->var_type, "int") == 0)
                {
                    VariableType expr_type;
                    
                    // Determine expression type
                    if (node->left->type == NODE_STRING_LITERAL) {
                        expr_type = STRING_TYPE;
                    } else if (node->left->type == NODE_FLOAT_LITERAL) {
                        expr_type = FLOAT_TYPE;
                    } else if (node->left->type == NODE_BOOL_LITERAL) {
                        expr_type = BOOL_TYPE;
                    } else if (node->left->type == NODE_CHAR_LITERAL) {
                        expr_type = CHAR_TYPE;
                    } else if (node->left->type == NODE_LITERAL && get_variable(node->left->value)) {
                        expr_type = get_variable(node->left->value)->type;
                    } else {
                        expr_type = INT_TYPE;  // Default for numeric expressions
                    }

                    // Check type compatibility
                    if (expr_type != INT_TYPE && !can_implicitly_convert(expr_type, INT_TYPE)) {
                        printf("Error: Type mismatch - Cannot convert from '%s' to 'int' for variable '%s'\n", 
                               type_to_string(expr_type), node->var_name);
                        exit(1);
                    }

                    // Handle the conversion
                    if (expr_type == BOOL_TYPE) {
                        bool bool_val = evaluate_bool_expression(node->left);
                        int value = bool_val ? 1 : 0;
                        set_variable(node->var_name, INT_TYPE, &value);
                    } else if (expr_type == CHAR_TYPE) {
                        char char_val = node->left->value[0];
                        int value = (int)char_val;
                        set_variable(node->var_name, INT_TYPE, &value);
                    } else {
                        int value = evaluate_expression(node->left);
                        set_variable(node->var_name, INT_TYPE, &value);
                    }
                }
                else if (strcmp(node->var_type, "boolean") == 0)
                {
                    bool value = node->left ? evaluate_bool_expression(node->left) : false;
                    set_variable(node->var_name, BOOL_TYPE, &value);
                }
                else if (strcmp(node->var_type, "string") == 0)
                {
                    if (node->left->type != NODE_STRING_LITERAL && 
                        !(node->left->type == NODE_LITERAL && 
                          get_variable(node->left->value)->type == STRING_TYPE) &&
                        !(node->left->type == NODE_BINARY_OP && strcmp(node->left->value, "+") == 0)) {
                        printf("Error: Type mismatch - Cannot convert to string for variable '%s'. Use string concatenation (+) for conversion.\n", 
                               node->var_name);
                        exit(1);
                    }
                    char *value = evaluate_string_expression(node->left);
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
            default:
                break;
        }
        node = node->next;
    }
}
