#ifndef LEXER_H
#define LEXER_H

#include <stddef.h> // For size_t

// Token types
typedef enum
{
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_ASSIGN,
    TOKEN_FLOAT,
    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_CHAR_AT,
    // NODE_LITERAL,
    TOKEN_NUMBER,
    TOKEN_PRINT,
    TOKEN_BOOL,
    TOKEN_KEYWORD,
    TOKEN_LBRACE,                // {
    TOKEN_RBRACE,                // }
    TOKEN_QUESTION,              // ?
    TOKEN_COLON,                 // :
    TOKEN_SEMICOLON,             // ;
    TOKEN_LPAREN,                // (
    TOKEN_RPAREN,                // )
    TOKEN_LBRACKET,              // [
    TOKEN_RBRACKET,              // ]
    TOKEN_COMMA,                 // ,
    TOKEN_DOT,                   // .
    TOKEN_PLUS,                  // +
    TOKEN_MINUS,                 // -
    TOKEN_MULTIPLY,              // *
    TOKEN_DIVIDE,                // /
    TOKEN_MODULUS,               // %
    TOKEN_POWER,                 // **
    TOKEN_NEWLINE,               // \n
    TOKEN_EQUAL,                 // ==
    TOKEN_NOT_EQUAL,             // !=
    TOKEN_GREATER_THAN,          // >
    TOKEN_LESS_THAN,             // <
    TOKEN_GREATER_THAN_OR_EQUAL, // >=
    TOKEN_LESS_THAN_OR_EQUAL,    // <=
    TOKEN_NOT,                   // !
    TOKEN_BITWISE_AND,           // &
    TOKEN_BITWISE_OR,            // |
    TOKEN_BITWISE_XOR,           // ^
    TOKEN_BITWISE_NOT,           // ~
    TOKEN_BITWISE_SHIFT_LEFT,    // <<
    TOKEN_BITWISE_SHIFT_RIGHT,   // >>
    TOKEN_UNKNOWN,               // For
    TOKEN_INT_TYPE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELSEIF,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_FLOAT_TYPE,
    TOKEN_CHAR_TYPE,
    TOKEN_SHORT_TYPE,
    TOKEN_BOOL_TYPE,
    TOKEN_LONG_TYPE,
    TOKEN_UNSIGNED_TYPE,
    TOKEN_SIGNED_TYPE,
    TOKEN_DOUBLE_TYPE,
    TOKEN_STRING_TYPE,
    TOKEN_FOR,
    TOKEN_INCREMENT,    // ++
    TOKEN_DECREMENT,    // --
    TOKEN_LOGICAL_AND,  // &&
    TOKEN_LOGICAL_OR,   // ||
    TOKEN_DEFINE,       // #define
    TOKEN_YIELD,        // yield
    TOKEN_FUNCTION,     // function
    TOKEN_TYPE,         // type declarations (int, float, string, etc.)
    TOKEN_PLUS_ASSIGN,  // +=
    TOKEN_MINUS_ASSIGN, // -=
    TOKEN_MUL_ASSIGN,   // *=
    TOKEN_DIV_ASSIGN,   // /=
    TOKEN_MOD_ASSIGN,   // %=
    TOKEN_ARRAY_TYPE,   // <type>
    TOKEN_METHOD_CALL,  // .methodName
    TOKEN_ADD_LAST,     // .addLast
    TOKEN_REMOVE_LAST,  // .removeLast
    TOKEN_LENGTH,       // length function
    TOKEN_ADD_FIRST,    // .addFirst
    TOKEN_REMOVE_FIRST, // .removeFirst
    TOKEN_NOTHING_TYPE,  // For 'nothing' keyword
    TOKEN_EMPTY,        // For 'empty' keyword
    TOKEN_INPUT,
    TOKEN_INDEX_OF,
    TOKEN_SUBSTRING,    
    TOKEN_CONCAT,
    TOKEN_REPLACE,
    TOKEN_LOWERCASE,
    TOKEN_UPPERCASE,
    TOKEN_REVERSE,
    TOKEN_TRIM,
    TOKEN_REPEAT,
    // TOKEN_REMOVE_CHAR,
    // TOKEN_REMOVE_CHAR_AT,
    // TOKEN_ADD_CHAR,
    // TOKEN_ADD_CHAR_AT,
    TOKEN_ABS,
    TOKEN_CEIL,
    TOKEN_FLOOR,
    TOKEN_ROUND,
    TOKEN_SQRT,
    TOKEN_CBRT,
    TOKEN_ROOT,
    TOKEN_SIN,
    TOKEN_COS,
    TOKEN_TAN,
    TOKEN_ARCTAN,
    TOKEN_ARCSIN,
    TOKEN_ARCCOS,
    TOKEN_MAX,
    TOKEN_MIN,
    TOKEN_RANDOM,
    TOKEN_RANDOM_INT,
    TOKEN_RANDOM_FLOAT,
    TOKEN_DATE,
    TOKEN_TIME,
    TOKEN_NOW,
    TOKEN_TO_PRECISION
} TokenType;

// Define keywords
typedef enum
{
    KEYWORD_PRINT,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_ELSEIF,
    KEYWORD_BREAK,
    KEYWORD_CONTINUE,
    KEYWORD_INT,
    KEYWORD_FLOAT,
    KEYWORD_BOOL,
    KEYWORD_VOID,
    KEYWORD_RETURN,
    KEYWORD_CONST,
    KEYWORD_CHAR,
    KEYWORD_SHORT,
    KEYWORD_LONG,
    KEYWORD_UNSIGNED,
    KEYWORD_SIGNED,
    KEYWORD_DOUBLE,
    KEYWORD_STRING,
    KEYWORD_DEFINE,    // #define keyword
    KEYWORD_YIELD,     // yield keyword
    KEYWORD_FUNCTION,  // function keyword
} KeywordType;

// Token structure
typedef struct
{
    TokenType type;
    union
    {
        char *value;         // For identifiers and literals
        KeywordType keyword; // For keywords
    };
} Token;

// Lexer structure
typedef struct
{
    const char *input;
    size_t position;      // Current position in the input
    size_t read_position; // Current reading position in the input
    char current_char;
} Lexer;

/**
 * @brief Initializes a new lexer with the given input.
 * 
 * @param input The source code string to be tokenized.
 * @return Lexer* A pointer to the newly created Lexer structure.
 */
Lexer *init_lexer(const char *input);

/**
 * @brief Advances the lexer to the next character in the input.
 * 
 * @param lexer A pointer to the Lexer structure.
 */
void advance(Lexer *lexer);

/**
 * @brief Peeks at the next character in the input without advancing the lexer.
 * 
 * @param lexer A pointer to the Lexer structure.
 * @return char The next character in the input.
 */
char peek_char(Lexer *lexer);

/**
 * @brief Peeks ahead to find the next non-whitespace character without advancing the lexer.
 * 
 * @param lexer A pointer to the Lexer structure.
 * @return char The next non-whitespace character in the input.
 */
char peek_next_non_whitespace(Lexer *lexer);

/**
 * @brief Retrieves the next token from the input.
 * 
 * @param lexer A pointer to the Lexer structure.
 * @return Token* A pointer to the next Token in the input.
 */
Token *next_token(Lexer *lexer);

// Get the next token without advancing the lexer
Token *peek_next_token(Lexer *lexer);

#endif // LEXER_H
