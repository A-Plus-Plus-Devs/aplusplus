#include "array_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations of helper functions
static void advance_token(ArrayParser *parser);
static bool expect_token(ArrayParser *parser, ArrayTokenType type);
static VariableType token_to_variable_type(ArrayTokenType type);
static ArrayASTNode *parse_array_elements(ArrayParser *parser);
static ArrayASTNode *parse_expression(ArrayParser *parser);

ArrayParser *array_parser_init(ArrayLexer *lexer) {
    ArrayParser *parser = malloc(sizeof(ArrayParser));
    if (!parser) return NULL;

    parser->lexer = lexer;
    parser->current_token = array_lexer_next_token(lexer);
    parser->peek_token = array_lexer_next_token(lexer);

    return parser;
}

void array_parser_free(ArrayParser *parser) {
    if (parser) {
        array_token_free(parser->current_token);
        array_token_free(parser->peek_token);
        free(parser);
    }
}

static void advance_token(ArrayParser *parser) {
    array_token_free(parser->current_token);
    parser->current_token = parser->peek_token;
    parser->peek_token = array_lexer_next_token(parser->lexer);
}

static bool expect_token(ArrayParser *parser, ArrayTokenType type) {
    if (parser->current_token->type != type) {
        char error[100];
        snprintf(error, sizeof(error), "Expected %s, got %s",
                array_token_type_to_string(type),
                array_token_type_to_string(parser->current_token->type));
        array_parser_error(parser, error);
        return false;
    }
    advance_token(parser);
    return true;
}

ArrayASTNode *array_parse(ArrayParser *parser) {
    switch (parser->current_token->type) {
        case ARRAY_TOKEN_IDENTIFIER:
            if (parser->peek_token->type == ARRAY_TOKEN_LESS_THAN) {
                return array_parse_declaration(parser);
            } else if (parser->peek_token->type == ARRAY_TOKEN_DOT) {
                char *array_name = strdup(parser->current_token->value);
                advance_token(parser); // consume identifier
                advance_token(parser); // consume dot
                ArrayASTNode *node = array_parse_method_call(parser, array_name);
                free(array_name);
                return node;
            } else if (parser->peek_token->type == ARRAY_TOKEN_LBRACKET) {
                char *array_name = strdup(parser->current_token->value);
                advance_token(parser); // consume identifier
                ArrayASTNode *node = array_parse_access(parser, array_name);
                free(array_name);
                return node;
            }
            break;
        case ARRAY_TOKEN_LBRACKET:
            return array_parse_literal(parser);
        default:
            array_parser_error(parser, "Expected array declaration, method call, or literal");
            return NULL;
    }
    return NULL;
}

ArrayASTNode *array_parse_declaration(ArrayParser *parser) {
    char *array_name = strdup(parser->current_token->value);
    advance_token(parser); // consume identifier

    if (!expect_token(parser, ARRAY_TOKEN_LESS_THAN)) return NULL;
    
    ArrayTokenType type_token = parser->current_token->type;
    if (!expect_token(parser, type_token)) return NULL;
    if (!expect_token(parser, ARRAY_TOKEN_GREATER_THAN)) return NULL;
    
    if (!expect_token(parser, ARRAY_TOKEN_EQUALS)) return NULL;
    
    ArrayASTNode *elements = array_parse_literal(parser);
    if (!elements) return NULL;

    if (!expect_token(parser, ARRAY_TOKEN_SEMICOLON)) {
        array_free_ast(elements);
        return NULL;
    }

    return array_create_declaration_node(array_name, 
                                      token_to_variable_type(type_token), 
                                      elements);
}

ArrayASTNode *array_parse_literal(ArrayParser *parser) {
    // Parse: [elem1, elem2, ...]
    if (!expect_token(parser, ARRAY_TOKEN_LBRACKET)) return NULL;
    
    ArrayASTNode *elements = parse_array_elements(parser);
    
    if (!expect_token(parser, ARRAY_TOKEN_RBRACKET)) {
        if (elements) array_free_ast(elements);
        return NULL;
    }

    return array_create_literal_node(elements);
}

ArrayASTNode *array_parse_method_call(ArrayParser *parser, const char *array_name) {
    ArrayNodeType method_type;
    switch (parser->current_token->type) {
        case ARRAY_TOKEN_METHOD_ADD_LAST:
            method_type = ARRAY_NODE_ADD_LAST;
            break;
        case ARRAY_TOKEN_METHOD_ADD_FIRST:
            method_type = ARRAY_NODE_ADD_FIRST;
            break;
        case ARRAY_TOKEN_METHOD_REMOVE_LAST:
            method_type = ARRAY_NODE_REMOVE_LAST;
            break;
        case ARRAY_TOKEN_METHOD_REMOVE_FIRST:
            method_type = ARRAY_NODE_REMOVE_FIRST;
            break;
        case ARRAY_TOKEN_METHOD_LENGTH:
            method_type = ARRAY_NODE_LENGTH;
            break;
        default:
            array_parser_error(parser, "Unknown array method");
            return NULL;
    }
    advance_token(parser);

    ArrayASTNode *argument = NULL;
    if (method_type == ARRAY_NODE_ADD_LAST || method_type == ARRAY_NODE_ADD_FIRST) {
        if (!expect_token(parser, ARRAY_TOKEN_LPAREN)) return NULL;
        argument = parse_expression(parser);
        if (!argument) return NULL;
        if (!expect_token(parser, ARRAY_TOKEN_RPAREN)) {
            array_free_ast(argument);
            return NULL;
        }
    }

    return array_create_method_node(method_type, array_name, argument);
}

ArrayASTNode *array_parse_access(ArrayParser *parser, const char *array_name) {
    if (!expect_token(parser, ARRAY_TOKEN_LBRACKET)) return NULL;
    
    ArrayASTNode *index = parse_expression(parser);
    if (!index) return NULL;

    if (!expect_token(parser, ARRAY_TOKEN_RBRACKET)) {
        array_free_ast(index);
        return NULL;
    }

    return array_create_access_node(array_name, index);
}

static ArrayASTNode *parse_array_elements(ArrayParser *parser) {
    if (parser->current_token->type == ARRAY_TOKEN_RBRACKET) {
        return NULL; // Empty array
    }

    ArrayASTNode *first_element = parse_expression(parser);
    if (!first_element) return NULL;

    ArrayASTNode *current = first_element;
    while (parser->current_token->type == ARRAY_TOKEN_COMMA) {
        advance_token(parser); // consume comma
        current->next = parse_expression(parser);
        if (!current->next) {
            array_free_ast(first_element);
            return NULL;
        }
        current = current->next;
    }

    return first_element;
}

static ArrayASTNode *parse_expression(ArrayParser *parser) {
    // For now, we'll handle basic literals
    switch (parser->current_token->type) {
        case ARRAY_TOKEN_NUMBER: {
            int value = atoi(parser->current_token->value);
            advance_token(parser);
            return array_create_element_node(INT_TYPE, &value);
        }
        case ARRAY_TOKEN_STRING: {
            char *value = parser->current_token->value;
            advance_token(parser);
            return array_create_element_node(STRING_TYPE, value);
        }
        case ARRAY_TOKEN_BOOL: {
            bool value = strcmp(parser->current_token->value, "true") == 0;
            advance_token(parser);
            return array_create_element_node(BOOL_TYPE, &value);
        }
        case ARRAY_TOKEN_CHAR: {
            char value = parser->current_token->value[0];
            advance_token(parser);
            return array_create_element_node(CHAR_TYPE, &value);
        }
        default:
            array_parser_error(parser, "Expected array element");
            return NULL;
    }
}

static VariableType token_to_variable_type(ArrayTokenType type) {
    switch (type) {
        case ARRAY_TOKEN_TYPE_INT: return INT_TYPE;
        case ARRAY_TOKEN_TYPE_STRING: return STRING_TYPE;
        case ARRAY_TOKEN_TYPE_FLOAT: return FLOAT_TYPE;
        case ARRAY_TOKEN_TYPE_BOOL: return BOOL_TYPE;
        case ARRAY_TOKEN_TYPE_CHAR: return CHAR_TYPE;
        default: return INT_TYPE; // Default to INT_TYPE
    }
}

void array_parser_error(ArrayParser *parser, const char *message) {
    fprintf(stderr, "Parser error at line %zu, column %zu: %s\n",
            parser->current_token->line,
            parser->current_token->column,
            message);
} 