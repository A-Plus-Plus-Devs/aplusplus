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
static ASTNode *parse_function_body(Parser *parser);
static ASTNode *parse_yield_statement(Parser *parser);
static ASTNode *parse_function_call(Parser *parser);

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
        return NULL;
    }
    
    ASTNode *root = NULL;
    ASTNode *current = NULL;
    
    while (parser->current_token->type != TOKEN_EOF)
    {
        ASTNode *statement = parse_statement(parser);
        if (!statement)
        {
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
    // printf("[DEBUG] Parsing statement, current token type: %d, value: %s\n",
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
            // printf("[DEBUG] Found identifier\n");
            if (peek_char(parser->lexer) == '(')
            {
                node = parse_function_call(parser);
            }
            else
            {
                node = parse_assignment(parser);
            }
            break;

        case TOKEN_YIELD:
            // printf("[DEBUG] Found yield statement\n");
            node = parse_yield_statement(parser);
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

    if (!node)
    {
        // printf("[DEBUG] Failed to parse statement\n");
        return NULL;
    }

    // printf("[DEBUG] Successfully parsed statement\n");
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
    // printf("[DEBUG] Parsing factor, token type: %d, value: '%s'\n", 
    //        token->type, token->value ? token->value : "NULL");

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
        get_next_token(parser);  // consume '('
        ASTNode *expr = parse_expression(parser);
        
        if (!expr)
        {
            printf("Error: Invalid expression inside parentheses\n");
            return NULL;
        }
        
        if (parser->current_token->type != TOKEN_RPAREN)
        {
            printf("Error: Expected closing parenthesis\n");
            return NULL;
        }
        
        get_next_token(parser);  // consume ')'
        return expr;
    }
    else if (token->type == TOKEN_BOOL)
    {
        ASTNode *node = create_node(NODE_BOOL_LITERAL, NULL, NULL, token->value);
        get_next_token(parser);
        return node;
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
    ASTNode *else_body = NULL;
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
    
    // Skip #define token
    if (parser->current_token->type != TOKEN_DEFINE)
    {
        // printf("[DEBUG] Error: Expected '#define' for function definition, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        return NULL;
    }
    get_next_token(parser);

    // Parse return type - handle both direct types and TOKEN_TYPE
    char *return_type = NULL;
    // printf("[DEBUG] Current token for return type: type %d, value: '%s'\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    if (parser->current_token->type == TOKEN_INT_TYPE ||
        parser->current_token->type == TOKEN_FLOAT_TYPE ||
        parser->current_token->type == TOKEN_STRING_TYPE ||
        parser->current_token->type == TOKEN_CHAR_TYPE ||
        parser->current_token->type == TOKEN_BOOL_TYPE)
    {
        return_type = strdup(parser->current_token->value);
        // printf("[DEBUG] Found return type: %s\n", return_type);
        get_next_token(parser);
    }
    else
    {
        // printf("[DEBUG] Error: Expected return type, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        return NULL;
    }

    // Parse function name
    // printf("[DEBUG] Current token for function name: type %d, value: '%s'\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    if (parser->current_token->type != TOKEN_IDENTIFIER)
    {
        // printf("[DEBUG] Error: Expected function name, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        // free(return_type);
        return NULL;
    }

    char *function_name = strdup(parser->current_token->value);
    // printf("[DEBUG] Found function name: %s\n", function_name);
    get_next_token(parser);

    // Parse parameters
    // printf("[DEBUG] Current token before parameters: type %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    if (parser->current_token->type != TOKEN_LPAREN)
    {
        // printf("[DEBUG] Error: Expected '(' after function name, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free(return_type);
        free(function_name);
        return NULL;
    }
    get_next_token(parser); // consume (

    ASTNode *parameters = parse_function_parameters(parser);
    // Note: parse_function_parameters will handle its own error messages

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        // printf("[DEBUG] Error: Expected ')' after parameters, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free(return_type);
        free(function_name);
        if (parameters) free_ast(parameters);
        return NULL;
    }
    get_next_token(parser); // consume )

    // Parse function body
    // printf("[DEBUG] Current token before function body: type %d, value: %s\n",
    //        parser->current_token->type,
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    if (parser->current_token->type != TOKEN_LBRACE)
    {
        // printf("[DEBUG] Error: Expected '{' after function parameters, got token type %d with value '%s'\n",
        //        parser->current_token->type,
        //        parser->current_token->value ? parser->current_token->value : "NULL");
        free(return_type);
        free(function_name);
        if (parameters) free_ast(parameters);
        return NULL;
    }
    get_next_token(parser); // consume {

    // printf("[DEBUG] Parsing function body\n");
    ASTNode *body = parse_function_body(parser);
    if (!body)
    {
        // printf("[DEBUG] Error: Failed to parse function body\n");
        free(return_type);
        free(function_name);
        if (parameters) free_ast(parameters);
        return NULL;
    }

    // printf("[DEBUG] Successfully parsed function definition: %s %s\n", return_type, function_name);
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
static ASTNode *parse_function_body(Parser *parser)
{
    ASTNode *first_statement = NULL;
    ASTNode *current_statement = NULL;

    while (parser->current_token->type != TOKEN_RBRACE)
    {
        ASTNode *statement = NULL;
        
        // Check for yield statement
        if (parser->current_token->type == TOKEN_YIELD)
        {
            statement = parse_yield_statement(parser);
        }
        else
        {
            statement = parse_statement(parser);
        }

        if (!statement)
        {
            if (first_statement) free_ast(first_statement);
            return NULL;
        }

        // Add statement to body
        if (!first_statement)
        {
            first_statement = statement;
            current_statement = first_statement;
        }
        else
        {
            current_statement->next = statement;
            current_statement = statement;
        }
    }

    // Consume closing brace
    get_next_token(parser); // consume }
    return first_statement;
}

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
        return create_function_call_node(function_name, NULL);
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

    return create_function_call_node(function_name, first_arg);
}

static ASTNode *parse_print(Parser *parser)
{
    // printf("[DEBUG] Parsing print statement\n");
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
            //    parser->current_token->value ? parser->current_token->value : "NULL");
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
