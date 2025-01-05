#ifndef ARRAY_LEXER_H
#define ARRAY_LEXER_H

#include <stddef.h>

// Token types specific to array operations
typedef enum {
    ARRAY_TOKEN_EOF,
    ARRAY_TOKEN_IDENTIFIER,    // Variable names
    ARRAY_TOKEN_NUMBER,        // Numeric literals
    ARRAY_TOKEN_STRING,        // String literals
    ARRAY_TOKEN_BOOL,         // Boolean literals
    ARRAY_TOKEN_CHAR,         // Character literals
    ARRAY_TOKEN_LBRACKET,     // [
    ARRAY_TOKEN_RBRACKET,     // ]
    ARRAY_TOKEN_COMMA,        // ,
    ARRAY_TOKEN_DOT,          // .
    ARRAY_TOKEN_LESS_THAN,    // <
    ARRAY_TOKEN_GREATER_THAN, // >
    ARRAY_TOKEN_EQUALS,       // =
    ARRAY_TOKEN_TYPE_INT,     // "int"
    ARRAY_TOKEN_TYPE_STRING,  // "string"
    ARRAY_TOKEN_TYPE_FLOAT,   // "float"
    ARRAY_TOKEN_TYPE_BOOL,    // "bool"
    ARRAY_TOKEN_TYPE_CHAR,    // "char"
    ARRAY_TOKEN_METHOD_ADD_LAST,    // "addLast"
    ARRAY_TOKEN_METHOD_ADD_FIRST,   // "addFirst"
    ARRAY_TOKEN_METHOD_REMOVE_LAST, // "removeLast"
    ARRAY_TOKEN_METHOD_REMOVE_FIRST,// "removeFirst"
    ARRAY_TOKEN_METHOD_LENGTH,      // "length"
    ARRAY_TOKEN_LPAREN,      // (
    ARRAY_TOKEN_RPAREN,      // )
    ARRAY_TOKEN_SEMICOLON,   // ;
    ARRAY_TOKEN_ERROR        // Invalid token
} ArrayTokenType;

// Token structure
typedef struct {
    ArrayTokenType type;
    char *value;
    size_t line;
    size_t column;
} ArrayToken;

// Lexer structure
typedef struct {
    const char *input;
    size_t position;
    size_t read_position;
    char current_char;
    size_t line;
    size_t column;
} ArrayLexer;

// Lexer functions
ArrayLexer *array_lexer_init(const char *input);
void array_lexer_free(ArrayLexer *lexer);
ArrayToken *array_lexer_next_token(ArrayLexer *lexer);
void array_token_free(ArrayToken *token);

// Utility functions
const char *array_token_type_to_string(ArrayTokenType type);
void array_lexer_error(ArrayLexer *lexer, const char *message);

#endif // ARRAY_LEXER_H 