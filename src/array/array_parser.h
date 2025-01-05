#ifndef ARRAY_PARSER_H
#define ARRAY_PARSER_H

#include "array_lexer.h"
#include "array_ast.h"
#include "array.h"  // For forward declaration

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
ArrayASTNode *array_parse_declaration(ArrayParser *parser);
ArrayASTNode *array_parse_literal(ArrayParser *parser);
ArrayASTNode *array_parse_method_call(ArrayParser *parser, const char *array_name);
ArrayASTNode *array_parse_access(ArrayParser *parser, const char *array_name);

// Error handling
void array_parser_error(ArrayParser *parser, const char *message);

#endif // ARRAY_PARSER_H 