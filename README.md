# A++ Programming Language & Compiler

A++ is a modern programming language and compiler written in C, designed for readability and ease of use while maintaining powerful features. The project implements a complete compilation pipeline including lexer, parser, and interpreter.

View site [here](https://a-plus-plus-devs.github.io/aplusplus/)

## Features

- **Rich Type System:**
  - Basic types: int, float, boolean, char, string
  - Array support with built-in methods
  - Dynamic type casting
  - Type inference for variables

- **Control Flow:**
  - If-else statements with else-if support
  - For loops with flexible iteration
  - Ternary operators (?:)
  - Function definitions and calls
  - Yield statements

- **Operators:**
  - Arithmetic: +, -, *, /, %, ** (power)
  - Comparison: <, >, <=, >=, ==, !=
  - Logical: &&, ||, !
  - Bitwise: &, |, ^, ~, <<, >>
  - Compound assignment: +=, -=, etc.

- **Array Operations:**
  - Array declarations and literals
  - Index-based access
  - Array method calls
  - Dynamic array manipulation

- **Input/Output:**
  - Multiple print keywords: `print`, `echo`, `yap`
  - Input handling for user interaction
  - Error reporting system

- **Development Tools:**
  - Comprehensive test suite
  - Version management system
  - Cross-platform support (Windows, Linux, macOS)
  - Detailed logging and debugging features

## Installation

Requirements:
- GCC or compatible C compiler
- Make
- Git

1. Clone the repository:
```sh
git clone https://github.com/A-Plus-Plus-Devs/aplusplus.git
```

2. Build the project:
```sh
make
```

## Usage

### Running Programs

Use the appropriate script for your platform:

**Unix/Linux/macOS:**
```sh
./src/scripts/a++ program.a++
```

**Windows CMD:**
```cmd
src\scripts\a++.cmd program.a++
```

**Windows PowerShell:**
```powershell
.\src\scripts\a++.ps1 program.a++
```

### Version Management

Control version numbers using:

```sh
# Bump version numbers
./scripts/version.sh bump major|minor|patch

# Set specific version
./scripts/version.sh set X.Y.Z

# List all versions
./scripts/version.sh list

# Show current version
./scripts/version.sh current
```

### Running Tests

Execute the test suite using the appropriate script:

```sh
# Unix/Linux/macOS
./tests/run_tests.sh

# Windows CMD
tests\run_tests.cmd

# Windows PowerShell
.\tests\run_tests.ps1
```

## Project Structure

```
src/
├── ast/          # Abstract Syntax Tree implementation
├── common/       # Shared types and utilities
├── interpreter/  # AST execution engine
├── lexer/        # Source code tokenization
├── parser/       # AST generation from tokens
├── scripts/      # Platform-specific launcher scripts
└── main.c        # Compiler entry point

scripts/          # Version management tools
tests/            # Test suite and runners
```

## Development Status

Current features:
- Complete lexer and parser implementation
- AST-based interpretation
- Variable and type management
- Control flow statements
- Basic string traversal
- Function support
- Cross-platform compatibility

Under development:
- Array operations (currently in parser implementation phase)
- Bitwise operations
- Standard library expansion
- Performance optimizations
- Enhanced error handling
- Documentation improvements

Known Issues:
- Array operations not yet functional
- Bitwise operations still in implementation
- Character and string handling improvements needed
- Some mathematical edge cases need addressing

## Issue Tracking

The A++ project uses a centralized issue tracking system on GitHub to manage development tasks, bug reports, and feature requests.

### A++ Issue Tracker Features

- **Issue Creation**: Detailed issue reporting with descriptions and examples
- **Project Assignment**: Organized project boards for different aspects
- **Status Tracking**: Clear progress monitoring
- **Contributor Collaboration**: Streamlined task management

### Creating Issues

1. Navigate to the **Issues** tab
2. Click **New Issue**
3. Provide:
   - Clear, descriptive title
   - Detailed problem/feature description
   - Example code (if applicable)
   - Expected behavior
   - Current behavior (for bugs)

Example Issue Format:
```markdown
# Feature/Bug Title

## Current Issues
- Detailed description of problems
- Examples of incorrect behavior

## Expected Behavior
- What should happen
- Example of correct output

## Implementation Notes
- Affected components
- Suggested fixes
```

### Issue Status Labels

- **Open**: New issue awaiting action
- **In Progress**: Work has started
- **Blocked**: Waiting on dependencies
- **Closed**: Resolved or dismissed

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Documentation Standards

#### 1. Feature Documentation
Create a new markdown file in `docs/` for significant features or changes:
```
docs/
├── feature_name.md
├── float_improvements.md
├── for_loop_implementation.md
└── ...
```

Documentation should include:
- Issue/Purpose description
- Implementation details
- Example usage
- Testing information
- Future improvements

#### 2. File Headers
All source files must maintain a documentation header:
```c
/*
 * Component Name
 * 
 * Brief description of what this file does.
 * 
 * Key features:
 * - Feature 1
 * - Feature 2
 * 
 * Original Author: [Name]
 * Created: [Date]
 * 
 * Edited by:
 * - [Contributor] - [Date] - [Brief description of changes]
 *
 * File: src/component/file.c
 */
```

#### 3. Script Headers
All script files must include:
```bash
#!/bin/bash

# Script Name
# 
# Brief description of what this script does.
# 
# Key features:
# - Feature 1
# - Feature 2
#
# Original Author: [Name]
# Created: [Date]
# 
# Edited by:
# - [Contributor] - [Date] - [Brief description of changes]
#
# File: scripts/script.sh
```

Please ensure your code:
- Includes appropriate documentation headers
- Follows the existing code style
- Includes relevant tests
- Updates version numbers appropriately
- Creates/updates relevant documentation files

### Working with Issues

1. Check the [A++ Issue Tracker](https://github.com/A-Plus-Plus-Devs/projects/1) for open tasks
2. Comment on issues you'd like to work on
3. Reference issue numbers in commits and pull requests
4. Update issue status as you make progress

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Authors

- **Paul Kabulu** - *Initial work* - Created August 2024

See also the list of contributors who have participated in this project.
