// parser.c
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
ASTNode *parse_print(Parser *parser); // Note: This is not static
// static ASTNode *parse_echo(Parser *parser);
static ASTNode *parse_var_declaration(Parser *parser);

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
    ASTNode *statement = NULL;

    // Check the type of the current token and parse accordingly
    switch (parser->current_token->type)
    {
    case TOKEN_INT_TYPE:
    case TOKEN_FLOAT_TYPE:
    case TOKEN_STRING_TYPE:
        statement = parse_var_declaration(parser); // Parse a variable declaration
        break;
    case TOKEN_PRINT:
        statement = parse_print(parser); // Parse a print statement
        break;
    case TOKEN_IDENTIFIER:
        statement = parse_assignment(parser); // Parse an assignment statement
        break;
    case TOKEN_EOF:
        return NULL; // End of file reached
    default:
        printf("Unexpected token in statement: Type %d, Value %s\n", parser->current_token->type, parser->current_token->value);
        get_next_token(parser); // Skip the unexpected token
        return NULL;
    }

    // Check if the statement ends with a semicolon
    if (statement && parser->current_token->type != TOKEN_SEMICOLON)
    {
        printf("Error: Expected semicolon at the end of the statement.\n");
        free_ast(statement); // Free the statement if it exists
        return NULL;
    }

    // If we successfully parsed a statement, move past the semicolon
    if (statement)
    {
        get_next_token(parser); // Consume the semicolon
    }

    return statement; // Return the parsed statement
}

// This function parses a variable declaration statement
static ASTNode *parse_var_declaration(Parser *parser)
{
    char *type = strdup(parser->current_token->value); // Duplicate the type
    get_next_token(parser);

    // Check if the next token is an identifier
    if (parser->current_token->type != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected identifier after type in variable declaration.\n");
        free(type);
        return NULL;
    }

    char *var_name = strdup(parser->current_token->value); // Duplicate the variable name
    get_next_token(parser);

    ASTNode *value = NULL; // Initialize the value to NULL
    if (parser->current_token->type == TOKEN_ASSIGN)
    { 
        get_next_token(parser); // Consume the '=' token

        // Check if the type is string and the value is a string
        if (strcmp(type, "string") == 0 && parser->current_token->type == TOKEN_STRING)
        {
            // Create a new AST node for the string literal
            value = create_node(NODE_STRING_LITERAL, NULL, NULL, parser->current_token->value);
            get_next_token(parser); // Move to the next token
        } else if (strcmp(type, "bool") == 0 && parser->current_token->type == TOKEN_BOOL)
        {
            value = create_node(NODE_BOOL_LITERAL, NULL, NULL, parser->current_token->value);
            get_next_token(parser);
        }
        else
        {
            value = parse_expression(parser); // Parse the expression for non-string types
        }

        // If parsing the value fails, free the allocated memory and return NULL
        if (!value)
        {
            free(type);     // Free the type
            free(var_name); // Free the variable name
            return NULL;    // Return NULL to indicate failure
        }
    }

    // Create and return a new AST node for the variable declaration
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
    char *var_name = strdup(parser->current_token->value);
    get_next_token(parser);

    if (parser->current_token->type != TOKEN_ASSIGN)
    {
        printf("Error: Expected '=' in assignment.\n");
        free(var_name);
        return NULL;
    }
    get_next_token(parser);

    ASTNode *value = parse_expression(parser);

    if (parser->current_token->type == TOKEN_BOOL)
    {
        value = create_node(NODE_BOOL_LITERAL, NULL, NULL, parser->current_token->value);
        get_next_token(parser);
    } else {
        value = parse_expression(parser);
    }

    if (!value)
    {
        free(var_name);
        return NULL;
    }

    return create_assignment_node(var_name, value);
}

static ASTNode *parse_expression(Parser *parser)
{
    ASTNode *left = parse_term(parser);

    while (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS)
    {
        Token *op_token = parser->current_token;
        char *op = op_token->type == TOKEN_PLUS ? "+" : "-";
        get_next_token(parser);
        ASTNode *right = parse_term(parser);
        left = create_node(NODE_BINARY_OP, left, right, op);
        printf("Debug: Created binary op node: %s\n", op);
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
        printf("Debug: Created binary op node: **\n");
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
        char *op;
        if (op_token->type == TOKEN_MULTIPLY) op = "*";
        else if (op_token->type == TOKEN_DIVIDE) op = "/";
        else op = "%";
        
        get_next_token(parser);
        ASTNode *right = parse_power(parser);
        left = create_node(NODE_BINARY_OP, left, right, op);
        printf("Debug: Created binary op node: %s\n", op);
    }

    return left;
}

static ASTNode *parse_factor(Parser *parser)
{
    Token *token = parser->current_token;

    if (token->type == TOKEN_NUMBER)
    {
        ASTNode *node = create_node(NODE_INT_LITERAL, NULL, NULL, token->value);
        printf("Debug: Created int literal node: %s\n", token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_IDENTIFIER)
    {
        ASTNode *node = create_node(NODE_LITERAL, NULL, NULL, token->value);
        printf("Debug: Created identifier node: %s\n", token->value);
        get_next_token(parser);
        return node;
    } else if (token->type == TOKEN_BOOL)
    {
        ASTNode *node = create_node(NODE_BOOL_LITERAL, NULL, NULL, token->value);
        printf("Debug: Created bool literal node: %s\n", token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_STRING)
    {
        ASTNode *node = create_node(NODE_STRING_LITERAL, NULL, NULL, token->value);
        printf("Debug: Created string literal node: %s\n", token->value);
        get_next_token(parser);
        return node;
    }
    else if (token->type == TOKEN_LPAREN)
    {
        get_next_token(parser);
        ASTNode *expr = parse_expression(parser);
        if (parser->current_token->type != TOKEN_RPAREN)
        {
            printf("Error: Expected closing parenthesis\n");
            return NULL;
        }
        get_next_token(parser);
        return expr;
    }

    printf("Error: Unexpected token in factor\n");
    return NULL;
}
