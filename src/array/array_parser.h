#ifndef ARRAY_PARSER_H
#define ARRAY_PARSER_H

#include "array_lexer.h"
#include "array_ast.h"
#include "array.h"

// Define the actual structure (using the forward declaration from array.h)
struct ArrayParser {
    ArrayLexer *lexer;
    ArrayToken *current_token;
    ArrayToken *peek_token;
};

// Parser initialization and cleanup
ArrayParser *array_parser_init(ArrayLexer *lexer);
void array_parser_free(ArrayParser *parser);

// Main parsing functions
ArrayASTNode *array_parse(ArrayParser *parser);

// Error handling
void array_parser_error(ArrayParser *parser, const char *message);

#endif // ARRAY_PARSER_H 