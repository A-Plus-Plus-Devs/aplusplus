#include "lexer.h"
// #include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Initialize the lexer
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
    char buffer[256] = {0};                                            // Initialize a buffer to store the identifier
    int i = 0;                                                         // Initialize an index for the buffer
    while (isalnum(lexer->current_char) || lexer->current_char == '_') // While the current character is alphanumeric or an underscore
    {
        buffer[i++] = lexer->current_char; // Add the current character to the buffer
        advance(lexer);                    // Move to the next character
    }
    buffer[i] = '\0'; // Null-terminate the buffer

    Token *token = malloc(sizeof(Token)); // Allocate memory for a new token
    token->value = strdup(buffer);        // Duplicate the buffer to store the identifier

    // Check if the buffer matches any keywords
    if (strcmp(buffer, "int") == 0)
        token->type = TOKEN_INT_TYPE;
    else if (strcmp(buffer, "string") == 0)
        token->type = TOKEN_STRING_TYPE;
    else if (strcmp(buffer, "print") == 0 || strcmp(buffer, "echo") == 0)
        token->type = TOKEN_PRINT;
    else if (strcmp(buffer, "bool") == 0)
        token->type = TOKEN_BOOL_TYPE;
    else
        token->type = TOKEN_IDENTIFIER; // If it's not a keyword, it's an identifier

    return token; // Return the token
}

static Token *boolean(Lexer *lexer)
{
    char buffer[6] = {0}; // "true" and "false" are at most 5 characters long
    int i = 0;
    while (isalpha(lexer->current_char))
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }
    buffer[i] = '\0';
    Token *token = malloc(sizeof(Token));

    if (strcmp(buffer, "true") == 0 || strcmp(buffer, "false") == 0) {
        token->type = TOKEN_BOOL;
        token->value = strdup(buffer);
    } else {
          // If it's not "true" or "false", treat it as an identifier
        token->type = TOKEN_IDENTIFIER;
        token->value = strdup(buffer);
    }
    return token;
}


// This function identifies numbers
static Token *number(Lexer *lexer)
{
    char buffer[256] = {0};              // Initialize a buffer to store the number
    int i = 0;                           // Initialize an index for the buffer
    while (isdigit(lexer->current_char)) // While the current character is a digit
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }
    buffer[i] = '\0'; // Null-terminate the buffer

    Token *token = malloc(sizeof(Token)); // Allocate memory for a new token
    token->type = TOKEN_NUMBER;           // Set the type of the token to a number
    token->value = strdup(buffer);        // Duplicate the buffer to store the number

    return token; // Return the token
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

    Token *token = malloc(sizeof(Token));
    token->type = TOKEN_STRING;
    token->value = strdup(buffer);

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
        // Add more multi-char operators here
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
    // While the current character is a space, tab, newline, or carriage return
    while (lexer->current_char == ' ' || lexer->current_char == '\t' || lexer->current_char == '\n' || lexer->current_char == '\r')
    {
        advance(lexer);
    }
}


// Get the next token
Token *next_token(Lexer *lexer)
{
    // Skip any whitespace and comments
    skip_whitespace(lexer);
    skip_comments(lexer);

    Token *token = malloc(sizeof(Token));
    token->value = NULL;

    // If we've reached the end of the input
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
    if ((lexer->current_char == 't' && strncmp(lexer->input + lexer->position, "true", 4) == 0) ||
        (lexer->current_char == 'f' && strncmp(lexer->input + lexer->position, "false", 5) == 0))
    {
        return boolean(lexer);
    }

    
    // Check for single-character operators and tokens
    switch (lexer->current_char)
    {
    case '=':
        if (peek_char(lexer) == '=')
        {
            advance(lexer);
            token->type = TOKEN_EQUAL;
        }
        else
        {
            token->type = TOKEN_ASSIGN;
        }
        break;
    case '+': token->type = TOKEN_PLUS; token->value = strdup("+"); break;
    case '-': token->type = TOKEN_MINUS; token->value = strdup("-"); break;
    case '%': token->type = TOKEN_MODULUS; token->value = strdup("%"); break;
    case '*':
        if (peek_char(lexer) == '*')
        {
            advance(lexer); // Consume the second '*'
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
    case '"': return string(lexer);
    default:
        printf("Unknown character: %c\n", lexer->current_char);
        token->type = TOKEN_EOF;
        return token;
    }

    advance(lexer);
    return token;
}

