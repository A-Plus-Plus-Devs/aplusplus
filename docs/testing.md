# A++ Testing Documentation

## Overview
This document describes the testing framework and test cases implemented for the A++ programming language compiler. The testing system is designed to verify the correct implementation of language features and catch potential regressions.

## Test Structure

### 1. Test Files
- `comprehensive_test.a++`: Main test file containing all language feature tests
- `basic_test.a++`: Simple tests for basic functionality
- `run_tests.sh`: Test runner script that executes and verifies test outputs

### 2. Test Categories
The comprehensive test suite covers the following categories:

#### Basic Print Tests
- Integer printing
- Float printing
- String printing
- Boolean printing (yup/nope)
- Empty string printing
- Escaped character printing

#### Variable Tests
- Integer variable declaration and assignment
- Float variable declaration and assignment
- String variable declaration and assignment
- Boolean variable declaration and assignment
- Variable reassignment
- Variable-to-variable assignment

#### Arithmetic Tests
- Addition
- Subtraction
- Multiplication
- Division
- Modulus
- Operator precedence
- Parenthesized expressions

#### Loop Tests
- Basic for loop functionality
- Loop counter
- Nested loops
- Break statement

#### Conditional Tests
- Basic if statements
- if-else statements
- Nested if statements
- if-else chains

#### Comparison Tests
- Less than (<)
- Greater than (>)
- Equal to (==)
- Not equal to (!=)
- Greater than or equal to (>=)
- Less than or equal to (<=)

#### Logical Operator Tests
- AND operator (&&)
- OR operator (||)
- NOT operator (!)
- Combined logical expressions

## Test Runner Features

### 1. Output Verification
The test runner (`run_tests.sh`) includes:
- Colored output for pass/fail status
- Section-by-section verification
- Detailed error reporting
- Full test output logging

### 2. Test Results
For each test case, the runner:
- Displays a checkmark (✓) for passed tests
- Shows the test category and specific test
- Provides the expected and actual output
- Creates a detailed log file

### 3. Error Handling
The test runner handles:
- Compiler errors
- Runtime errors
- Missing expected output
- Unexpected output

## Recent Fixes and Improvements

### 1. Boolean Operations
- Fixed logical operator evaluation (&&, ||, !)
- Corrected output formatting for boolean values (yup/nope)
- Added proper operator precedence handling

### 2. Test Runner Enhancements
- Added test timing information
- Improved error message formatting
- Added test categorization
- Enhanced log file generation

## Running Tests

### Basic Usage
```bash
# Run all tests
./run_tests.sh

# View test logs
cat ./temp/test_log.txt
```

### Test Output Example
```
Running A++ Comprehensive Tests...
✓ [Print] Found expected output: === Basic Print Tests ===
✓ [Print Integer] Found expected output: 42
✓ [Print Float] Found expected output: 3.14
...
```

## Adding New Tests

To add new tests:
1. Add test cases to `comprehensive_test.a++`
2. Update expected outputs in `run_tests.sh`
3. Run the test suite to verify

Example test case:
```cpp
// Test category
print("=== New Feature Tests ===");

// Test case with comment
print(some_expression);  // Should print expected_output
```

## Best Practices

1. **Test Organization**
   - Group related tests together
   - Add clear comments for expected output
   - Use consistent formatting

2. **Test Coverage**
   - Test both valid and invalid cases
   - Include edge cases
   - Test operator combinations

3. **Maintenance**
   - Update tests when language features change
   - Keep test output messages clear and descriptive
   - Document any special test cases
