/*
 * Lexer Implementation
 * 
 * This file implements the lexical analyser (lexer) for the A++ programming language.
 * It converts source code text into a sequence of tokens for the parser.
 * 
 * Key features:
 * - Token identification and classification
 * - Keyword recognition
 * - Number and string literal handling
 * - Operator and symbol processing
 * - Comment handling
 * - Error detection and reporting
 *
 * Original Author: Paul Kabulu
 * Created: August 2024
 * 
 * Edited by: Paul Kabulu - January 2025 - Added array methods keywords
 *
 * File: src/lexer/lexer.c
 */

#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Initialise the lexer
Lexer *init_lexer(const char *input)
{
    // printf("[DEBUG] Initializing lexer with input starting with: %.20s...\n", input);
    
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer)); // Allocate memory for a new Lexer structure
    if (!lexer) {
        // printf("[DEBUG] Error: Failed to allocate memory for lexer\n");
        return NULL;
    }

    lexer->input = input;                          // Set the input string for the lexer
    lexer->position = 0;                           // Initialize the current position to the start of the input
    lexer->read_position = 1;                      // Set the read position to the next character
    lexer->current_char = input[0];                // Set the current character to the first character of the input

    // printf("[DEBUG] Lexer initialized. First char: '%c'\n", lexer->current_char);
    return lexer;                                  // Return the newly created lexer
}

// This function moves the lexer to the next character
void advance(Lexer *lexer)
{
    if (!lexer) {
        // printf("[DEBUG] Error: Null lexer in advance\n");
        return;
    }
    
    // If we haven't reached the end of the input
    if (lexer->read_position < strlen(lexer->input))
    {
        lexer->position = lexer->read_position;              // Move the current position forward
        lexer->current_char = lexer->input[lexer->position]; // Set the current character to the next character
        lexer->read_position++;                              // Move the read position forward
        // printf("[DEBUG] Advanced to char: '%c'\n", lexer->current_char);
    }
    else
    {
        // If we've reached the end, set the current character to null
        lexer->current_char = '\0';
        // printf("[DEBUG] Advanced to end of input\n");
    }
}

// This function identifies keywords or identifiers
static Token *identifier_or_keyword(Lexer *lexer)
{
    char buffer[256] = {0};
    int i = 0;
    
    // Consume the entire identifier
    while (isalnum(lexer->current_char) || lexer->current_char == '_')
    {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }
    buffer[i] = '\0';

    Token *token = malloc(sizeof(Token));
    token->value = strdup(buffer);

    // Check for keywords
    if (strcmp(buffer, "import") == 0)
        token->type = TOKEN_IMPORT;
    else if (strcmp(buffer, "from") == 0)
        token->type = TOKEN_FROM;
    else if (strcmp(buffer, "export") == 0)
        token->type = TOKEN_EXPORT;
    else if (strcmp(buffer, "length") == 0)
        token->type = TOKEN_LENGTH;
    else if (strcmp(buffer, "charAt") == 0)
        token->type = TOKEN_CHAR_AT;
    else if (strcmp(buffer, "substring") == 0)
        token->type = TOKEN_SUBSTRING;
    else if (strcmp(buffer, "concat") == 0)
        token->type = TOKEN_CONCAT;
    else if (strcmp(buffer, "replace") == 0)
        token->type = TOKEN_REPLACE;
    else if (strcmp(buffer, "toLowerCase") == 0)
        token->type = TOKEN_LOWERCASE;
    else if (strcmp(buffer, "toUpperCase") == 0)
        token->type = TOKEN_UPPERCASE;
    else if (strcmp(buffer, "reverse") == 0)
        token->type = TOKEN_REVERSE;
    else if (strcmp(buffer, "indexOf") == 0)
        token->type = TOKEN_INDEX_OF;
    else if (strcmp(buffer, "trim") == 0)
        token->type = TOKEN_TRIM;
    else if (strcmp(buffer, "repeat") == 0)
        token->type = TOKEN_REPEAT;
    else if (strcmp(buffer, "abs") == 0)
        token->type = TOKEN_ABS;
    else if (strcmp(buffer, "ceil") == 0)
        token->type = TOKEN_CEIL;
    else if (strcmp(buffer, "floor") == 0)
        token->type = TOKEN_FLOOR;
    else if (strcmp(buffer, "round") == 0)
        token->type = TOKEN_ROUND;
    else if (strcmp(buffer, "sqrt") == 0)
        token->type = TOKEN_SQRT;
    else if (strcmp(buffer, "cbrt") == 0)
        token->type = TOKEN_CBRT;
    else if (strcmp(buffer, "root") == 0)
        token->type = TOKEN_ROOT;
    else if (strcmp(buffer, "sin") == 0)
        token->type = TOKEN_SIN;
    else if (strcmp(buffer, "cos") == 0)
        token->type = TOKEN_COS;
    else if (strcmp(buffer, "tan") == 0)
        token->type = TOKEN_TAN;
    else if (strcmp(buffer, "arctan") == 0)
        token->type = TOKEN_ARCTAN;
    else if (strcmp(buffer, "arcsin") == 0)
        token->type = TOKEN_ARCSIN;
    else if (strcmp(buffer, "arccos") == 0)
        token->type = TOKEN_ARCCOS;
    else if (strcmp(buffer, "max") == 0)
        token->type = TOKEN_MAX;
    else if (strcmp(buffer, "min") == 0)
        token->type = TOKEN_MIN;
    else if (strcmp(buffer, "random") == 0)
        token->type = TOKEN_RANDOM;
    else if (strcmp(buffer, "date") == 0)
        token->type = TOKEN_DATE;
    else if (strcmp(buffer, "time") == 0)
        token->type = TOKEN_TIME;
    else if (strcmp(buffer, "now") == 0)
        token->type = TOKEN_NOW;
    else if (strcmp(buffer, "toPrecision") == 0)
        token->type = TOKEN_TO_PRECISION;
    else if (strcmp(buffer, "if") == 0)
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
    else if (strcmp(buffer, "char") == 0)
        token->type = TOKEN_CHAR_TYPE;
    else if (strcmp(buffer, "yield") == 0)
        token->type = TOKEN_YIELD;
    else if (strcmp(buffer, "addLast") == 0)
        token->type = TOKEN_ADD_LAST;
    else if (strcmp(buffer, "removeLast") == 0)
        token->type = TOKEN_REMOVE_LAST;
    else if (strcmp(buffer, "addFirst") == 0)
        token->type = TOKEN_ADD_FIRST;
    else if (strcmp(buffer, "removeFirst") == 0)
        token->type = TOKEN_REMOVE_FIRST;
    else if (strcmp(buffer, "nothing") == 0)
        token->type = TOKEN_NOTHING_TYPE;
    else if (strcmp(buffer, "empty") == 0)
        token->type = TOKEN_EMPTY;
    else if (strcmp(buffer, "input") == 0)
        token->type = TOKEN_INPUT;
    else if (strcmp(buffer, "insert") == 0)
        token->type = TOKEN_INSERT;
    else
        token->type = TOKEN_IDENTIFIER;

    // Skip any whitespace after the identifier
    while (isspace(lexer->current_char)) {
        advance(lexer);
    }

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
        // Handle escape sequences
        if (lexer->current_char == '\\')
        {
            advance(lexer); // Move past the backslash
            switch (lexer->current_char)
            {
                case 'n':
                    buffer[i++] = '\n';
                    break;
                default:
                    // For unsupported escape sequences, just include the character
                    buffer[i++] = lexer->current_char;
                    break;
            }
        }
        else
        {
            buffer[i++] = lexer->current_char;
        }
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

    return token;
}



static Token *char_literal(Lexer *lexer)
{
    char buffer[2] = {0};
    advance(lexer); // Skip the opening quote
    buffer[0] = lexer->current_char;
    advance(lexer); // Move to closing quote
    if (lexer->current_char != '\'') {
        printf("Error: Expected closing quote for character literal\n");
        buffer[0] = '\0';
    }
    advance(lexer); // Skip the closing quote
    buffer[1] = '\0';
    Token *token = malloc(sizeof(Token));
    token->type = TOKEN_CHAR;
    token->value = strdup(buffer);
    return token;
}

// This function looks at the next character without moving the lexer
char peek_char(Lexer *lexer)
{
    if (lexer->read_position >= strlen(lexer->input))
    {
        return '\0';
    }
    return lexer->input[lexer->read_position];
}

//function to peek at the next non-whitespace character
char peek_next_non_whitespace(Lexer *lexer)
{
    size_t pos = lexer->position;
    while (pos < strlen(lexer->input))
    {
        char c = lexer->input[pos];
        if (!isspace(c))
        {
            return c;
        }
        pos++;
    }
    return '\0';
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

static Token* array_type(Lexer* lexer) {
    Token* token = malloc(sizeof(Token));
    char buffer[256] = {0};
    int i = 0;
    
    // Skip the '<'
    advance(lexer);
    
    // Read the type name
    while (isalnum(lexer->current_char)) {
        buffer[i++] = lexer->current_char;
        advance(lexer);
    }
    
    // Expect closing '>'
    if (lexer->current_char != '>') {
        // printf("[DEBUG] Expected '>', got '%c'\n", lexer->current_char);
        free(token);
        return NULL;
    }
    advance(lexer); // consume '>'
    
    buffer[i] = '\0';
    token->type = TOKEN_ARRAY_TYPE;
    token->value = strdup(buffer);
    return token;
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
        if (peek_char(lexer) == '=') {
            advance(lexer);
            token->type = TOKEN_LESS_THAN_OR_EQUAL;
        } else {
            // Check for array type annotation
          if (isalpha(peek_char(lexer))) {
            free(token);
            return array_type(lexer);
        } else {
            token->type = TOKEN_LESS_THAN;
            token->value = strdup("<");
            advance(lexer);
        }
        }
        advance(lexer);
        return token;

    case '+':
        if (peek_char(lexer) == '+') {
            token->type = TOKEN_INCREMENT;
            token->value = strdup("++");
            advance(lexer); // consume first '+'
            advance(lexer); // consume second '+'
        } else if (peek_char(lexer) == '=') {
            token->type = TOKEN_PLUS_ASSIGN;
            token->value = strdup("+=");
            advance(lexer); // consume '+'
            advance(lexer); // consume '='
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
        } else if (peek_char(lexer) == '=') {
            token->type = TOKEN_MINUS_ASSIGN;
            token->value = strdup("-=");
            advance(lexer); // consume '-'
            advance(lexer); // consume '='
        } else {
            token->type = TOKEN_MINUS;
            token->value = strdup("-");
            advance(lexer);
        }
        return token;

    case '*':
        if (peek_char(lexer) == '*') {
            token->type = TOKEN_POWER;
            token->value = strdup("**");
            advance(lexer); // consume first '*'
            advance(lexer); // consume second '*'
        } else if (peek_char(lexer) == '=') {
            token->type = TOKEN_MUL_ASSIGN;
            token->value = strdup("*=");
            advance(lexer); // consume '*'
            advance(lexer); // consume '='
        } else {
            token->type = TOKEN_MULTIPLY;
            token->value = strdup("*");
            advance(lexer);
        }
        return token;

    case '/':
        if (peek_char(lexer) == '/') {
            skip_comments(lexer);
            free(token);
            return next_token(lexer);
        } else if (peek_char(lexer) == '*') {
            skip_comments(lexer);
            free(token);
            return next_token(lexer);
        } else if (peek_char(lexer) == '=') {
            token->type = TOKEN_DIV_ASSIGN;
            token->value = strdup("/=");
            advance(lexer); // consume '/'
            advance(lexer); // consume '='
        } else {
            token->type = TOKEN_DIVIDE;
            token->value = strdup("/");
            advance(lexer);
        }
        return token;

    case '%':
        if (peek_char(lexer) == '=') {
            token->type = TOKEN_MOD_ASSIGN;
            token->value = strdup("%=");
            advance(lexer); // consume '%'
            advance(lexer); // consume '='
        } else {
            token->type = TOKEN_MODULUS;
            token->value = strdup("%");
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

    case '(':
        advance(lexer);
        token->type = TOKEN_LPAREN;
        token->value = strdup("(");
        return token;

    case ')':
        advance(lexer);
        token->type = TOKEN_RPAREN;
        token->value = strdup(")");
        return token;

    case ';':
        advance(lexer);
        token->type = TOKEN_SEMICOLON;
        token->value = strdup(";");
        return token;

    case ',':
        advance(lexer);
        token->type = TOKEN_COMMA;
        token->value = strdup(",");
        return token;

    case '{':
        advance(lexer);
        token->type = TOKEN_LBRACE;
        token->value = strdup("{");
        return token;

    case '}':
        advance(lexer);
        token->type = TOKEN_RBRACE;
        token->value = strdup("}");
        return token;

    case '?':
        token->type = TOKEN_QUESTION;
        token->value = strdup("?");
        advance(lexer);
        return token;
            
    case ':':
        token->type = TOKEN_COLON;
        token->value = strdup(":");
        advance(lexer);
        return token;

    case '!':
        if (peek_char(lexer) == '=') {
            token->type = TOKEN_NOT_EQUAL;
            token->value = strdup("!=");
            advance(lexer); // consume '!'
            advance(lexer); // consume '='
        } else {
            token->type = TOKEN_NOT;
            token->value = strdup("!");
            advance(lexer);
        }
        return token;

    case '"':
        free(token); // Free the token we created since string() creates its own
        return string(lexer);
    case '\'':
        free(token); // Free the token we created since char_literal() creates its own
        return char_literal(lexer);
    case '\n':
    case '\r':
        advance(lexer);
        return next_token(lexer); // Skip newlines and get next token

    case '#':
        advance(lexer); // consume #
        if (strncmp(lexer->input + lexer->position, "define", 6) == 0)
        {
            for (int i = 0; i < 6; i++) advance(lexer); // consume "define"
            token->type = TOKEN_DEFINE;
            token->value = strdup("#define");
            return token;
        }
        token->type = TOKEN_UNKNOWN;
        token->value = strdup("#");
        return token;

    case '.':
        token->type = TOKEN_DOT;
        advance(lexer);
        
        // Check for method names
        if (isalpha(lexer->current_char)) {
            char method_buffer[256] = {0};
            int i = 0;
            
            while (isalpha(lexer->current_char)) {
                method_buffer[i++] = lexer->current_char;
                advance(lexer);
            }
            method_buffer[i] = '\0';
            
            // Check for specific method names
            if (strcmp(method_buffer, "addFirst") == 0)
                token->type = TOKEN_ADD_FIRST;
            else if (strcmp(method_buffer, "addLast") == 0)
                token->type = TOKEN_ADD_LAST;
            else if (strcmp(method_buffer, "removeFirst") == 0)
                token->type = TOKEN_REMOVE_FIRST;
            else if (strcmp(method_buffer, "removeLast") == 0)
                token->type = TOKEN_REMOVE_LAST;
            else if (strcmp(method_buffer, "length") == 0)
                token->type = TOKEN_LENGTH;
            else if (strcmp(method_buffer, "insert") == 0)
                token->type = TOKEN_INSERT;
            else

                token->type = TOKEN_METHOD_CALL;
            
            token->value = strdup(method_buffer);
        }
        return token;

    case '[':
        token->type = TOKEN_LBRACKET;
        token->value = strdup("[");
        advance(lexer);
        return token;

    case ']':
        token->type = TOKEN_RBRACKET;
        token->value = strdup("]");
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

// Get the next token without advancing the lexer's state
Token *peek_next_token(Lexer *lexer) {
    // Save current lexer state
    size_t saved_position = lexer->position;
    size_t saved_read_position = lexer->read_position;
    char saved_current_char = lexer->current_char;
    
    // Get next token
    Token *token = next_token(lexer);
    
    // Restore lexer state
    lexer->position = saved_position;
    lexer->read_position = saved_read_position;
    lexer->current_char = saved_current_char;
    
    return token;
}
