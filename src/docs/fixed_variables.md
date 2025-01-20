# Implementation of Fixed (Constant) Variables

## Task Description
Implement constant variables in Y language using the 'fixed' keyword, similar to how constants work in JavaScript and Java.

## Technical Approach

### Key Components to Modify:
1. Lexer: Add 'fixed' as a new keyword token
2. Parser: Handle the 'fixed' keyword in variable declarations
3. AST: Add support for constant variable nodes
4. Interpreter: Implement constant variable behavior

### Implementation Details

#### Constant Variable Behavior:
- Variables declared with 'fixed' cannot be reassigned after initialization
- Must be initialized at declaration time
- Block-scoped (similar to JavaScript let/const)
- Can be used in any scope (global, function, block)

#### Error Cases to Handle:
1. Reassignment attempt to a fixed variable
2. Declaration without initialization
3. Redeclaration of fixed variables in same scope

## Next Steps
1. Add lexer token for 'fixed' keyword
2. Implement parser rules for fixed variable declarations
3. Create AST node type for constant variables
4. Add semantic analysis for constant reassignment prevention
5. Update interpreter to enforce constant behavior

## Questions/Assumptions
- Should fixed variables be hoisted? (Assuming no, like JavaScript const)
- Should we allow shadowing in nested scopes? (Assuming yes, like most languages)
