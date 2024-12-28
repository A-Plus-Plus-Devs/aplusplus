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
ASTNode *parse_print(Parser *parser);
// static ASTNode *parse_echo(Parser *parser);
static ASTNode *parse_var_declaration(Parser *parser);
static ASTNode *parse_for_statement(Parser *parser);
static ASTNode *parse_logical_or(Parser *parser);
static ASTNode *parse_logical_and(Parser *parser);
static ASTNode *parse_comparison(Parser *parser);

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

    ASTNode *root = NULL;
    ASTNode *current = NULL;

    // Keep parsing statements until we reach the end of the file
    while (parser->current_token->type != TOKEN_EOF)
    {
        // Parse a single statement
        ASTNode *node = parse_statement(parser);

        if (node)
        {
            // If this is the first node, make it the root
            if (root == NULL)
            {
                root = node;
            }
            else
            {
                // Otherwise, add it to the end of the current list
                current->next = node;
            }
            current = node;
        }
        else if (parser->current_token->type == TOKEN_EOF)
        {
            // If we reach the end of the file, break out of the loop
            break; // End of file reached
        }
        else
        {
            // If we couldn't parse the statement, print an error message
            printf("ERROR: Unexpected token in statement: Type %d, Value %s\n", parser->current_token->type, parser->current_token->value);
        }
    }
    // Return the root of the AST
    return root;
}

// This function creates a new parser with the given lexer
Parser *create_parser(Lexer *lexer)
{
    Parser *parser = malloc(sizeof(Parser));        // Allocate memory for the parser
    parser->lexer = lexer;                          // Set the lexer for the parser
    parser->current_token = get_next_token(parser); // Get the first token
    return parser;                                  // Return the parser
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
static ASTNode *parse_statement(Parser *parser)
{
    // printf("\nDEBUG: Starting statement parse\n");
    // printf("DEBUG: Current token type: %d, value: '%s'\n", 
    //        parser->current_token->type, 
    //        parser->current_token->value ? parser->current_token->value : "NULL");

    ASTNode *statement = NULL;

    switch (parser->current_token->type)
    {
        case TOKEN_INT_TYPE:
        case TOKEN_FLOAT_TYPE:
        case TOKEN_STRING_TYPE:
        case TOKEN_BOOL_TYPE:
            return parse_var_declaration(parser);
        case TOKEN_PRINT:
            statement = parse_print(parser);
            break;
        case TOKEN_IDENTIFIER:
            statement = parse_assignment(parser);
            if (statement && parser->current_token->type == TOKEN_SEMICOLON)
            {
                get_next_token(parser); // consume semicolon
            }
            return statement;
        case TOKEN_IF:
            return parse_if_statement(parser);
        case TOKEN_FOR:
            return parse_for_statement(parser);
        case TOKEN_EOF:
            return NULL;
        default:
            printf("Unexpected token in statement: Type %d, Value %s\n", 
                   parser->current_token->type, 
                   parser->current_token->value);
            get_next_token(parser);
            return NULL;
    }

    if (statement && parser->current_token->type == TOKEN_SEMICOLON)
    {
        get_next_token(parser); // consume semicolon
    }

    return statement;
}

// This function parses a variable declaration statement
static ASTNode *parse_var_declaration(Parser *parser)
{
    // printf("\nDEBUG: Starting var declaration parse\n");
    char *type = strdup(parser->current_token->value);
    
    get_next_token(parser);
    
    if (parser->current_token->type != TOKEN_IDENTIFIER)
    {
        printf("ERROR: Expected identifier after type\n");
        free(type);
        return NULL;
    }

    char *var_name = strdup(parser->current_token->value);
    
    get_next_token(parser);

    if (parser->current_token->type != TOKEN_ASSIGN)
    {
        printf("ERROR: Expected '=' after variable name\n");
        free(type);
        free(var_name);
        return NULL;
    }

    get_next_token(parser);  // Move past '='

    // Parse the full expression, which can include binary operations
    ASTNode *value = parse_expression(parser);
    if (!value)
    {
        printf("ERROR: Failed to parse value in variable declaration\n");
        free(type);
        free(var_name);
        return NULL;
    }

    if (parser->current_token->type != TOKEN_SEMICOLON)
    {
        printf("ERROR: Expected semicolon after variable declaration\n");
        free_ast(value);
        free(type);
        free(var_name);
        return NULL;
    }

    get_next_token(parser); // consume semicolon
    return create_var_declaration_node(type, var_name, value);
}

ASTNode *parse_print(Parser *parser)
{
    get_next_token(parser); // Consume 'print' token

    if (parser->current_token->type != TOKEN_LPAREN)
    {
        printf("Error: Expected '(' after print.\n");
        return NULL;
    }
    get_next_token(parser);

    ASTNode *expression = parse_expression(parser);
    if (!expression)
    {
        return NULL;
    }

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        printf("Error: Expected ')' after print argument.\n");
        free_ast(expression);
        return NULL;
    }
    get_next_token(parser);

    return create_node(NODE_PRINT, expression, NULL, NULL);
}

static ASTNode *parse_assignment(Parser *parser)
{
    if (parser->current_token->type != TOKEN_IDENTIFIER)
    {
        return NULL;
    }

    char *var_name = strdup(parser->current_token->value);
    get_next_token(parser);

    if (parser->current_token->type != TOKEN_ASSIGN)
    {
        free(var_name);
        return NULL;
    }

    get_next_token(parser); // consume '='
    
    // Parse the full expression
    ASTNode *value = parse_expression(parser);
    if (!value)
    {
        free(var_name);
        return NULL;
    }

    return create_assignment_node(var_name, value);
}

static ASTNode *parse_expression(Parser *parser)
{
    ASTNode *left = parse_comparison(parser);  // Start with comparison

    while (parser->current_token->type == TOKEN_PLUS || 
           parser->current_token->type == TOKEN_MINUS)
    {
        Token *op_token = parser->current_token;
        char *op = op_token->type == TOKEN_PLUS ? "+" : "-";
        
        get_next_token(parser);
        ASTNode *right = parse_comparison(parser); 
        left = create_node(NODE_BINARY_OP, left, right, op);
    }

    return left;
}

static ASTNode *parse_power(Parser *parser)
{
    ASTNode *left = parse_factor(parser);

    while (parser->current_token->type == TOKEN_POWER)
    {
        get_next_token(parser);
        ASTNode *right = parse_factor(parser);
        left = create_node(NODE_BINARY_OP, left, right, "**");
        // printf("Debug: Created binary op node: **\n");
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
        char *op = NULL;  // Initialise to NULL to fix warning
        
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
        // printf("Debug: Created binary op node: %s\n", op);
    }

    return left;
}

static ASTNode *parse_factor(Parser *parser)
{
    Token *token = parser->current_token;
    // printf("Debug: Parsing factor, token type: %d, value: %s\n", 
    //        token->type, token->value ? token->value : "NULL");

    if (token->type == TOKEN_NOT)
    {
        get_next_token(parser); // consume '!'
        ASTNode *operand = parse_factor(parser);
        if (!operand) {
            printf("Error: Expected expression after '!'\n");
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
        // printf("Debug: Created string literal node: %s\n", token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_IDENTIFIER)
    {
        ASTNode *node = create_node(NODE_LITERAL, NULL, NULL, token->value);
        // printf("Debug: Created identifier node: %s\n", token->value);
        get_next_token(parser);
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
        // printf("Debug: Created bool literal node: %s\n", token->value);
        get_next_token(parser);
        return node;
    }

    if (token->type == TOKEN_INCREMENT || token->type == TOKEN_DECREMENT)
    {
        char *op = token->type == TOKEN_INCREMENT ? "++" : "--";
        get_next_token(parser);
        ASTNode *operand = parse_factor(parser);
        if (!operand) {
            printf("Error: Expected expression after %s\n", op);
            return NULL;
        }
        return create_node(NODE_BINARY_OP, operand, NULL, op);
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

    // Check for else
    if (parser->current_token->type == TOKEN_ELSE)
    {
        get_next_token(parser); // consume 'else'
        
        if (parser->current_token->type != TOKEN_LBRACE)
        {
            printf("Error: Expected '{' after else\n");
            return NULL;
        }
        get_next_token(parser); // consume '{'

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
    }

    ASTNode *if_node = create_node(NODE_IF, condition, if_body, NULL);
    if_node->else_branch = else_body;
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


// Parse expressions with logical OR (||)
static ASTNode *parse_logical_or(Parser *parser)
{
    ASTNode *left = parse_logical_and(parser);

    while (parser->current_token->type == TOKEN_LOGICAL_OR)
    {
        // printf("DEBUG: Parsing OR operator\n");
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
        // printf("DEBUG: Parsing AND operator\n");
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
