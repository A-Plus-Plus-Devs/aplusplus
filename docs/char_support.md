# Adding Character Literal Support to A++

## Problem Statement
The A++ language needed support for character literals to allow users to work with single characters. This required changes across multiple components of the compiler to properly handle character literals like `'a'` in both variable declarations and print statements.

## Initial Issues
When attempting to use character literals, several issues were encountered:
```c
char c = 'a';
print('b');
```
This code produced multiple errors:
1. "Unexpected token in statement: Type 50, Value char"
2. "ERROR: Expected ')' after print argument"
3. Issues with token recognition and parsing

## Analysis and Implementation

### 1. Type System Update
First, we added character type support to the type system:
```c
typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    BOOL_TYPE,
    CHAR_TYPE  // Added character support
} VariableType;
```

### 2. Lexer Changes
The lexer needed proper handling of character literals:

#### Before:
```c
static Token *char_literal(Lexer *lexer)
{
    char buffer[2] = {0};
    advance(lexer); // Skip the opening quote
    buffer[0] = lexer->current_char;
    advance(lexer); // Skip the closing quote
    buffer[1] = '\0';
    Token *token = malloc(sizeof(Token));
    token->type = TOKEN_CHAR;
    token->value = strdup(buffer);
    return token;
}
```

#### After:
```c
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
```

The key improvement was proper handling of the closing quote and error checking.

### 3. Parser Updates
The parser needed to recognize character literals in expressions and variable declarations:

1. Added `TOKEN_CHAR_TYPE` to the statement parser:
```c
switch (parser->current_token->type) {
    case TOKEN_INT_TYPE:
    case TOKEN_FLOAT_TYPE:
    case TOKEN_STRING_TYPE:
    case TOKEN_BOOL_TYPE:
    case TOKEN_CHAR_TYPE:  // Added support for char type
        return parse_var_declaration(parser);
    // ...
}
```

2. Added character literal handling in parse_factor:
```c
else if (token->type == TOKEN_CHAR)
{
    ASTNode *node = create_node(NODE_CHAR_LITERAL, NULL, NULL, token->value);
    get_next_token(parser);
    return node;
}
```

### 4. Interpreter Changes
The interpreter needed updates to handle character values:

1. Added char_value to Variable union:
```c
union {
    int int_value;
    double float_value;
    char *string_value;
    bool bool_value;
    char char_value;  // Added for character support
} value;
```

2. Added character literal evaluation:
```c
else if (node->type == NODE_CHAR_LITERAL)
{
    return node->value[0];
}
```

3. Added character printing support:
```c
else if (node->left->type == NODE_CHAR_LITERAL ||
         (node->left->type == NODE_LITERAL && get_variable(node->left->value)->type == CHAR_TYPE))
{
    char result = evaluate_expression(node->left);
    printf("%c\n", result);
}
```

## Solution
The final implementation allows for:
1. Character variable declarations: `char c = 'a';`
2. Character literal printing: `print('b');`
3. Proper error handling for malformed character literals
4. Integration with existing type system and expression evaluation

## Testing
The implementation was tested with basic character operations:
```c
char c = 'a';
print('b');
```
Output:
```
b
```

## Future Enhancements
Possible future enhancements could include:
1. Support for escape sequences (`'\n'`, `'\t'`, etc.)
2. Character arithmetic operations
3. Character comparison operations
4. Multi-byte character support
