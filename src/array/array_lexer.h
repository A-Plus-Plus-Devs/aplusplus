#ifndef ARRAY_LEXER_H
#define ARRAY_LEXER_H

#include <stddef.h>
#include "array.h"  // For forward declaration

// Token types specific to array operations
typedef enum {
    ARRAY_TOKEN_EOF = 0,
    ARRAY_TOKEN_IDENTIFIER = 1,    // Variable names
    ARRAY_TOKEN_NUMBER = 2,        // Numeric literals
    ARRAY_TOKEN_STRING = 3,        // String literals
    ARRAY_TOKEN_BOOL = 4,          // Boolean literals
    ARRAY_TOKEN_CHAR = 5,          // Character literals
    ARRAY_TOKEN_LBRACKET = 6,     // [
    ARRAY_TOKEN_RBRACKET = 7,     // ]
    ARRAY_TOKEN_COMMA = 8,        // ,
    ARRAY_TOKEN_DOT = 9,          // .
    ARRAY_TOKEN_SEMICOLON = 10,   // ;
    ARRAY_TOKEN_LESS_THAN = 11,    // <
    ARRAY_TOKEN_GREATER_THAN = 12, // >
    ARRAY_TOKEN_ASSIGN = 13,       // =
    ARRAY_TOKEN_TYPE_INT = 14,     // "int"
    ARRAY_TOKEN_TYPE_STRING = 15,  // "string"
    ARRAY_TOKEN_TYPE_FLOAT = 16,   // "float"
    ARRAY_TOKEN_TYPE_BOOL = 17,    // "bool"
    ARRAY_TOKEN_TYPE_CHAR = 18,    // "char"
    ARRAY_TOKEN_METHOD_ADD_LAST = 19,    // "addLast"
    ARRAY_TOKEN_METHOD_ADD_FIRST = 20,   // "addFirst"
    ARRAY_TOKEN_METHOD_REMOVE_LAST = 21, // "removeLast"
    ARRAY_TOKEN_METHOD_REMOVE_FIRST = 22,// "removeFirst"
    ARRAY_TOKEN_METHOD_LENGTH = 23,      // "length"
    ARRAY_TOKEN_LPAREN = 24,      // (
    ARRAY_TOKEN_RPAREN = 25,      // )
    ARRAY_TOKEN_TERM = 10,        // ; (same as SEMICOLON)
    ARRAY_TOKEN_ERROR = 27,       // Invalid token
    ARRAY_TOKEN_PRINT = 28,       // print keyword
} ArrayTokenType;

// Token structure
typedef struct {
    ArrayTokenType type;
    char *value;
    size_t line;
    size_t column;
} ArrayToken;

// Define the actual structure (using the forward declaration from array.h)
struct ArrayLexer {
    const char *input;
    size_t position;
    size_t read_position;
    char current_char;
    size_t line;
    size_t column;
};

// Lexer functions
ArrayLexer *array_lexer_init(const char *input);
void array_lexer_free(ArrayLexer *lexer);
ArrayToken *array_lexer_next_token(ArrayLexer *lexer);
void array_token_free(ArrayToken *token);

// Utility functions
const char *array_token_type_to_string(ArrayTokenType type);
void array_lexer_error(ArrayLexer *lexer, const char *message);

#endif // ARRAY_LEXER_H 