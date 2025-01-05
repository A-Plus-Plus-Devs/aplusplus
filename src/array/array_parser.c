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
static ArrayASTNode *array_parse_statement(ArrayParser *parser);
static ArrayASTNode *array_parse_method_call(ArrayParser *parser, const char *array_name);
static ArrayASTNode *array_parse_array_operation(ArrayParser *parser, const char *name);
static ArrayASTNode *array_parse_literal(ArrayParser *parser);
static void debug_token(ArrayParser *parser, const char *location);

// Debug function implementation
static void debug_token(ArrayParser *parser, const char *location) {
    printf("DEBUG [%s] Current token: type=%d, value='%s'\n", 
           location,
           parser->current_token->type,
           parser->current_token->value ? parser->current_token->value : "NULL");
}

// Helper function implementations
static void advance_token(ArrayParser *parser) {
    printf("DEBUG: Advancing token from type=%d", 
           parser->current_token ? parser->current_token->type : -1);
    
    array_token_free(parser->current_token);
    parser->current_token = parser->peek_token;
    parser->peek_token = array_lexer_next_token(parser->lexer);
    
    printf(" to type=%d\n", 
           parser->current_token ? parser->current_token->type : -1);
}

static bool expect_token(ArrayParser *parser, ArrayTokenType type) {
    printf("DEBUG: Expecting token type %d, got %d\n", type, parser->current_token->type);
    
    if (parser->current_token->type != type) {
        char error[100];
        snprintf(error, sizeof(error), "Expected token type %d, got %d", 
                type, parser->current_token->type);
        array_parser_error(parser, error);
        return false;
    }
    advance_token(parser);
    return true;
}

// Implementation of array_parse_literal
static ArrayASTNode *array_parse_literal(ArrayParser *parser) {
    printf("DEBUG: Parsing array literal\n");
    return parse_array_elements(parser);
}

// Implementation of array_parse_method_call
static ArrayASTNode *array_parse_method_call(ArrayParser *parser, const char *array_name) {
    printf("DEBUG: Parsing method call for array '%s'\n", array_name);
    debug_token(parser, "method_call");

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

    if (method_type != ARRAY_NODE_LENGTH) {
        if (!expect_token(parser, ARRAY_TOKEN_LPAREN)) return NULL;
        argument = array_parse_expression(parser);
        if (!argument) return NULL;
        if (!expect_token(parser, ARRAY_TOKEN_RPAREN)) {
            array_free_ast(argument);
            return NULL;
        }
    }

    if (!expect_token(parser, ARRAY_TOKEN_TERM)) {
        if (argument) array_free_ast(argument);
        return NULL;
    }

    return array_create_method_node(method_type, array_name, argument);
}

// Add the implementation of array_parse_array_operation
static ArrayASTNode *array_parse_array_operation(ArrayParser *parser, const char *name) {
    printf("DEBUG: Entering array_parse_array_operation with name='%s'\n", name);
    debug_token(parser, "array_parse_array_operation");

    if (parser->current_token->type == ARRAY_TOKEN_LESS_THAN) {
        printf("DEBUG: Found array declaration pattern for array '%s'\n", name);
        advance_token(parser); // consume <

        // Parse type
        if (parser->current_token->type != ARRAY_TOKEN_TYPE_INT &&
            parser->current_token->type != ARRAY_TOKEN_TYPE_STRING &&
            parser->current_token->type != ARRAY_TOKEN_TYPE_FLOAT &&
            parser->current_token->type != ARRAY_TOKEN_TYPE_BOOL &&
            parser->current_token->type != ARRAY_TOKEN_TYPE_CHAR) {
            array_parser_error(parser, "Expected type after <");
            return NULL;
        }

        VariableType type = token_to_variable_type(parser->current_token->type);
        printf("DEBUG: Array type: %d\n", type);
        advance_token(parser);

        if (!expect_token(parser, ARRAY_TOKEN_GREATER_THAN)) return NULL;
        if (!expect_token(parser, ARRAY_TOKEN_ASSIGN)) return NULL;
        if (!expect_token(parser, ARRAY_TOKEN_LBRACKET)) return NULL;

        ArrayASTNode *elements = parse_array_elements(parser);
        if (!elements) return NULL;

        if (!expect_token(parser, ARRAY_TOKEN_RBRACKET)) {
            array_free_ast(elements);
            return NULL;
        }

        if (!expect_token(parser, ARRAY_TOKEN_TERM)) {
            array_free_ast(elements);
            return NULL;
        }

        return array_create_declaration_node(name, type, elements);
    } 
    else if (parser->current_token->type == ARRAY_TOKEN_DOT) {
        printf("DEBUG: Found method call pattern for array '%s'\n", name);
        advance_token(parser); // consume dot
        return array_parse_method_call(parser, name);
    }

    printf("DEBUG: No valid array operation found for '%s'\n", name);
    return NULL;
}

// Rest of the implementations...

// Main parsing function
ArrayASTNode *array_parse(ArrayParser *parser) {
    printf("DEBUG: Starting array parse\n");
    ArrayASTNode *first = NULL;
    ArrayASTNode *current = NULL;

    while (parser->current_token->type != ARRAY_TOKEN_EOF) {
        printf("DEBUG: Parsing statement\n");
        debug_token(parser, "main_loop");

        ArrayASTNode *statement = NULL;
        
        if (parser->current_token->type == ARRAY_TOKEN_IDENTIFIER) {
            // Handle array operations
            const char *array_name = strdup(parser->current_token->value);
            printf("DEBUG: Found identifier: %s\n", array_name);
            advance_token(parser);

            if (parser->current_token->type == ARRAY_TOKEN_LESS_THAN ||
                parser->current_token->type == ARRAY_TOKEN_DOT) {
                statement = array_parse_array_operation(parser, array_name);
            }
            free((void*)array_name);
        }
        else if (parser->current_token->type == ARRAY_TOKEN_TYPE_INT) {
            // Handle variable declarations
            advance_token(parser);
            if (parser->current_token->type == ARRAY_TOKEN_IDENTIFIER) {
                const char *var_name = strdup(parser->current_token->value);
                advance_token(parser);
                
                if (parser->current_token->type == ARRAY_TOKEN_ASSIGN) {
                    advance_token(parser);
                    if (parser->current_token->type == ARRAY_TOKEN_IDENTIFIER) {
                        const char *array_name = strdup(parser->current_token->value);
                        advance_token(parser);
                        if (parser->current_token->type == ARRAY_TOKEN_DOT) {
                            advance_token(parser);
                            if (parser->current_token->type == ARRAY_TOKEN_METHOD_LENGTH) {
                                statement = array_create_length_node(array_name, var_name);
                                advance_token(parser);
                                expect_token(parser, ARRAY_TOKEN_TERM);
                            }
                        }
                        free((void*)array_name);
                    }
                }
                free((void*)var_name);
            }
        }
        else if (parser->current_token->type == ARRAY_TOKEN_PRINT) {
            advance_token(parser);
            if (expect_token(parser, ARRAY_TOKEN_LPAREN)) {
                if (parser->current_token->type == ARRAY_TOKEN_IDENTIFIER) {
                    statement = array_create_print_node(
                        array_create_identifier_node(parser->current_token->value)
                    );
                    advance_token(parser);
                    expect_token(parser, ARRAY_TOKEN_RPAREN);
                    expect_token(parser, ARRAY_TOKEN_TERM);
                }
            }
        }

        if (statement) {
            if (!first) {
                first = statement;
                current = statement;
            } else {
                current->next = statement;
                current = statement;
            }
        }
    }

    return first;
}

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

static ArrayASTNode *array_parse_statement(ArrayParser *parser) {
    switch (parser->current_token->type) {
        case ARRAY_TOKEN_IDENTIFIER: {
            const char *name = parser->current_token->value;
            advance_token(parser);
            
            if (parser->current_token->type == ARRAY_TOKEN_LESS_THAN ||
                parser->current_token->type == ARRAY_TOKEN_DOT) {
                return array_parse_array_operation(parser, name);
            }
            
            if (parser->current_token->type == ARRAY_TOKEN_ASSIGN) {
                advance_token(parser);
                ArrayASTNode *value = array_parse_expression(parser);
                expect_token(parser, ARRAY_TOKEN_TERM);
                return array_create_assignment_node(name, value);
            }
        }
        break;

        case ARRAY_TOKEN_PRINT: {
            advance_token(parser);
            expect_token(parser, ARRAY_TOKEN_LPAREN);
            ArrayASTNode *expr = array_parse_expression(parser);
            expect_token(parser, ARRAY_TOKEN_RPAREN);
            expect_token(parser, ARRAY_TOKEN_TERM);
            return array_create_print_node(expr);
        }
        break;

        default:
            array_parser_error(parser, "Unexpected token in statement");
            return NULL;
    }
    return NULL;
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
    
    if (!expect_token(parser, ARRAY_TOKEN_ASSIGN)) {
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
    printf("DEBUG: Starting to parse array elements\n");
    ArrayASTNode *first = NULL;
    ArrayASTNode *current = NULL;

    while (parser->current_token->type != ARRAY_TOKEN_RBRACKET) {
        printf("DEBUG: Parsing array element\n");
        debug_token(parser, "parse_array_elements");

        ArrayASTNode *element = array_parse_expression(parser);
        if (!element) {
            printf("DEBUG: Failed to parse element\n");
            if (first) array_free_ast(first);
            return NULL;
        }

        if (!first) {
            first = element;
            current = element;
        } else {
            current->next = element;
            current = element;
        }

        if (parser->current_token->type == ARRAY_TOKEN_COMMA) {
            printf("DEBUG: Found comma, continuing to next element\n");
            advance_token(parser);
        } else {
            printf("DEBUG: No comma found, ending element list\n");
            break;
        }
    }

    printf("DEBUG: Finished parsing array elements\n");
    return first;
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
            parser->lexer->line,
            parser->lexer->column,
            message);
} 