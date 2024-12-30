# Testing in A++

## Overview

A++ comes with a comprehensive testing framework that helps ensure the reliability of your code. This guide will walk you through the testing system and show you how to write and run tests.

## Test Structure

### Test Files
The A++ testing suite includes several key test files:

- `basic_test.a++`: Simple tests for core functionality
- `comprehensive_test.a++`: Complete language feature tests
- `edge_cases_test.a++`: Tests for boundary conditions and error cases
- `for_loop_test.a++`: Specific tests for loop functionality

### Test Categories

#### 1. Basic Operations
```a++
yap(42);         // Integer printing
yap(3.14);       // Float printing
yap("Hello");    // String printing
yap(yup);        // Boolean printing
```

#### 2. Variable Operations
```a++
num x = 42;
float pi = 3.14;
text greeting = "Hello";
bool flag = yup;
```

#### 3. Arithmetic Tests
```a++
echo(1 + 1);      // Addition
echo(5 - 3);      // Subtraction
echo(4 * 2);      // Multiplication
echo(8 / 2);      // Division
echo(7 % 3);      // Modulus
```

#### 4. Control Flow
```a++
if (x > 0) {
    print("Positive";)
} else {
    print("Non-positive");
}

for (num i = 0; i < 5; i = i + 1) {
    print(i);
}
```

## Running Tests

### Using the Test Runner
The `run_tests.sh` script automates test execution:

```bash
./run_tests.sh            # Run all tests
./run_tests.sh basic      # Run basic tests only
./run_tests.sh -v        # Run with verbose output
```

### Test Output
Tests produce color-coded output:
- Green checkmark for passed tests
- Red X for failed tests
- Detailed error messages for failures

Example output:
```
Running A++ Test Suite...
Green checkmark Basic Operations
  Green checkmark Integer printing
  Green checkmark Float printing
  Green checkmark String printing
Red X Variable Operations
  Red X Assignment failed
    Expected: 42
    Got: 41
```

## Writing Tests

### Test File Structure
```a++
// Test file header
// Description: Basic arithmetic tests
// Author: Your Name

// Test case 1: Addition
num a = 5;
num b = 3;
print(a + b);  // Expected: 8

// Test case 2: Multiplication
print(a * b);  // Expected: 15
```

### Best Practices
1. Comment expected outputs
2. Group related tests together
3. Include edge cases
4. Test error conditions
5. Keep test cases focused

## Error Handling

The testing framework handles various types of errors:

- Syntax errors
- Runtime errors
- Type mismatches
- Stack overflow
- Memory limits

## Recent Updates

- Added timing information for test runs
- Improved error reporting
- Enhanced log file generation
- Added test categorization

## Contributing

To contribute new tests:

1. Create a new test file in the `tests` directory
2. Follow the test file structure
3. Update `run_tests.sh` if needed
4. Submit a pull request

For more information, check the [A++ GitHub repository](https://github.com/A-Plus-Plus-Devs/aplusplus).
