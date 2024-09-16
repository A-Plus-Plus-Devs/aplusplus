# A++ Compiler

The A++ Compiler is a simple compiler for the A++ programming language. It includes a lexer, parser, code generator, and interpreter to process and execute A++ source files.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [File Descriptions](#file-descriptions)
- [Contributing](#contributing)
- [License](#license)

## Installation

To build the A++ Compiler, you need to have a C compiler installed on your system. You can use `gcc` or any other standard C compiler.

1. Clone the repository:
    ```sh
    git clone https://github.com/A-Plus-Plus-Devs/aplusplus.git
    ```

2. Build the project:
    ```sh
    make
    ```

## Usage

To use the A++ Compiler, run the following command:
```
sh
./build/bin/a++c <source_file>.a++
```
Replace `<source_file>.a++` with the path to your A++ source file.


This will compile the source file into an executable binary.

## Project Structure

The project is organized into several directories, each containing specific components of the compiler:

- `src/`: Contains the source code for the compiler.
  - `main.c`: The main entry point of the compiler.
  - `lexer/`: Contains the lexer implementation.
  - `parser/`: Contains the parser implementation.
  - `codegen/`: Contains the code generation implementation.
  - `interpreter/`: Contains the interpreter implementation.
  - `ast/`: Contains the Abstract Syntax Tree (AST) implementation.
  - `common/`: Contains common types and utilities.

## File Descriptions

### src/main.c

This is the main entry point of the compiler. It handles command-line arguments, reads the input file, and orchestrates the compilation process by calling the lexer, parser, and interpreter.

Key functions:
- `print_usage()`: Displays usage instructions.
- `run_file()`: Reads the input file and initiates the compilation process.
- `main()`: The main function that handles command-line arguments and calls `run_file()`.

### src/lexer/lexer.h

This header file defines the structure and functions for the lexer, which is responsible for breaking down the input source code into tokens.

Key components:
- `TokenType` enum: Defines all possible token types.
- `KeywordType` enum: Defines keyword types.
- `Token` struct: Represents a single token.
- `Lexer` struct: Represents the lexer state.
- Function declarations for lexer operations.

### src/lexer/lexer.c

This file implements the lexer functionality defined in `lexer.h`.

Key functions:
- `init_lexer()`: Initializes a new lexer with given input.
- `advance()`: Moves the lexer to the next character.
- `peek_char()`: Looks at the next character without advancing.
- `next_token()`: Retrieves the next token from the input.
- Various helper functions for identifying different types of tokens.

### src/parser/parser.h

This header file defines the structure and functions for the parser, which is responsible for analyzing the sequence of tokens and creating an Abstract Syntax Tree (AST).

Key components:
- `Parser` struct: Represents the parser state.
- Function declarations for parser operations.

### src/parser/parser.c

This file implements the parser functionality defined in `parser.h`.

Key functions:
- `create_parser()`: Creates a new parser with a given lexer.
- `parse_tokens()`: Parses all tokens and builds the AST.
- Various parsing functions for different language constructs (e.g., `parse_statement()`, `parse_expression()`).

### src/ast/ast.h

This header file defines the structure and functions for the Abstract Syntax Tree (AST), which represents the structure of the program.

Key components:
- `ASTNodeType` enum: Defines all possible AST node types.
- `ASTNode` struct: Represents a node in the AST.
- Function declarations for AST operations.

### src/ast/ast.c

This file implements the AST functionality defined in `ast.h`.

Key functions:
- `create_node()`: Creates a new AST node.
- `create_var_declaration_node()`: Creates a node for variable declarations.
- `create_assignment_node()`: Creates a node for assignment statements.
- `free_ast()`: Frees the memory allocated for an AST.

### src/interpreter/interpreter.h

This header file defines the function for interpreting the AST.

Key components:
- Declaration of the `interpret()` function.

### src/interpreter/interpreter.c

This file implements the interpreter functionality defined in `interpreter.h`.

Key functions:
- `interpret()`: Walks through the AST and executes each node.
- Helper functions for evaluating expressions and managing variables.

### src/common/types.h

This header file defines common types used throughout the compiler.

Key components:
- `VariableType` enum: Defines the types of variables supported by the language.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request with your changes.


