# Function Implementation Documentation

## Task Overview
Adding function support to the language with the following syntax:
```
#define <type> name(<type> param1,..) {
    // function body
    yield result;
}
```

Key features:
- Return type specified after #define
- 'nothing' as void type
- 'yield' keyword for returning values
- Support for multiple parameters

## Technical Components Involved
1. Parser
2. AST
3. Lexer
4. Interpreter

## Approach

### 1. AST Changes Needed
- New AST node types:
  - FunctionDefinition
  - FunctionParameter
  - YieldStatement
  - FunctionCall

### 2. Parser Changes Required
- Add function definition parsing
- Parameter list parsing
- Function body parsing
- Yield statement parsing

### 3. Debug Logging Strategy
- Add detailed logging for:
  - Function definition entry/exit
  - Parameter parsing
  - Scope management
  - Yield statement processing
  - Function call resolution

### 4. File Structure
To maintain code stability:
- Create new files for function-related code
- Separate function parsing logic
- Implement isolated testing

## Edge Cases and Considerations
1. Recursive function calls
2. Parameter type checking
3. Return type validation
4. Scope management
5. Function redefinition handling

## Questions and Clarifications
1. Should functions be allowed to be redefined?
2. How should forward declarations be handled?
3. Is there a maximum number of parameters?
4. How should function overloading be handled?

## Next Steps
1. Create new AST nodes for functions
2. Implement function parsing
3. Add debug logging
4. Create test cases
5. Implement semantic validation



## Implementation Details

### 1. AST Structure
- **Function Definition Node**: Stores return type, name, parameters, and body
- **Function Parameter Node**: Stores parameter type and name
- **Yield Statement Node**: Contains the expression to return
- **Function Call Node**: Contains function name and argument list

### 2. Parser Implementation
The parser handles functions through several specialized parsers:

#### Function Definition Parser
```c
static ASTNode *parse_function_definition(Parser *parser)
{
    // Parse return type
    // Parse function name
    // Parse parameters
    // Parse function body
    return create_function_definition_node(return_type, name, params, body);
}
```

#### Parameter Parser
- Handles multiple parameters with types
- Supports all basic types: int, float, string, char, bool
- Creates parameter nodes for each argument

#### Yield Statement Parser
```c
static ASTNode *parse_yield_statement(Parser *parser)
{
    // Parse yield expression
    // Create yield node with expression as left child
    return create_node(NODE_YIELD_STATEMENT, expr, NULL, NULL);
}
```

### 3. Interpreter Implementation
The interpreter manages function execution through:

#### Function Registration
- Functions are stored in a global function table
- Each function entry contains:
  - Name
  - Return type
  - Parameter list
  - Function body AST

#### Function Execution
```c
char *execute_function(Function *func, ASTNode *args)
{
    // Set up function scope
    // Execute function body
    // Handle yield statement
    // Return result as string
}
```

#### Type Handling
The interpreter supports returning:
- Integers: Converted to string representation
- Floats: Uses %g format for precision
- Strings: Direct string values
- Booleans: "yup" or "nope"
- Characters: Single character strings

### 4. Working Examples

#### String Function
```
#define string sayHello() {
    yield "Hello, World!";
}
```

#### Math Function
```
#define float calculateArea(float radius) {
    yield 3.14159 * radius ** 2;
}
```

#### String Concatenation
```
#define string getFullName(string first, string last) {
    string fullName = first + " " + last;
    yield fullName;
}
```

#### Comparison Function
```
#define int max(int x, int y) {
    if (x > y) {
        yield x;
    }
    yield y;
}
```

## Current Limitations and Future Improvements
1. No support for recursive functions yet
2. Limited type checking on function calls
3. No function overloading
4. No forward declarations
5. Global scope only (no nested functions)

## Testing Strategy
1. Test basic function calls
2. Test parameter passing
3. Test return type handling
4. Test error conditions
5. Test complex expressions in yield statements
