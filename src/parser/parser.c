#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include <limits.h>

// These are function declarations. They tell the compiler that these functions will be defined later.
static ASTNode *parse_statement(Parser *parser);
static ASTNode *parse_assignment(Parser *parser);
static ASTNode *parse_expression(Parser *parser);
static ASTNode *parse_power(Parser *parser);
static ASTNode *parse_term(Parser *parser);
static ASTNode *parse_factor(Parser *parser);
static ASTNode *parse_if_statement(Parser *parser);
static ASTNode *parse_ternary(Parser *parser)  __attribute__((unused));
static ASTNode *parse_var_declaration(Parser *parser);
static ASTNode *parse_print(Parser *parser);
static ASTNode *parse_for_statement(Parser *parser);
static ASTNode *parse_logical_or(Parser *parser);
static ASTNode *parse_logical_and(Parser *parser);
static ASTNode *parse_comparison(Parser *parser);
static ASTNode *parse_function_definition(Parser *parser);
static ASTNode *parse_function_parameters(Parser *parser);
static ASTNode *parse_function_body(Parser *parser) __attribute__((unused));
static ASTNode *parse_yield_statement(Parser *parser);
static ASTNode *parse_function_call(Parser *parser);
static ASTNode *parse_array_declaration(Parser *parser);
static ASTNode *parse_array_literal(Parser *parser);
static ASTNode *parse_array_access(Parser *parser, char *array_name);
static ASTNode *parse_array_method_call(Parser *parser, char *array_name);
static ASTNode *parse_input(Parser *parser);

// This function is used to get the next token from the lexer
static Token *get_next_token(Parser *parser)
{
    // If there's a current token, free its memory
    if (parser->current_token)
    {
        if (parser->current_token->value)
        {
            free(parser->current_token->value);
        }
        free(parser->current_token);
    }
    // Get the next token from the lexer and return it
    parser->current_token = next_token(parser->lexer);
    return parser->current_token;
}

static ASTNode *parse_print_statement(Parser *parser) __attribute__((unused));

// This function is used to parse a print statement
static ASTNode *parse_print_statement(Parser *parser)
{
    get_next_token(parser); // Consume PRINT token

    // Check if the next token is an opening parenthesis
    if (parser->current_token->type != TOKEN_LPAREN)
    {
        printf("Error: Expected '(' after PRINT.\n");
        return NULL;
    }
    // Move past the opening parenthesis
    get_next_token(parser);

    // Parse the expression inside the parentheses
    ASTNode *expr = parse_expression(parser);
    if (!expr)
    {
        // If parsing the expression fails, return NULL
        return NULL;
    }

    // Check if the next token is a closing parenthesis
    if (parser->current_token->type != TOKEN_RPAREN)
    {
        printf("Error: Expected ')' after expression in PRINT statement.\n");
        return NULL;
    }

    // Move past the closing parenthesis
    get_next_token(parser);

    // Check if the statement ends with a semicolon
    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        printf("Error: Expected ';' after PRINT statement.\n");
        return NULL;
    }

    // Move past the semicolon
    get_next_token(parser);

    // Create and return a new AST node for the print statement
    return create_node(NODE_PRINT, expr, NULL, NULL);
}

// This function parses all the tokens and builds the AST
ASTNode *parse_tokens(Parser *parser)
{
    if (!parser || !parser->current_token)
    {
        printf("Error: Parser or current token is NULL\n");
        return NULL;
    }
    
    // printf("DEBUG: Starting parse_tokens. First token type: %d, value: %s\n", 
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");
    
    ASTNode *root = NULL;
    ASTNode *current = NULL;
    
    while (parser->current_token->type != TOKEN_EOF)
    {
        ASTNode *statement = parse_statement(parser);
        if (!statement)
        {
            // printf("DEBUG: Failed to parse statement\n");
            free_ast(root);
            return NULL;
        }
        
        if (!root)
        {
            root = statement;
            current = root;
        }
        else
        {
            current->next = statement;
            current = statement;
        }
    }
    
    return root;
}

static ASTNode *parse_statement(Parser *parser)
{
    // printf("DEBUG: Parsing statement, current token type: %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    ASTNode *node = NULL;

    switch (parser->current_token->type)
    {
        case TOKEN_DEFINE:
            // printf("[DEBUG] Found function definition\n");
            node = parse_function_definition(parser);
            break;

        case TOKEN_PRINT:
            // printf("[DEBUG] Found print statement\n");
            node = parse_print(parser);
            break;

        case TOKEN_IF:
            // printf("[DEBUG] Found if statement\n");
            node = parse_if_statement(parser);
            break;

        case TOKEN_FOR:
            // printf("[DEBUG] Found for statement\n");
            node = parse_for_statement(parser);
            break;

        case TOKEN_INT_TYPE:
        case TOKEN_FLOAT_TYPE:
        case TOKEN_STRING_TYPE:
        case TOKEN_CHAR_TYPE:
        case TOKEN_BOOL_TYPE:
            // printf("[DEBUG] Found variable declaration\n");
            node = parse_var_declaration(parser);
            break;

        case TOKEN_IDENTIFIER:
        {
            // printf("DEBUG: Found identifier: %s\n", parser->current_token->value);
            char next_char = peek_next_non_whitespace(parser->lexer);
            // printf("DEBUG: Next non-whitespace character after identifier: '%c'\n", next_char);
            
            if (next_char == '(')
            {
                node = parse_function_call(parser);
            }
            else if (next_char == '[')
            {
                node = parse_array_access(parser, parser->current_token->value);
            }
            else if (next_char == '.')
            {
                node = parse_array_method_call(parser, parser->current_token->value);
            }
            else if (next_char == '<')
            {
                // printf("DEBUG: Detected array declaration syntax\n");
                node = parse_array_declaration(parser);
            }
            else
            {
                // printf("DEBUG: Treating as regular assignment\n");
                node = parse_assignment(parser);
            }
            
            if (!node) {
                printf("Error: Failed to parse identifier expression\n");
            }
            break;
        }

        case TOKEN_YIELD:
            // printf("[DEBUG] Found yield statement\n");
            node = parse_yield_statement(parser);
            break;

        case TOKEN_ARRAY_TYPE:
            node = parse_array_declaration(parser);
            break;

        case TOKEN_EOF:
            // printf("[DEBUG] Found EOF\n");
            return NULL;

        default:
            // printf("[DEBUG] Error: Unexpected token type %d with value '%s'\n",
                //    parser->current_token->type,
                //    parser->current_token->value ? parser->current_token->value : "NULL");
            get_next_token(parser); // Advance past the unexpected token
            return NULL;
    }

    if (!node) {
        printf("Error: Failed to create node for statement\n");
    }

    return node;
}

// This function creates a new parser with the given lexer
Parser *create_parser(Lexer *lexer)
{
    // printf("[DEBUG] Creating parser\n");
    
    if (!lexer) {
        // printf("[DEBUG] Error: Null lexer passed to create_parser\n");
        return NULL;
    }
    
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    if (!parser) {
        // printf("[DEBUG] Error: Failed to allocate memory for parser\n");
        return NULL;
    }

    parser->lexer = lexer;
    parser->current_token = next_token(lexer);

    if (!parser->current_token) {
        // printf("[DEBUG] Error: Failed to get first token\n");
        free(parser);
        return NULL;
    }

    // printf("[DEBUG] Parser created. First token type: %d, value: '%s'\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    return parser;
}

// This function frees the memory allocated for the parser
void free_parser(Parser *parser)
{
    if (parser)
    {
        // Free the current token if it exists
        if (parser->current_token)
        {
            if (parser->current_token->value)
            {
                free(parser->current_token->value);
            }
            free(parser->current_token);
        }

        // Free the parser itself
        free(parser);
    }
}

// This function parses a single statement from the source code
static ASTNode *parse_assignment(Parser *parser)
{
    if (parser->current_token->type != TOKEN_IDENTIFIER)
    {
        return NULL;
    }

    char *var_name = strdup(parser->current_token->value);
    get_next_token(parser);

    TokenType assign_type = parser->current_token->type;
    if (assign_type != TOKEN_ASSIGN && 
        assign_type != TOKEN_PLUS_ASSIGN && 
        assign_type != TOKEN_MINUS_ASSIGN && 
        assign_type != TOKEN_MUL_ASSIGN && 
        assign_type != TOKEN_DIV_ASSIGN && 
        assign_type != TOKEN_MOD_ASSIGN)
    {
        free(var_name);
        return NULL;
    }

    char *op = NULL;
    if (assign_type == TOKEN_PLUS_ASSIGN) op = "+";
    else if (assign_type == TOKEN_MINUS_ASSIGN) op = "-";
    else if (assign_type == TOKEN_MUL_ASSIGN) op = "*";
    else if (assign_type == TOKEN_DIV_ASSIGN) op = "/";
    else if (assign_type == TOKEN_MOD_ASSIGN) op = "%";

    get_next_token(parser); // consume assignment operator
    
    // Parse the full expression
    ASTNode *value = NULL;
    if (parser->current_token->type == TOKEN_BOOL)
    {
        value = create_node(NODE_BOOL_LITERAL, NULL, NULL, parser->current_token->value);
        get_next_token(parser);
    } 
    else 
    {
        value = parse_expression(parser);
    }

    if (!value)
    {
        free(var_name);
        return NULL;
    }

    if (op) {
        // For compound assignments (+=, -=, etc.), create a binary operation node
        ASTNode *var_node = create_node(NODE_LITERAL, NULL, NULL, var_name);
        ASTNode *op_node = create_node(NODE_BINARY_OP, var_node, value, op);
        return create_assignment_node(var_name, op_node);
    } else {
        // For simple assignment (=)
        return create_assignment_node(var_name, value);
    }
}

static ASTNode *parse_expression(Parser *parser)
{
    // printf("[DEBUG] Parsing expression, current token type: %d, value: '%s'\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    ASTNode *left = parse_logical_or(parser);  // Start with logical OR

    while (parser->current_token->type == TOKEN_PLUS || 
           parser->current_token->type == TOKEN_MINUS)
    {
        Token *op_token = parser->current_token;
        char *op = op_token->type == TOKEN_PLUS ? "+" : "-";
        
        get_next_token(parser);
        ASTNode *right = parse_logical_or(parser);
        left = create_node(NODE_BINARY_OP, left, right, op);
    }

    return left;
}

// Parse expressions with logical OR (||)
static ASTNode *parse_logical_or(Parser *parser)
{
    ASTNode *left = parse_logical_and(parser);

    while (parser->current_token->type == TOKEN_LOGICAL_OR)
    {
        get_next_token(parser); // consume '||'
        ASTNode *right = parse_logical_and(parser);
        left = create_node(NODE_BINARY_OP, left, right, "||");
    }

    return left;
}

// Parse expressions with logical AND (&&)
static ASTNode *parse_logical_and(Parser *parser)
{
    ASTNode *left = parse_comparison(parser);

    while (parser->current_token->type == TOKEN_LOGICAL_AND)
    {
        get_next_token(parser); // consume '&&'
        ASTNode *right = parse_comparison(parser);
        left = create_node(NODE_BINARY_OP, left, right, "&&");
    }

    return left;
}

// Parse comparison expressions (<, >, ==, etc.)
static ASTNode *parse_comparison(Parser *parser)
{
    ASTNode *left = parse_term(parser);

    while (parser->current_token->type == TOKEN_EQUAL ||
           parser->current_token->type == TOKEN_NOT_EQUAL ||
           parser->current_token->type == TOKEN_LESS_THAN ||
           parser->current_token->type == TOKEN_GREATER_THAN ||
           parser->current_token->type == TOKEN_LESS_THAN_OR_EQUAL ||
           parser->current_token->type == TOKEN_GREATER_THAN_OR_EQUAL)
    {
        char *op = NULL;
        switch (parser->current_token->type) {
            case TOKEN_EQUAL: op = "=="; break;
            case TOKEN_NOT_EQUAL: op = "!="; break;
            case TOKEN_LESS_THAN: op = "<"; break;
            case TOKEN_GREATER_THAN: op = ">"; break;
            case TOKEN_LESS_THAN_OR_EQUAL: op = "<="; break;
            case TOKEN_GREATER_THAN_OR_EQUAL: op = ">="; break;
            default: op = "=="; break;
        }

        get_next_token(parser);
        ASTNode *right = parse_term(parser);
        left = create_node(NODE_BINARY_OP, left, right, op);
    }

    return left;
}

static ASTNode *parse_term(Parser *parser)
{
    ASTNode *left = parse_power(parser);

    while (parser->current_token->type == TOKEN_MULTIPLY || 
           parser->current_token->type == TOKEN_DIVIDE ||
           parser->current_token->type == TOKEN_MODULUS)
    {
        Token *op_token = parser->current_token;
        char *op = NULL;  // Initialize to NULL to fix warning
        
        if (op_token->type == TOKEN_MULTIPLY) 
            op = "*";
        else if (op_token->type == TOKEN_DIVIDE) 
            op = "/";
        else if (op_token->type == TOKEN_MODULUS) 
            op = "%";
        
        if (op == NULL) {  // Safety check
            printf("Error: Unknown operator\n");
            return left;
        }
        
        get_next_token(parser);
        ASTNode *right = parse_power(parser);
        left = create_node(NODE_BINARY_OP, left, right, op);
    }

    return left;
}

static ASTNode *parse_power(Parser *parser)
{
    // printf("[DEBUG] Parsing power expression\n");
    ASTNode *left = parse_factor(parser);

    while (parser->current_token->type == TOKEN_POWER)
    {
        get_next_token(parser); // consume **
        ASTNode *right = parse_factor(parser);
        left = create_node(NODE_BINARY_OP, left, right, "**");
    }

    return left;
}

static ASTNode *parse_factor(Parser *parser)
{
    Token *token = parser->current_token;
    
    if (token->type == TOKEN_LENGTH || token->type == TOKEN_INDEX || 
        (token->type == TOKEN_IDENTIFIER && peek_char(parser->lexer) == '(')) {
        char *func_name = strdup(token->value);
        get_next_token(parser);
        
        if (parser->current_token->type != TOKEN_LPAREN) {
            printf("Error: Expected '(' after function name\n");
            free(func_name);
            return NULL;
        }
        get_next_token(parser);
        
        ASTNode *argument = NULL;
        ASTNode *current_arg = NULL;

        // Parse arguments until we hit the closing parenthesis
        while (parser->current_token->type != TOKEN_RPAREN) {
            ASTNode *next_arg = parse_expression(parser);
            if (!next_arg) {
                printf("Error: Invalid argument in function call\n");
                free(func_name);
                if (argument) free_ast(argument);
                return NULL;
            }

            if (!argument) {
                argument = next_arg;
                current_arg = argument;
            } else {
                current_arg->next = next_arg;
                current_arg = next_arg;
            }

            // Check for comma if there might be more arguments
            if (parser->current_token->type == TOKEN_COMMA) {
                get_next_token(parser); // consume comma
            } else if (parser->current_token->type != TOKEN_RPAREN) {
                printf("Error: Expected ',' or ')' after function argument\n");
                free(func_name);
                free_ast(argument);
                return NULL;
            }
        }
        
        if (parser->current_token->type != TOKEN_RPAREN) {
            printf("Error: Expected ')' after function argument\n");
            free(func_name);
            if (argument) free_ast(argument);
            return NULL;
        }
        get_next_token(parser);
        
        ASTNode *node = create_function_call_node(func_name, argument);
        free(func_name);
        return node;
    }
    
    // Add handling for unary minus
    if (token->type == TOKEN_MINUS)
    {
        get_next_token(parser); // consume '-'
        ASTNode *operand = parse_factor(parser);
        if (!operand) {
            return NULL;
        }
        // Create a binary operation node that multiplies by -1
        ASTNode *minus_one = create_node(NODE_INT_LITERAL, NULL, NULL, "-1");
        return create_node(NODE_BINARY_OP, minus_one, operand, "*");
    }

    if (token->type == TOKEN_NOT)
    {
        get_next_token(parser); // consume '!'
        ASTNode *operand = parse_factor(parser);
        if (!operand) {
            // printf("Error: Expected expression after '!'\n");
            return NULL;
        }
        return create_node(NODE_BINARY_OP, operand, NULL, "!");
    }
    else if (token->type == TOKEN_NUMBER || token->type == TOKEN_FLOAT)
    {
        ASTNode *node = create_node(
            token->type == TOKEN_FLOAT ? NODE_FLOAT_LITERAL : NODE_INT_LITERAL,
            NULL, NULL, token->value
        );
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_STRING)
    {
        ASTNode *node = create_node(NODE_STRING_LITERAL, NULL, NULL, token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_CHAR)
    {
        ASTNode *node = create_node(NODE_CHAR_LITERAL, NULL, NULL, token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_IDENTIFIER)
    {
        char *identifier = strdup(token->value);
        get_next_token(parser);  // consume identifier
        
        // Check if this is a function call
        if (parser->current_token->type == TOKEN_LPAREN)
        {
            get_next_token(parser);  // consume '('
            
            // Parse arguments
            ASTNode *args = NULL;
            ASTNode *current_arg = NULL;
            
            // Handle empty argument list
            if (parser->current_token->type != TOKEN_RPAREN)
            {
                // Parse first argument
                args = parse_expression(parser);
                if (!args)
                {
                    free(identifier);
                    return NULL;
                }
                current_arg = args;
                
                // Parse remaining arguments
                while (parser->current_token->type == TOKEN_COMMA)
                {
                    get_next_token(parser);  // consume comma
                    ASTNode *next_arg = parse_expression(parser);
                    if (!next_arg)
                    {
                        free(identifier);
                        return NULL;
                    }
                    current_arg->next = next_arg;
                    current_arg = next_arg;
                }
            }
            
            if (parser->current_token->type != TOKEN_RPAREN)
            {
                printf("Error: Expected ')' after function arguments\n");
                free(identifier);
                return NULL;
            }
            get_next_token(parser);  // consume ')'
            
            // Create function call node
            ASTNode *node = create_function_call_node(identifier, args);
            free(identifier);
            return node;
        }
        
        // Not a function call, just a regular identifier
        ASTNode *node = create_node(NODE_LITERAL, NULL, NULL, identifier);
        free(identifier);
        return node;
    }
    else if (token->type == TOKEN_LPAREN)
    {
        get_next_token(parser); // consume '('
        
        // Check if this is a type cast
        if (parser->current_token->value && 
            (strcmp(parser->current_token->value, "int") == 0 || 
             strcmp(parser->current_token->value, "float") == 0 || 
             strcmp(parser->current_token->value, "string") == 0 || 
             strcmp(parser->current_token->value, "boolean") == 0 ||
             strcmp(parser->current_token->value, "char") == 0))
        {
            char *target_type = strdup(parser->current_token->value);
            get_next_token(parser); // consume type
            
            if (parser->current_token->type != TOKEN_RPAREN)
            {
                printf("Error: Expected ')' after type in cast\n");
                free(target_type);
                return NULL;
            }
            get_next_token(parser); // consume ')'
            
            // Parse the expression to be cast
            ASTNode *expr = parse_factor(parser);
            if (!expr)
            {
                printf("Error: Invalid expression in type cast\n");
                free(target_type);
                return NULL;
            }

            // Validate boolean casts
            if (strcmp(target_type, "boolean") == 0) {
                if (expr->type == NODE_INT_LITERAL) {
                    int value = atoi(expr->value);
                    if (value != 0 && value != 1) {
                        printf("Error: Cannot cast %d to boolean. Only 0 and 1 are valid values.\n", value);
                        free(target_type);
                        free_ast(expr);
                        return NULL;
                    }
                }
            }
            
            return create_type_cast_node(target_type, expr);
        }
        
        // If it's not a type cast, parse as normal parentheses
        ASTNode *expr = parse_expression(parser);
        if (!expr)
        {
            return NULL;
        }

        if (parser->current_token->type != TOKEN_RPAREN)
        {
            printf("Error: Expected ')'\n");
            free_ast(expr);
            return NULL;
        }

        get_next_token(parser); // consume ')'
        return expr;
    }
    else if (token->type == TOKEN_BOOL)
    {
        ASTNode *node = create_node(NODE_BOOL_LITERAL, NULL, NULL, token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_INPUT)
    {
        return parse_input(parser);
    }

    // Add handling for length function
    if (token->type == TOKEN_LENGTH) {
        printf("DEBUG: Parsing length function call\n");
        get_next_token(parser); // consume 'length'
        
        // Check for opening parenthesis
        if (parser->current_token->type != TOKEN_LPAREN) {
            printf("Error: Expected '(' after length\n");
            return NULL;
        }
        get_next_token(parser); // consume '('
        
        // Parse the argument
        ASTNode *argument = parse_expression(parser);
        if (!argument) {
            printf("Error: Invalid argument to length function\n");
            return NULL;
        }
        
        // Check for closing parenthesis
        if (parser->current_token->type != TOKEN_RPAREN) {
            printf("Error: Expected ')' after length argument\n");
            free_ast(argument);
            return NULL;
        }
        get_next_token(parser); // consume ')'
        
        return create_function_call_node("length", argument);
    }

    printf("Error: Unexpected token in factor: %d\n", token->type);
    return NULL;
}

// Add parsing for if statements
static ASTNode *parse_if_statement(Parser *parser)
{
    get_next_token(parser); // consume 'if'

    if (parser->current_token->type != TOKEN_LPAREN)
    {
        printf("Error: Expected '(' after 'if'\n");
        return NULL;
    }
    get_next_token(parser); // consume '('

    ASTNode *condition = parse_logical_or(parser);
    if (!condition)
    {
        printf("Error: Invalid condition in if statement\n");
        return NULL;
    }

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        printf("Error: Expected ')' after if condition\n");
        free_ast(condition);
        return NULL;
    }
    get_next_token(parser); // consume ')'

    if (parser->current_token->type != TOKEN_LBRACE)
    {
        printf("Error: Expected '{' after if condition\n");
        return NULL;
    }
    get_next_token(parser); // consume '{'

    ASTNode *if_body = NULL;
    ASTNode *else_body = NULL; __attribute__((unused))
    ASTNode *current = NULL;

    // Parse the if body
    while (parser->current_token->type != TOKEN_RBRACE && 
           parser->current_token->type != TOKEN_EOF)
    {
        ASTNode *statement = parse_statement(parser);
        if (statement)
        {
            if (!if_body)
            {
                if_body = statement;
                current = if_body;
            }
            else
            {
                current->next = statement;
                current = statement;
            }
        }
    }

    if (parser->current_token->type != TOKEN_RBRACE)
    {
        printf("Error: Expected '}' at end of if body\n");
        return NULL;
    }
    get_next_token(parser); // consume '}'

    // Create the initial if node
    ASTNode *if_node = create_node(NODE_IF, condition, if_body, NULL);
    if_node->else_branch = NULL;

    // Check for else
    while (parser->current_token->type == TOKEN_ELSE)
    {
        get_next_token(parser); // consume 'else'
        
        // Check if this is an "else if"
        if (parser->current_token->type == TOKEN_IF)
        {
            get_next_token(parser); // consume 'if'

            if (parser->current_token->type != TOKEN_LPAREN)
            {
                printf("Error: Expected '(' after 'if'\n");
                return NULL;
            }
            get_next_token(parser); // consume '('

            ASTNode *else_if_condition = parse_logical_or(parser);
            if (!else_if_condition)
            {
                printf("Error: Invalid condition in else if statement\n");
                return NULL;
            }

            if (parser->current_token->type != TOKEN_RPAREN)
            {
                printf("Error: Expected ')' after else if condition\n");
                free_ast(else_if_condition);
                return NULL;
            }
            get_next_token(parser); // consume ')'

            if (parser->current_token->type != TOKEN_LBRACE)
            {
                printf("Error: Expected '{' after else if condition\n");
                return NULL;
            }
            get_next_token(parser); // consume '{'

            ASTNode *else_if_body = NULL;
            current = NULL;

            // Parse the else if body
            while (parser->current_token->type != TOKEN_RBRACE && 
                   parser->current_token->type != TOKEN_EOF)
            {
                ASTNode *statement = parse_statement(parser);
                if (statement)
                {
                    if (!else_if_body)
                    {
                        else_if_body = statement;
                        current = else_if_body;
                    }
                    else
                    {
                        current->next = statement;
                        current = statement;
                    }
                }
            }

            if (parser->current_token->type != TOKEN_RBRACE)
            {
                printf("Error: Expected '}' at end of else if body\n");
                return NULL;
            }
            get_next_token(parser); // consume '}'

            // Create a new if node for the else if branch
            ASTNode *else_if_node = create_node(NODE_IF, else_if_condition, else_if_body, NULL);
            
            // Find the last if node in the chain
            ASTNode *last_if = if_node;
            while (last_if->else_branch != NULL && last_if->else_branch->type == NODE_IF)
            {
                last_if = last_if->else_branch;
            }
            
            // Add it as the else branch of the last if node
            last_if->else_branch = else_if_node;
        }
        else // Just else
        {
            if (parser->current_token->type != TOKEN_LBRACE)
            {
                printf("Error: Expected '{' after else\n");
                return NULL;
            }
            get_next_token(parser); // consume '{'

            ASTNode *else_body = NULL;
            current = NULL;

            while (parser->current_token->type != TOKEN_RBRACE && 
                   parser->current_token->type != TOKEN_EOF)
            {
                ASTNode *statement = parse_statement(parser);
                if (statement)
                {
                    if (!else_body)
                    {
                        else_body = statement;
                        current = else_body;
                    }
                    else
                    {
                        current->next = statement;
                        current = statement;
                    }
                }
            }

            if (parser->current_token->type != TOKEN_RBRACE)
            {
                printf("Error: Expected '}' at end of else body\n");
                return NULL;
            }
            get_next_token(parser); // consume '}'

            // Find the last if node in the chain
            ASTNode *last_if = if_node;
            while (last_if->else_branch != NULL && last_if->else_branch->type == NODE_IF)
            {
                last_if = last_if->else_branch;
            }
            
            // Add the else body to the last if node
            last_if->else_branch = else_body;
            break;
        }
    }

    return if_node;
}

// Add parsing for ternary operators
static ASTNode *parse_ternary(Parser *parser)
{
    ASTNode *condition = parse_expression(parser);

    if (parser->current_token->type != TOKEN_QUESTION)
    {
        return condition;
    }

    get_next_token(parser); // consume '?'
    ASTNode *true_expr = parse_expression(parser);

    if (parser->current_token->type != TOKEN_COLON)
    {
        printf("Error: Expected ':' in ternary expression\n");
        return NULL;
    }
    get_next_token(parser); // consume ':'

    ASTNode *false_expr = parse_expression(parser);

    ASTNode *ternary = create_node(NODE_TERNARY, condition, true_expr, NULL);
    ternary->else_branch = false_expr;
    return ternary;
}

static ASTNode *parse_for_statement(Parser *parser)
{
    get_next_token(parser); // consume 'for'

    if (parser->current_token->type != TOKEN_LPAREN)
    {
        printf("Error: Expected '(' after 'for'\n");
        return NULL;
    }
    get_next_token(parser); // consume '('

    // Parse initialisation
    ASTNode *init = NULL;
    if (parser->current_token->type == TOKEN_INT_TYPE ||
        parser->current_token->type == TOKEN_FLOAT_TYPE ||
        parser->current_token->type == TOKEN_STRING_TYPE ||
        parser->current_token->type == TOKEN_BOOL_TYPE)
    {
        init = parse_var_declaration(parser);
    }
    else if (parser->current_token->type == TOKEN_IDENTIFIER)
    {
        init = parse_assignment(parser);
        if (parser->current_token->type == TOKEN_SEMICOLON)
        {
            get_next_token(parser); // consume ';'
        }
    }

    if (!init)
    {
        printf("Error: Invalid for loop initialization\n");
        return NULL;
    }

    // Parse condition
    ASTNode *condition = parse_expression(parser);
    if (!condition)
    {
        printf("Error: Invalid for loop condition\n");
        free_ast(init);
        return NULL;
    }

    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        printf("Error: Expected ';' after for loop condition\n");
        free_ast(init);
        free_ast(condition);
        return NULL;
    }
    get_next_token(parser); // consume ';'

    // Parse increment
    ASTNode *increment = NULL;
    if (parser->current_token->type == TOKEN_IDENTIFIER)
    {
        // Handle both assignment and increment/decrement operators
        char *var_name = strdup(parser->current_token->value);
        get_next_token(parser);

        if (parser->current_token->type == TOKEN_INCREMENT)
        {
            get_next_token(parser); // consume '++'
            // Create a node that represents: var_name = var_name + 1
            ASTNode *var_node = create_node(NODE_LITERAL, NULL, NULL, var_name);
            ASTNode *one = create_node(NODE_INT_LITERAL, NULL, NULL, "1");
            ASTNode *add = create_node(NODE_BINARY_OP, var_node, one, "+");
            increment = create_assignment_node(var_name, add);
            free(var_name); // free the strdup'd name since create_assignment_node makes its own copy
        }
        else if (parser->current_token->type == TOKEN_DECREMENT)
        {
            get_next_token(parser); // consume '--'
            // Create a node that represents: var_name = var_name - 1
            ASTNode *var_node = create_node(NODE_LITERAL, NULL, NULL, var_name);
            ASTNode *one = create_node(NODE_INT_LITERAL, NULL, NULL, "1");
            ASTNode *sub = create_node(NODE_BINARY_OP, var_node, one, "-");
            increment = create_assignment_node(var_name, sub);
            free(var_name); // free the strdup'd name since create_assignment_node makes its own copy
        }
        else if (parser->current_token->type == TOKEN_ASSIGN)
        {
            get_next_token(parser); // consume '='
            ASTNode *value = parse_expression(parser);
            increment = create_assignment_node(var_name, value);
            free(var_name);
        }
    }

    if (!increment)
    {
        printf("Error: Invalid for loop increment\n");
        free_ast(init);
        free_ast(condition);
        return NULL;
    }

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        printf("Error: Expected ')' after for loop increment\n");
        free_ast(init);
        free_ast(condition);
        free_ast(increment);
        return NULL;
    }
    get_next_token(parser); // consume ')'

    if (parser->current_token->type != TOKEN_LBRACE)
    {
        printf("Error: Expected '{' after for loop header\n");
        free_ast(init);
        free_ast(condition);
        free_ast(increment);
        return NULL;
    }
    get_next_token(parser); // consume '{'

    // Parse body
    ASTNode *body = NULL;
    ASTNode *current = NULL;

    while (parser->current_token->type != TOKEN_RBRACE && 
           parser->current_token->type != TOKEN_EOF)
    {
        ASTNode *statement = parse_statement(parser);
        if (statement)
        {
            if (!body)
            {
                body = statement;
                current = body;
            }
            else
            {
                current->next = statement;
                current = statement;
            }
        }
    }

    if (parser->current_token->type != TOKEN_RBRACE)
    {
        printf("Error: Expected '}' at end of for loop body\n");
        free_ast(init);
        free_ast(condition);
        free_ast(increment);
        if (body) free_ast(body);
        return NULL;
    }
    get_next_token(parser); // consume '}'

    return create_for_node(init, condition, increment, body);
}

// Parse a function definition
static ASTNode *parse_function_definition(Parser *parser)
{
    // printf("[DEBUG] Entering parse_function_definition\n");
    // printf("[DEBUG] Current token type: %d, value: %s\n", 
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");
    
    // Check for #define
    if (parser->current_token->type != TOKEN_DEFINE) {
        printf("[DEBUG] Error: Expected '#define', got token type %d\n", 
               parser->current_token->type);
        return NULL;
    }
    get_next_token(parser); // consume #define
    // printf("[DEBUG] After #define, token type: %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    // Get return type
    char *return_type = NULL;
    if (parser->current_token->type == TOKEN_NOTHING_TYPE) {
        // printf("[DEBUG] Found 'nothing' return type\n");
        return_type = strdup("nothing");
        get_next_token(parser);
    } else if (parser->current_token->type == TOKEN_INT_TYPE ||
               parser->current_token->type == TOKEN_STRING_TYPE ||
               parser->current_token->type == TOKEN_FLOAT_TYPE ||
               parser->current_token->type == TOKEN_BOOL_TYPE ||
               parser->current_token->type == TOKEN_CHAR_TYPE) {
        // printf("[DEBUG] Found standard return type: %s\n", parser->current_token->value);
        return_type = strdup(parser->current_token->value);
        get_next_token(parser);
    } else {
        // printf("[DEBUG] Error: Expected return type, got token type %d\n", 
        //        parser->current_token->type);
        return NULL;
    }

    // printf("[DEBUG] After return type, current token type: %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    // Get function name
    if (parser->current_token->type != TOKEN_IDENTIFIER) {
        // printf("[DEBUG] Error: Expected function name identifier, got token type %d\n",
        //        parser->current_token->type);
        free(return_type);
        return NULL;
    }

    char *function_name = strdup(parser->current_token->value);
    // printf("[DEBUG] Found function name: %s\n", function_name);
    get_next_token(parser);

    // Parse parameters
    // printf("[DEBUG] Parsing parameters, current token: %d\n", parser->current_token->type);
    if (parser->current_token->type != TOKEN_LPAREN) {
        // printf("[DEBUG] Error: Expected '(' after function name\n");
        free(return_type);
        free(function_name);
        return NULL;
    }
    get_next_token(parser); // consume (

    ASTNode *parameters = parse_function_parameters(parser);
    // printf("[DEBUG] Finished parsing parameters\n");

    // // Parse function body
    // printf("[DEBUG] Starting to parse function body\n");
    // printf("[DEBUG] Current token type: %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    if (parser->current_token->type != TOKEN_LBRACE) {
        // printf("[DEBUG] Error: Expected '{' to start function body, got token type %d\n",
        //        parser->current_token->type);
        free(return_type);
        free(function_name);
        return NULL;
    }
    get_next_token(parser); // consume {

    ASTNode *body = NULL;
    ASTNode *current = NULL;

    // printf("[DEBUG] Parsing function body statements\n");
    while (parser->current_token->type != TOKEN_RBRACE && 
           parser->current_token->type != TOKEN_EOF) {
        // printf("[DEBUG] Parsing statement in function body, token type: %d\n",
        //        parser->current_token->type);
        
        ASTNode *statement = parse_statement(parser);
        if (!statement) {
            // printf("[DEBUG] Failed to parse statement in function body\n");
            free(return_type);
            free(function_name);
            // Free any previously parsed body statements
            free_ast(body);
            return NULL;
        }

        if (!body) {
            body = statement;
            current = body;
        } else {
            current->next = statement;
            current = statement;
        }
    }

    if (parser->current_token->type != TOKEN_RBRACE) {
        // printf("[DEBUG] Error: Expected '}' to end function body\n");
        free(return_type);
        free(function_name);
        free_ast(body);
        return NULL;
    }
    get_next_token(parser); // consume }

    // printf("[DEBUG] Successfully parsed function definition for %s\n", function_name);
    return create_function_definition_node(return_type, function_name, parameters, body);
}

// Parse function parameters
static ASTNode *parse_function_parameters(Parser *parser)
{
    // printf("[DEBUG] Entering parse_function_parameters\n");
    ASTNode *first_param = NULL;
    ASTNode *current_param = NULL;

    // Handle empty parameter list
    if (parser->current_token->type == TOKEN_RPAREN) {
        // printf("[DEBUG] Empty parameter list found\n");
        get_next_token(parser); // consume )
        return NULL;
    }

    while (1) // Continue until return or break
    {
        // printf("[DEBUG] Starting parameter parse, current token type: %d, value: %s\n", 
        //        parser->current_token->type, 
        //        parser->current_token->value ? parser->current_token->value : "NULL");

        // Parse parameter type
        char *param_type = NULL;
        if (parser->current_token->type == TOKEN_INT_TYPE ||
            parser->current_token->type == TOKEN_FLOAT_TYPE ||
            parser->current_token->type == TOKEN_STRING_TYPE ||
            parser->current_token->type == TOKEN_CHAR_TYPE ||
            parser->current_token->type == TOKEN_BOOL_TYPE)
        {
            param_type = strdup(parser->current_token->value);
            // printf("[DEBUG] Found parameter type: %s\n", param_type);
            get_next_token(parser);
        }
        else
        {
            // printf("[DEBUG] Error: Expected parameter type, got token type %d with value '%s'\n", 
            //        parser->current_token->type,
            //        parser->current_token->value ? parser->current_token->value : "NULL");
            if (first_param) free_ast(first_param);
            return NULL;
        }

        // printf("[DEBUG] After type parse, current token type: %d, value: %s\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");

        // Parse parameter name
        if (parser->current_token->type != TOKEN_IDENTIFIER)
        {
            // printf("[DEBUG] Error: Expected parameter name after type, got token type %d with value '%s'\n",
            //        parser->current_token->type,
            //        parser->current_token->value ? parser->current_token->value : "NULL");
            free(param_type);
            if (first_param) free_ast(first_param);
            return NULL;
        }

        char *param_name = strdup(parser->current_token->value);
        // printf("[DEBUG] Found parameter name: %s\n", param_name);
        get_next_token(parser);

        // printf("[DEBUG] After name parse, current token type: %d, value: %s\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");

        // Create parameter node
        ASTNode *param_node = create_function_parameter_node(param_type, param_name);
        free(param_type);
        free(param_name);

        if (!param_node)
        {
            // printf("[DEBUG] Error: Failed to create parameter node\n");
            if (first_param) free_ast(first_param);
            return NULL;
        }

        // Add to parameter list
        if (!first_param)
        {
            first_param = param_node;
            current_param = param_node;
        }
        else
        {
            current_param->next = param_node;
            current_param = param_node;
        }

        // printf("[DEBUG] Parameter parsed successfully: %s %s\n", param_node->var_type, param_node->var_name);

        // Check for comma or end of parameters

        // printf("[DEBUG] Checking for comma or end of parameters, current token type: %d, value: %s\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");

        if (parser->current_token->type == TOKEN_RPAREN)
        {
            // printf("[DEBUG] Found closing parenthesis, ending parameter list\n");
            get_next_token(parser); // consume )
            break; // End of parameter list
        }
        else if (parser->current_token->type == TOKEN_COMMA)
        {
            // printf("[DEBUG] Found comma, expecting another parameter\n");
            get_next_token(parser); // Skip comma
            // printf("[DEBUG] After comma, current token type: %d, value: %s\n",
            //        parser->current_token->type,
            //        parser->current_token->value ? parser->current_token->value : "NULL");
            
            // After comma, must have another parameter
            if (parser->current_token->type == TOKEN_RPAREN)
            {
                // printf("[DEBUG] Error: Found closing parenthesis after comma, expected parameter\n");
                if (first_param) free_ast(first_param);
                return NULL;
            }
        }
        else
        {
            // printf("[DEBUG] Error: Expected ',' or ')' after parameter, got token type %d with value '%s'\n",
            //        parser->current_token->type,
            //        parser->current_token->value ? parser->current_token->value : "NULL");
            if (first_param) free_ast(first_param);
            return NULL;
        }
    }

    // printf("[DEBUG] Successfully parsed all parameters\n");
    return first_param;
}

// Parse function body
static ASTNode *parse_function_body(Parser *parser) __attribute__((unused));

// Parse yield statement
static ASTNode *parse_yield_statement(Parser *parser)
{
    get_next_token(parser); // consume 'yield'

    // Parse the expression to yield
    ASTNode *expr = parse_expression(parser);
    if (!expr)
    {
        return NULL;
    }

    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        free_ast(expr);
        return NULL;
    }
    get_next_token(parser); // consume ;

    // Create yield node
    ASTNode *yield_node = create_node(NODE_YIELD_STATEMENT, expr, NULL, NULL);
    if (!yield_node)
    {
        free_ast(expr);
        return NULL;
    }

    return yield_node;
}

// Parse function call
static ASTNode *parse_function_call(Parser *parser)
{
    // printf("[DEBUG] Parsing function call\n");
    
    char *function_name = strdup(parser->current_token->value);
    // printf("[DEBUG] Function name: %s\n", function_name);
    get_next_token(parser); // Consume function name
    
    if (parser->current_token->type != TOKEN_LPAREN) {
        // printf("[DEBUG] Error: Expected '(' after function name %s, got %s\n", 
        //        function_name, parser->current_token->value);
        free(function_name);
        return NULL;
    }
    get_next_token(parser); // Consume (

    ASTNode *first_arg = NULL;
    ASTNode *current_arg = NULL;

    // Handle empty argument list
    if (parser->current_token->type == TOKEN_RPAREN) {
        // printf("[DEBUG] Empty argument list\n");
        get_next_token(parser); // Consume )
        
        // Check for semicolon
        if (parser->current_token->type != TOKEN_SEMICOLON) {
            // printf("[DEBUG] Error: Expected ';' after function call, got token type %d\n",
            //        parser->current_token->type);
            free(function_name);
            return NULL;
        }
        get_next_token(parser); // Consume ;
        
        ASTNode *node = create_function_call_node(function_name, NULL);
        free(function_name);
        // printf("[DEBUG] Successfully parsed function call with no arguments\n");
        return node;
    }

    while (1) {
        // printf("[DEBUG] Parsing argument. Current token: %s\n", parser->current_token->value);
        
        ASTNode *arg = parse_expression(parser);
        if (!arg) {
            // printf("[DEBUG] Error: Invalid function argument\n");
            free(function_name);
            if (first_arg) free_ast(first_arg);
            return NULL;
        }

        if (!first_arg) {
            first_arg = arg;
            current_arg = arg;
        } else {
            current_arg->next = arg;
            current_arg = arg;
        }

        // printf("[DEBUG] Successfully parsed argument\n");

        if (parser->current_token->type == TOKEN_RPAREN) {
            // printf("[DEBUG] End of argument list\n");
            get_next_token(parser); // Consume )
            break;
        }

        if (parser->current_token->type != TOKEN_COMMA) {
            // printf("[DEBUG] Error: Expected ',' or ')' after argument, got %s\n", 
            //        parser->current_token->value);
            free(function_name);
            free_ast(first_arg);
            return NULL;
        }
        get_next_token(parser); // Consume ,
    }

    // Check for semicolon after function call
    if (parser->current_token->type != TOKEN_SEMICOLON) {
        // printf("[DEBUG] Error: Expected ';' after function call, got token type %d\n",
        //        parser->current_token->type);
        free(function_name);
        if (first_arg) free_ast(first_arg);
        return NULL;
    }
    get_next_token(parser); // Consume ;

    // printf("[DEBUG] Successfully parsed function call to %s\n", function_name);
    return create_function_call_node(function_name, first_arg);
}

static ASTNode *parse_print(Parser *parser)
{
    //  printf("[DEBUG] Parsing print statement\n");
    get_next_token(parser); // Consume 'print' token

    if (parser->current_token->type != TOKEN_LPAREN)
    {
        // printf("[DEBUG] Error: Expected '(' after print, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        return NULL;
    }
    get_next_token(parser);

    ASTNode *expression = parse_expression(parser);
    if (!expression)
    {
        // printf("[DEBUG] Error: Failed to parse print expression\n");
        return NULL;
    }

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        // printf("[DEBUG] Error: Expected ')' after print argument, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free_ast(expression);
        return NULL;
    }
    get_next_token(parser); // consume ')'

    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        // printf("[DEBUG] Error: Expected ';' after print statement, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free_ast(expression);
        return NULL;
    }
    get_next_token(parser); // consume ';'

    // printf("[DEBUG] Successfully parsed print statement\n");
    return create_node(NODE_PRINT, expression, NULL, NULL);
}

static ASTNode *parse_var_declaration(Parser *parser)
{
    // printf("[DEBUG] Parsing variable declaration\n");
    char *type = strdup(parser->current_token->value);
    // printf("[DEBUG] Variable type: %s\n", type);
    
    get_next_token(parser);
    
    if (parser->current_token->type != TOKEN_IDENTIFIER)
    {
        // printf("[DEBUG] Error: Expected identifier after type, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free(type);
        return NULL;
    }

    char *var_name = strdup(parser->current_token->value);
    // printf("[DEBUG] Variable name: %s\n", var_name);
    
    get_next_token(parser);

    if (parser->current_token->type != TOKEN_ASSIGN)
    {
        // printf("[DEBUG] Error: Expected '=' after variable name, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free(type);
        free(var_name);
        return NULL;
    }

    get_next_token(parser);  // Move past '='

    // Parse the full expression, which can include binary operations
    ASTNode *value = parse_expression(parser);
    if (!value)
    {
        // printf("[DEBUG] Error: Failed to parse value in variable declaration\n");
        free(type);
        free(var_name);
        return NULL;
    }

    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        // printf("[DEBUG] Error: Expected semicolon after variable declaration, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free_ast(value);
        free(type);
        free(var_name);
        return NULL;
    }

    get_next_token(parser); // consume semicolon
    // printf("[DEBUG] Successfully parsed variable declaration: %s %s\n", type, var_name);
    return create_var_declaration_node(type, var_name, value);
}

static ASTNode *parse_array_declaration(Parser *parser)
{
    // printf("DEBUG: Starting array declaration parse\n");
    
    // Store the variable name
    char *var_name = strdup(parser->current_token->value);
    // printf("DEBUG: Array name: %s\n", var_name);
    
    get_next_token(parser); // Consume identifier
    // printf("DEBUG: After identifier, token type: %d, value: %s\n", 
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    // Check for array type annotation
    if (parser->current_token->type != TOKEN_ARRAY_TYPE && 
        parser->current_token->type != TOKEN_LESS_THAN)
    {
        // printf("DEBUG: Expected array type annotation, got token type: %d\n", 
        //        parser->current_token->type);
        free(var_name);
        return NULL;
    }

    get_next_token(parser); // Consume <
    // printf("DEBUG: After <, token type: %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    // Get the array type
    if (!parser->current_token->value)
    {
        // printf("DEBUG: No type specified after <\n");
        free(var_name);
        return NULL;
    }

    char *array_type = strdup(parser->current_token->value);
    // printf("DEBUG: Array type: %s\n", array_type);
    
    get_next_token(parser); // Consume type

    // Check for closing >
    if (parser->current_token->type != TOKEN_GREATER_THAN)
    {
        // printf("DEBUG: Expected >, got token type: %d\n", parser->current_token->type);
        free(var_name);
        free(array_type);
        return NULL;
    }

    get_next_token(parser); // Consume >

    // Check for assignment
    if (parser->current_token->type != TOKEN_ASSIGN)
    {
        // printf("DEBUG: Expected =, got token type: %d\n", parser->current_token->type);
        free(var_name);
        free(array_type);
        return NULL;
    }

    get_next_token(parser); // Consume =

    // printf("DEBUG: About to parse array literal\n");
    ASTNode *elements = parse_array_literal(parser);
    if (!elements)
    {
        // printf("DEBUG: Failed to parse array literal\n");
        free(var_name);
        free(array_type);
        return NULL;
    }

    // Check for semicolon
    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        // printf("DEBUG: Expected semicolon, got token type: %d\n", 
        //        parser->current_token->type);
        free(var_name);
        free(array_type);
        free_ast(elements);
        return NULL;
    }

    get_next_token(parser); // Consume ;

    // printf("DEBUG: Successfully parsed array declaration\n");
    return create_array_declaration_node(array_type, var_name, elements);
}

static ASTNode *parse_array_literal(Parser *parser)
{
    // printf("DEBUG: Entering parse_array_literal\n");
    
    if (parser->current_token->type != TOKEN_LBRACKET)
    {
        // printf("Error: Expected '[' at start of array literal, got token type %d\n",
        //        parser->current_token->type);
        return NULL;
    }
    get_next_token(parser); // Consume [

    ASTNode *elements = NULL;
    ASTNode *current = NULL;

    // Handle empty array
    if (parser->current_token->type == TOKEN_RBRACKET)
    {
        get_next_token(parser); // Consume ]
        return create_array_literal_node(NULL);
    }

    while (1)
    {
        // printf("DEBUG: Parsing array element\n");
        ASTNode *element = parse_expression(parser);
        if (!element)
        {
            // printf("DEBUG: Failed to parse array element\n");
            if (elements) free_ast(elements);
            return NULL;
        }

        if (!elements)
        {
            elements = element;
            current = elements;
        }
        else
        {
            current->next = element;
            current = element;
        }

        if (parser->current_token->type == TOKEN_RBRACKET)
        {
            get_next_token(parser); // Consume ]
            break;
        }

        if (parser->current_token->type != TOKEN_COMMA)
        {
            printf("Error: Expected ',' or ']' in array literal\n");
            free_ast(elements);
            return NULL;
        }
        get_next_token(parser); // Consume ,
    }

    return create_array_literal_node(elements);
}

static ASTNode *parse_array_access(Parser *parser, char *array_name)
{
    get_next_token(parser); // Consume [
    
    ASTNode *index = parse_expression(parser);
    if (!index)
    {
        return NULL;
    }

    if (parser->current_token->type != TOKEN_RBRACKET)
    {
        printf("Error: Expected ']' after array index\n");
        return NULL;
    }
    get_next_token(parser); // Consume ]

    return create_array_access_node(array_name, index);
}

static ASTNode *parse_array_method_call(Parser *parser, char *array_name)
{
    get_next_token(parser); // Consume .

    char *method_name = strdup(parser->current_token->value);
    TokenType method_type = parser->current_token->type;
    get_next_token(parser); // Consume method name

    ASTNode *argument = NULL;
    if (parser->current_token->type == TOKEN_LPAREN)
    {
        get_next_token(parser); // Consume (
        
        if (parser->current_token->type != TOKEN_RPAREN)
        {
            argument = parse_expression(parser);
            if (!argument)
            {
                return NULL;
            }
        }

        if (parser->current_token->type != TOKEN_RPAREN)
        {
            printf("Error: Expected ')' after method argument\n");
            return NULL;
        }
        get_next_token(parser); // Consume )
    }

    return create_array_method_call_node(array_name, method_name, argument);
}

static ASTNode *parse_input(Parser *parser)
{
    get_next_token(parser); // consume 'input'

    // Check for opening parenthesis
    if (parser->current_token->type != TOKEN_LPAREN)
    {
        printf("Error: Expected '(' after input\n");
        return NULL;
    }
    get_next_token(parser); // consume '('

    // Parse the prompt string
    if (parser->current_token->type != TOKEN_STRING)
    {
        printf("Error: Expected string prompt in input()\n");
        return NULL;
    }
    char *prompt = strdup(parser->current_token->value);
    get_next_token(parser); // consume string

    // Check for closing parenthesis
    if (parser->current_token->type != TOKEN_RPAREN)
    {
        printf("Error: Expected ')' after input prompt\n");
        free(prompt);
        return NULL;
    }
    get_next_token(parser); // consume ')'

    return create_node(NODE_INPUT, NULL, NULL, prompt);
}
