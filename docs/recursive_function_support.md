# Recursive Function Support in A++

## Issue
The factorial function in A++ was not working correctly because the interpreter didn't properly handle recursive function calls in expressions.

## Technical Analysis

### Components Involved
1. AST Node Types:
   - `NODE_FUNCTION_CALL`: Represents a function call in the AST
   - `NODE_BINARY_OP`: Represents binary operations (like multiplication)

2. Key Functions:
   - `evaluate_expression`: Evaluates expressions and returns integer results
   - `execute_function`: Executes a function and returns its result as a string

### Root Cause
The `evaluate_expression` function didn't handle `NODE_FUNCTION_CALL` nodes, which meant that recursive calls in expressions like `n * factorial(n - 1)` weren't being evaluated properly.

### Solution
Added support for function calls in `evaluate_expression` by:
1. Detecting `NODE_FUNCTION_CALL` nodes
2. Executing the function using `execute_function`
3. Converting the string result to an integer
4. Properly managing memory by freeing the result string

## Example
```a++
#define int factorial(int n) {
    if (n <= 1) {
        yield 1;
    }
    yield n * factorial(n - 1);
}
```

Now when evaluating `factorial(5)`:
1. First checks if n <= 1 (false)
2. Evaluates `n * factorial(n - 1)`
   - n = 5
   - Recursively calls factorial(4)
   - Continues until reaching factorial(1)
3. Returns 120 (5 * 4 * 3 * 2 * 1)

## Benefits
1. Enables proper recursive function support
2. Maintains memory safety through proper string result management
3. Preserves existing expression evaluation logic
