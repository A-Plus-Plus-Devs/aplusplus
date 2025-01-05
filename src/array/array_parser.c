#include "array_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations of helper functions
static void advance_token(ArrayParser *parser);
static bool expect_token(ArrayParser *parser, ArrayTokenType type);
static VariableType token_to_variable_type(ArrayTokenType type);
static ArrayASTNode *parse_array_elements(ArrayParser *parser);
static ArrayASTNode *array_parse_expression(ArrayParser *parser);

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
        snprintf(error, sizeof(error), "Expected %d, got %d", type, parser->current_token->type);
        array_parser_error(parser, error);
        return false;
    }
    advance_token(parser);
    return true;
}

ArrayASTNode *array_parse(ArrayParser *parser) {
    if (!parser || !parser->current_token) {
        array_parser_error(parser, "Invalid parser state");
        return NULL;
    }

    // Create a root node to hold all statements
    ArrayASTNode *root = NULL;
    ArrayASTNode *current = NULL;

    printf("Starting to parse array operations\n");

    while (parser->current_token && parser->current_token->type != ARRAY_TOKEN_EOF) {
        ArrayASTNode *statement = NULL;
        printf("Current token type: %d, value: %s\n", 
               parser->current_token->type,
               parser->current_token->value ? parser->current_token->value : "NULL");

        if (parser->current_token->type == ARRAY_TOKEN_IDENTIFIER) {
            const char *identifier = parser->current_token->value;
            advance_token(parser);  // Move past identifier

            // Check what comes after the identifier
            if (parser->current_token->type == ARRAY_TOKEN_LESS_THAN) {
                // Array declaration: numbers<int> = [1, 2, 3];
                printf("Parsing array declaration\n");
                statement = array_parse_declaration(parser);
            }
            else if (parser->current_token->type == ARRAY_TOKEN_DOT) {
                // Method call: numbers.addLast(4);
                printf("Parsing method call\n");
                advance_token(parser);  // Move past dot
                statement = array_parse_method_call(parser, identifier);
            }
        }

        if (!statement) {
            // Skip to next semicolon if we failed to parse
            printf("Skipping to next statement\n");
            while (parser->current_token && 
                   parser->current_token->type != ARRAY_TOKEN_SEMICOLON && 
                   parser->current_token->type != ARRAY_TOKEN_EOF) {
                advance_token(parser);
            }
            if (parser->current_token && parser->current_token->type == ARRAY_TOKEN_SEMICOLON) {
                advance_token(parser);
            }
            continue;
        }

        // Add statement to our list
        if (!root) {
            root = statement;
            current = root;
        } else {
            current->next = statement;
            current = statement;
        }

        // Skip semicolon if present
        if (parser->current_token && parser->current_token->type == ARRAY_TOKEN_SEMICOLON) {
            advance_token(parser);
        }
    }

    return root;
}

ArrayASTNode *array_parse_declaration(ArrayParser *parser) {
    // Save the identifier name
    const char *array_name = strdup(parser->current_token->value);
    advance_token(parser); // Move past identifier
    
    // Expect <type>
    if (!expect_token(parser, ARRAY_TOKEN_LESS_THAN)) {
        free((void*)array_name);
        return NULL;
    }
    
    // Get the type
    VariableType type;
    switch (parser->current_token->type) {
        case ARRAY_TOKEN_TYPE_INT:
            type = INT_TYPE;
            break;
        case ARRAY_TOKEN_TYPE_STRING:
            type = STRING_TYPE;
            break;
        case ARRAY_TOKEN_TYPE_FLOAT:
            type = FLOAT_TYPE;
            break;
        case ARRAY_TOKEN_TYPE_BOOL:
            type = BOOL_TYPE;
            break;
        case ARRAY_TOKEN_TYPE_CHAR:
            type = CHAR_TYPE;
            break;
        default:
            array_parser_error(parser, "Invalid array type");
            free((void*)array_name);
            return NULL;
    }
    advance_token(parser); // Move past type
    
    if (!expect_token(parser, ARRAY_TOKEN_GREATER_THAN)) {
        free((void*)array_name);
        return NULL;
    }
    
    if (!expect_token(parser, ARRAY_TOKEN_EQUALS)) {
        free((void*)array_name);
        return NULL;
    }
    
    // Parse array literal
    ArrayASTNode *elements = array_parse_literal(parser);
    if (!elements) {
        free((void*)array_name);
        return NULL;
    }
    
    return array_create_declaration_node(array_name, type, elements);
}

ArrayASTNode *array_parse_literal(ArrayParser *parser) {
    if (!expect_token(parser, ARRAY_TOKEN_LBRACKET)) return NULL;
    
    ArrayASTNode *elements = parse_array_elements(parser);
    
    if (!expect_token(parser, ARRAY_TOKEN_RBRACKET)) {
        array_free_ast(elements);
        return NULL;
    }
    
    return array_create_literal_node(elements);
}

ArrayASTNode *array_parse_method_call(ArrayParser *parser, const char *array_name) {
    printf("Parsing method call for array: %s\n", array_name);
    
    // Get the method name
    if (!parser->current_token) {
        array_parser_error(parser, "Unexpected end of input");
        return NULL;
    }

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
            printf("Unknown method token type: %d\n", parser->current_token->type);
            array_parser_error(parser, "Unknown array method");
            return NULL;
    }
    advance_token(parser);  // Move past method name

    // Handle method arguments if needed
    ArrayASTNode *argument = NULL;
    if (method_type == ARRAY_NODE_ADD_LAST || method_type == ARRAY_NODE_ADD_FIRST) {
        if (!parser->current_token || parser->current_token->type != ARRAY_TOKEN_LPAREN) {
            array_parser_error(parser, "Expected '(' after method name");
            return NULL;
        }
        advance_token(parser);  // Move past '('

        // Parse argument
        argument = array_parse_expression(parser);
        if (!argument) {
            array_parser_error(parser, "Invalid method argument");
            return NULL;
        }

        if (!parser->current_token || parser->current_token->type != ARRAY_TOKEN_RPAREN) {
            array_parser_error(parser, "Expected ')' after method argument");
            array_free_ast(argument);
            return NULL;
        }
        advance_token(parser);  // Move past ')'
    }

    return array_create_method_node(method_type, array_name, argument);
}

ArrayASTNode *array_parse_access(ArrayParser *parser, const char *array_name) {
    if (!expect_token(parser, ARRAY_TOKEN_LBRACKET)) return NULL;
    
    ArrayASTNode *index = array_parse_expression(parser);
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

    ArrayASTNode *first_element = array_parse_expression(parser);
    if (!first_element) return NULL;

    ArrayASTNode *current = first_element;
    while (parser->current_token->type == ARRAY_TOKEN_COMMA) {
        advance_token(parser); // consume comma
        current->next = array_parse_expression(parser);
        if (!current->next) {
            array_free_ast(first_element);
            return NULL;
        }
        current = current->next;
    }

    return first_element;
}

static ArrayASTNode *array_parse_expression(ArrayParser *parser) {
    if (!parser->current_token) return NULL;

    printf("Parsing expression, current token type: %d\n", parser->current_token->type);

    switch (parser->current_token->type) {
        case ARRAY_TOKEN_NUMBER: {
            int value = atoi(parser->current_token->value);
            advance_token(parser);
            return array_create_element_node(INT_TYPE, &value);
        }
        case ARRAY_TOKEN_STRING: {
            char *value = strdup(parser->current_token->value);
            advance_token(parser);
            return array_create_element_node(STRING_TYPE, value);
        }
        case ARRAY_TOKEN_BOOL: {
            bool value = (strcmp(parser->current_token->value, "true") == 0);
            advance_token(parser);
            return array_create_element_node(BOOL_TYPE, &value);
        }
        case ARRAY_TOKEN_CHAR: {
            char value = parser->current_token->value[0];
            advance_token(parser);
            return array_create_element_node(CHAR_TYPE, &value);
        }
        default:
            array_parser_error(parser, "Unexpected token in expression");
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