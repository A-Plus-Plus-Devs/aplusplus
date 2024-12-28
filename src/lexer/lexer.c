#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Initialise the lexer
Lexer *init_lexer(const char *input)
{
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer)); // Allocate memory for a new Lexer structure
    lexer->input = input;                          // Set the input string for the lexer
    lexer->position = 0;                           // Initialize the current position to the start of the input
    lexer->read_position = 1;                      // Set the read position to the next character
    lexer->current_char = input[0];                // Set the current character to the first character of the input
    return lexer;                                  // Return the newly created lexer
}

// This function moves the lexer to the next character
void advance(Lexer *lexer)
{
    // If we haven't reached the end of the input
    if (lexer->read_position < strlen(lexer->input))
    {
        lexer->position = lexer->read_position;              // Move the current position forward
        lexer->current_char = lexer->input[lexer->position]; // Set the current character to the next character
        lexer->read_position++;                              // Move the read position forward
    }
    else
    {
        // If we've reached the end, set the current character to null
        lexer->current_char = '\0';
    }
}

// This function identifies keywords or identifiers
static Token *identifier_or_keyword(Lexer *lexer)
{
    char buffer[256] = {0};
    int i = 0;
    while (isalnum(lexer->current_char) || lexer->current_char == '_')
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }
    buffer[i] = '\0';

    Token *token = malloc(sizeof(Token));
    token->value = strdup(buffer);

    // printf("DEBUG: Identifying token: '%s'\n", buffer);

    // Add new keywords
    if (strcmp(buffer, "if") == 0)
        token->type = TOKEN_IF;
    else if (strcmp(buffer, "else") == 0)
        token->type = TOKEN_ELSE;
    else if (strcmp(buffer, "int") == 0)
        token->type = TOKEN_INT_TYPE;
    else if (strcmp(buffer, "string") == 0)
        token->type = TOKEN_STRING_TYPE;
    else if (strcmp(buffer, "float") == 0)
        token->type = TOKEN_FLOAT_TYPE;
    else if (strcmp(buffer, "boolean") == 0 || strcmp(buffer, "bool") == 0)
        token->type = TOKEN_BOOL_TYPE;
    else if (strcmp(buffer, "print") == 0 || strcmp(buffer, "echo") == 0 || strcmp(buffer, "yap") == 0)
        token->type = TOKEN_PRINT;
    else if (strcmp(buffer, "yup") == 0 || strcmp(buffer, "nope") == 0)
        token->type = TOKEN_BOOL;
    else if (strcmp(buffer, "for") == 0)
        token->type = TOKEN_FOR;
    else
        token->type = TOKEN_IDENTIFIER;

    // printf("DEBUG: Token type assigned: %d\n", token->type);
    return token;
}

static Token *boolean(Lexer *lexer)
{
    char buffer[6] = {0};
    int i = 0;
    while (isalpha(lexer->current_char))
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }
    buffer[i] = '\0';
    Token *token = malloc(sizeof(Token));

    if (strcmp(buffer, "yup") == 0 || strcmp(buffer, "nope") == 0) {
        token->type = TOKEN_BOOL;
        token->value = strdup(buffer);
    } else {
        token->type = TOKEN_IDENTIFIER;
        token->value = strdup(buffer);
    }
    return token;
}


// This function identifies numbers
static Token *number(Lexer *lexer)
{
    char buffer[256] = {0};
    int i = 0;
    bool is_float = false;

    // printf("\nDEBUG: Starting number parse\n");
    // printf("DEBUG: Current char: '%c'\n", lexer->current_char);

    // Get digits before decimal point
    while (isdigit(lexer->current_char))
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }

    // Check for decimal point
    if (lexer->current_char == '.')
    {
        // printf("DEBUG: Found decimal point\n");
        is_float = true;
        buffer[i++] = lexer->current_char;
        advance(lexer);
        
        // Get digits after decimal point
        while (isdigit(lexer->current_char))
        {
            buffer[i++] = lexer->current_char;
            advance(lexer);
        }
    }

    buffer[i] = '\0';

    Token *token = malloc(sizeof(Token));
    token->type = is_float ? TOKEN_FLOAT : TOKEN_NUMBER;
    token->value = strdup(buffer);

    // printf("DEBUG: Created number token - type: %d, value: '%s'\n", 
    //        token->type, token->value);

    return token;
}

// This function identifies strings
static Token *string(Lexer *lexer)
{
    char buffer[256] = {0};
    int i = 0;
    advance(lexer); // Skip the opening quote

    while (lexer->current_char != '"' && lexer->current_char != '\0')
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }

    if (lexer->current_char == '"')
    {
        advance(lexer); // Skip the closing quote
    }
    else
    {
        printf("Error: Unterminated string literal\n");
    }

    buffer[i] = '\0';

    Token *token = malloc(sizeof(Token));
    token->type = TOKEN_STRING;
    token->value = strdup(buffer);
    // printf("Debug: Created string token: %s\n", token->value);

    return token;
}

// This function looks at the next character without moving the lexer
char peek_char(Lexer *lexer)
{
    if (lexer->read_position >= strlen(lexer->input))
    {
        return '\0'; // Return null if we've reached the end of the input
    }
    return lexer->input[lexer->read_position]; // Return the next character
}

static TokenType check_multi_char_operator(Lexer *lexer) __attribute__((unused));
static TokenType check_multi_char_operator(Lexer *lexer)
{
    switch (lexer->current_char)
    {
    case '=':
        if (peek_char(lexer) == '=')
        {
            advance(lexer);
            return TOKEN_EQUAL;
        }
        return TOKEN_ASSIGN;
    case '!':
        if (peek_char(lexer) == '=')
        {
            advance(lexer);
            return TOKEN_NOT_EQUAL;
        }
        return TOKEN_NOT;
    case '>':
        if (peek_char(lexer) == '=')
        {
            advance(lexer);
            return TOKEN_GREATER_THAN_OR_EQUAL;
        }
        return TOKEN_GREATER_THAN;
    case '<':
        if (peek_char(lexer) == '=')
        {
            advance(lexer);
            return TOKEN_LESS_THAN_OR_EQUAL;
        }
        return TOKEN_LESS_THAN;
    } 
    return TOKEN_UNKNOWN;
}

// Skip comments (both single-line and multi-line)
static void skip_comments(Lexer *lexer)
{
    // If the current character is a '/' and the next character is a '/'
    if (lexer->current_char == '/' && peek_char(lexer) == '/')
    {
        // Skip the comment until the end of the line or the end of the input
        while (lexer->current_char != '\n' && lexer->current_char != '\0')
        {
            advance(lexer);
        }
    }
    // If the current character is a '/' and the next character is a '*'
    else if (lexer->current_char == '/' && peek_char(lexer) == '*')
    {
        advance(lexer); // Skip '/'
        advance(lexer); // Skip '*'
        // Skip the comment until the end of the block or the end of the input
        while (!(lexer->current_char == '*' && peek_char(lexer) == '/'))
        {
            advance(lexer);
        }
        advance(lexer); // Skip '*'
        advance(lexer); // Skip '/'
    }
}

// Skip whitespace characters
void skip_whitespace(Lexer *lexer)
{
    while (lexer->current_char == ' ' || 
           lexer->current_char == '\t' || 
           lexer->current_char == '\n' || 
           lexer->current_char == '\r')
    {
        advance(lexer);
    }
}

// Get the next token
Token *next_token(Lexer *lexer)
{
    skip_whitespace(lexer);
    skip_comments(lexer);

    Token *token = malloc(sizeof(Token));
    token->value = NULL;

    if (lexer->current_char == '\0')
    {
        token->type = TOKEN_EOF;
        return token;
    }

    // If the current character is alphanumeric or an underscore
    if (isalpha(lexer->current_char) || lexer->current_char == '_')
    {
        return identifier_or_keyword(lexer); // Return the token
    }

    // If the current character is a digit
    if (isdigit(lexer->current_char))
    {
        return number(lexer); // Return the token
    }

    // Check for boolean values
    if ((lexer->current_char == 'y' && strncmp(lexer->input + lexer->position, "yup", 3) == 0) ||
        (lexer->current_char == 'n' && strncmp(lexer->input + lexer->position, "nope", 4) == 0))
    {
        return boolean(lexer);
    }

    
    // Check for single-character operators and tokens
    switch (lexer->current_char)
    {
    case '=':
        if (peek_char(lexer) == '=')
        {
            token->type = TOKEN_EQUAL;
            token->value = strdup("==");
            advance(lexer); // consume first '='
            advance(lexer); // consume second '='
        }
        else
        {
            token->type = TOKEN_ASSIGN;
            token->value = strdup("=");
            advance(lexer);
        }
        return token;

    case '>':
        if (peek_char(lexer) == '=')
        {
            token->type = TOKEN_GREATER_THAN_OR_EQUAL;
            token->value = strdup(">=");
            advance(lexer);
            advance(lexer);
        }
        else
        {
            token->type = TOKEN_GREATER_THAN;
            token->value = strdup(">");
            advance(lexer);
        }
        return token;

    case '<':
        if (peek_char(lexer) == '=')
        {
            token->type = TOKEN_LESS_THAN_OR_EQUAL;
            token->value = strdup("<=");
            advance(lexer);
            advance(lexer);
        }
        else
        {
            token->type = TOKEN_LESS_THAN;
            token->value = strdup("<");
            advance(lexer);
        }
        return token;

    case '+':
        if (peek_char(lexer) == '+') {
            token->type = TOKEN_INCREMENT;
            token->value = strdup("++");
            advance(lexer); // consume first '+'
            advance(lexer); // consume second '+'
        } else {
            token->type = TOKEN_PLUS;
            token->value = strdup("+");
            advance(lexer);
        }
        return token;

    case '-':
        if (peek_char(lexer) == '-') {
            token->type = TOKEN_DECREMENT;
            token->value = strdup("--");
            advance(lexer); // consume first '-'
            advance(lexer); // consume second '-'
        } else {
            token->type = TOKEN_MINUS;
            token->value = strdup("-");
            advance(lexer);
        }
        return token;

    case '&':
        if (peek_char(lexer) == '&') {
            token->type = TOKEN_LOGICAL_AND;
            token->value = strdup("&&");
            advance(lexer); // consume first '&'
            advance(lexer); // consume second '&'
        } else {
            token->type = TOKEN_BITWISE_AND;
            token->value = strdup("&");
            advance(lexer);
        }
        return token;

    case '|':
        if (peek_char(lexer) == '|') {
            token->type = TOKEN_LOGICAL_OR;
            token->value = strdup("||");
            advance(lexer); // consume first '|'
            advance(lexer); // consume second '|'
        } else {
            token->type = TOKEN_BITWISE_OR;
            token->value = strdup("|");
            advance(lexer);
        }
        return token;

    case '*': 
        if (peek_char(lexer) == '*')
        {
            advance(lexer);
            token->type = TOKEN_POWER;
            token->value = strdup("**");
        }
        else
        {
            token->type = TOKEN_MULTIPLY;
            token->value = strdup("*");
        }
        break;
    case '/': token->type = TOKEN_DIVIDE; token->value = strdup("/"); break;
    case '(': token->type = TOKEN_LPAREN; token->value = strdup("("); break;
    case ')': token->type = TOKEN_RPAREN; token->value = strdup(")"); break;
    case ';': token->type = TOKEN_SEMICOLON; token->value = strdup(";"); break;
    case '"': 
        free(token); // Free the token we created since string() creates its own
        return string(lexer);
    case '\n':
    case '\r':
        advance(lexer);
        return next_token(lexer); // Skip newlines and get next token

    case '?':
        token->type = TOKEN_QUESTION;
        token->value = strdup("?");
        break;
            
    case ':':
        token->type = TOKEN_COLON;
        token->value = strdup(":");
        break;

    case '{':
        token->type = TOKEN_LBRACE;
        token->value = strdup("{");
        break;

    case '}':
        token->type = TOKEN_RBRACE;
        token->value = strdup("}");
        break;

    case '!':
        if (peek_char(lexer) == '=')
        {
            token->type = TOKEN_NOT_EQUAL;
            token->value = strdup("!=");
            advance(lexer); // consume '!'
            advance(lexer); // consume '='
        }
        else
        {
            token->type = TOKEN_NOT;
            token->value = strdup("!");
            advance(lexer);
        }
        return token;

    case '%': 
        token->type = TOKEN_MODULUS; 
        token->value = strdup("%"); 
        advance(lexer);
        return token;

    default:
        if (isdigit(lexer->current_char))
        {
            return number(lexer);
        }
        else if (isalpha(lexer->current_char) || lexer->current_char == '_')
        {
            return identifier_or_keyword(lexer);
        }
        else
        {
            // printf("Debug: Unknown character: '%c' (ASCII: %d)\n", lexer->current_char, lexer->current_char);
            token->type = TOKEN_EOF;
            return token;
        }
    }

    advance(lexer);
    return token;
}

