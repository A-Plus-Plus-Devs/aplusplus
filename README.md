# A++ Programming Language & Compiler

A++ is a custom programming language and compiler written in C, featuring unique syntax elements and a straightforward compilation pipeline. The project implements a lexer, parser, and interpreter to execute A++ source files.

## Features

- **Custom Syntax Elements:**
  - Boolean literals: `yup` (true) and `nope` (false)
  - Multiple print keywords: `print`, `echo`, `yap`
  - C-like syntax with unique additions

- **Type System:**
  - Basic types: int, float, boolean, char, string
  - Dynamic type conversion
  - Variable declarations and assignments

- **Control Flow:**
  - If-else statements
  - For loops
  - Ternary operators (?:)

- **Operators:**
  - Arithmetic: +, -, *, /, %, ** (power)
  - Comparison: <, >, <=, >=, ==, !=
  - Logical: &&, ||, !
  - Bitwise: &, |, ^, ~, <<, >>

- **Comments:**
  - Single-line (//)
  - Multi-line (/* */)

## Installation

Requirements:
- GCC or any standard C compiler
- Make

1. Clone the repository:
```sh
git clone https://github.com/A-Plus-Plus-Devs/aplusplus.git
```

2. Build the project:
```sh
make
```

## Usage

Run A++ programs using the compiler:
```sh
./build/bin/a++c <source_file>.a++
```

Or use the convenient wrapper script:
```sh
./src/scripts/a++ <source_file>.a++
```

To make the A++ script globally accessible on macOS (run .a++ files from anywhere):

1. Make the script executable:
```sh
chmod +x src/scripts/a++
```

2. Create a symbolic link in your local bin directory:
```sh
ln -s "$(pwd)/src/scripts/a++" /usr/local/bin/a++
```

Now you can run A++ files from any directory:
```sh
a++ yourprogram.a++
```

The script automatically recompiles the compiler if source files have changed.

## Project Structure

- `src/`: Source code directory
  - `main.c`: Compiler entry point
  - `lexer/`: Tokenization of source code
  - `parser/`: AST generation from tokens
  - `ast/`: Abstract Syntax Tree implementation
  - `interpreter/`: AST execution engine
  - `common/`: Shared types and utilities
  - `scripts/`: Build and execution tools

## Current Limitations

- Fixed variable limit (100 variables)
- Single scope implementation
- No function support yet
- Basic error handling
- No file I/O beyond program input

## Development Status

The compiler currently supports:
- Basic arithmetic and logical operations
- Control flow statements
- Variable management
- Print statements
- Type conversions

Active development focuses on:
- Expanding core functionality
- Adding function support
- Implementing proper scoping
- Enhancing error messages
- Building standard library

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is open source and available under the MIT License.
