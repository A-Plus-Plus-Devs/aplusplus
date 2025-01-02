# Testing in A++

## Overview

A++ comes with a comprehensive testing framework that helps ensure the reliability of your code. The testing system supports multiple shells and operating systems.

## Test Structure

### Test Files
The A++ testing suite includes several key test files:

- `basic_test.a++`: Simple tests for core functionality
- `comprehensive_test.a++`: Complete language feature tests
- `edge_cases_test.a++`: Tests for boundary conditions and error cases
- `for_loop_test.a++`: Specific tests for loop functionality

## Running Tests

### Using the Test Runner

Choose the appropriate test runner for your shell:

```bash
# On Bash (Linux/macOS)
./run_tests.sh            # Run all tests
./run_tests.sh basic      # Run basic tests only
./run_tests.sh -v        # Run with verbose output

# On Zsh (macOS default)
./run_tests.zsh          # Run all tests
./run_tests.zsh basic    # Run basic tests only
./run_tests.zsh -v      # Run with verbose output

# On Windows (PowerShell)
.\run_tests.ps1          # Run all tests
.\run_tests.ps1 basic    # Run basic tests only
.\run_tests.ps1 -v      # Run with verbose output

# On Windows (CMD)
run_tests.cmd           # Run all tests
run_tests.cmd basic     # Run basic tests only
run_tests.cmd -v       # Run with verbose output
```

### Test Output
Tests produce color-coded output across all shells:
- 🟢 Green text for passed tests
- 🔴 Red text for failed tests
- 🟡 Yellow text for warnings/info
- Detailed error messages for failures

Example output:
```
Running A++ Test Suite...
✅ Basic Operations
  ✅ Integer printing
  ✅ Float printing
  ✅ String printing
❌ Variable Operations
  ❌ Assignment failed
    Expected: 42
    Got: 41
```

### Log Files
Test results are automatically logged with timestamps:
```
logs/
  test_run_20240101_120000.log     # Test output log
  test_run_20240101_120000.failures # Failure details
```

## Writing Tests

### Test File Structure
```c
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

### Expected Output Files
Each test can have two associated files:
- `testname.expected`: Contains expected output patterns
- `testname.errors`: Contains expected error messages

Example:
```
# basic_test.expected
Result: 8
Result: 15

# basic_test.errors
Division by zero
```

## Shell-Specific Notes

### PowerShell
- Requires PowerShell 5.0 or later
- May need execution policy adjustment:
```powershell
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### CMD
- Run as administrator for certain operations
- Uses Windows-style path separators
- Compatible with Windows 7 and later

### Bash/Zsh
- Requires execute permissions:
```bash
chmod +x run_tests.sh
chmod +x run_tests.zsh
```
- Uses POSIX-style paths

## Error Handling

The testing framework handles various types of errors:

- Syntax errors
- Runtime errors
- Type mismatches
- Stack overflow
- Memory limits

### Error Output Format
Errors are reported consistently across shells:
```
Error in test_file.a++ (line 42):
  Expected: <expected_output>
  Got: <actual_output>
  Details: <error_message>
```

## Test Configuration

### Environment Variables
```bash
A_PLUS_PLUS_TEST_TIMEOUT=30    # Test timeout in seconds
A_PLUS_PLUS_TEST_VERBOSE=1     # Enable verbose output
A_PLUS_PLUS_TEST_COLOR=1       # Enable colored output
```

### Command Line Options
```
-v, --verbose    Enable verbose output
-q, --quiet      Suppress all output except errors
-t, --timeout    Set test timeout (seconds)
-f, --filter     Run tests matching pattern
```

## Contributing

To contribute new tests:

1. Create a new test file in the `tests` directory
2. Create corresponding `.expected` and `.errors` files
3. Test on multiple shells if possible
4. Submit a pull request

### Testing Best Practices
1. Comment expected outputs
2. Group related tests together
3. Include edge cases
4. Test error conditions
5. Keep test cases focused
6. Test across different shells
7. Use appropriate line endings for each OS

For more information, check the [A++ GitHub repository](https://github.com/A-Plus-Plus-Devs/aplusplus).
