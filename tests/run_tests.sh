#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Create logs directory if it doesn't exist
mkdir -p logs

# Set up log file with timestamp
LOG_FILE="logs/test_run_$(date +%Y%m%d_%H%M%S).log"

# Function to log message to both console and file
log() {
    echo -e "$1"
    echo -e "${1//$GREEN/}${1//$RED/}${1//$NC/}" >> "$LOG_FILE"
}

log "Running A++ Comprehensive Tests..."
log "Test started at: $(date)"
log "\nTest Environment:"
log "----------------"
log "OS: $(uname -s)"
log "Compiler Version: $(../build/bin/a++c --version 2>&1 || echo 'Version info not available')"
log "----------------\n"

# Run the test file
TEST_OUTPUT=$(../build/bin/a++c comprehensive_test.a++ 2>&1)
EXIT_CODE=$?

# Function to check if output contains expected string
check_output() {
    local expected="$1"
    if echo "$TEST_OUTPUT" | grep -q "$expected"; then
        log "${GREEN}✓${NC} Found expected output: $expected"
        return 0
    else
        log "${RED}✗${NC} Missing expected output: $expected"
        return 1
    fi
}

# Test cases - add expected outputs here
EXPECTED_OUTPUTS=(
    "=== Basic Print Tests ==="
    "42"
    "3.14"
    "Hello, World!"
    "yup"
    "=== Variable Tests ==="
    "10"
    "20.5"
    "Testing"
    "yup"
    "=== Arithmetic Tests ==="
    "20"    # 15 + 5
    "10"    # 15 - 5
    "75"    # 15 * 5
    "3"     # 15 / 5
    "0"     # 15 % 5
    "759375"  # 15 ** 5
    "=== Order of Operations Tests ==="
    "14"    # 2 + 3 * 4
    "7"     # 10 - 6 / 2
    "10"    # 15 / 3 * 2
    "14"    # 20 - 4 - 2
    "26"    # 2 * 3 + 4 * 5
    "11"    # 10 + 20 / 5 - 3
    "19"    # 8 * 4 / 2 + 3
    "20"    # (2 + 3) * 4
    "5"     # 10 / (5 - 3)
    "5"     # (15 + 5) / (3 + 1)
    "=== Loop Tests ==="
    "0"
    "1"
    "2"
    "3"
    "4"
    "=== Conditional Tests ==="
    "Condition is yup"
    "10 is greater than 5"
    "=== Comparison Tests ==="
    "yup"   # 5 < 10
    "nope"  # 5 > 10
    "yup"   # 5 == 5
    "yup"   # 5 != 10
    "yup"   # 5 >= 5
    "nope"  # 5 <= 4
    "=== Logical Operator Tests ==="
    "yup"   # yup && yup
    "nope"  # yup && nope
    "yup"   # yup || nope
    "nope"  # !yup
)

# Run the checks
FAILED=0
TOTAL=0
PASSED=0

log "\nRunning Test Cases:"
log "----------------"

for expected in "${EXPECTED_OUTPUTS[@]}"; do
    ((TOTAL++))
    if check_output "$expected"; then
        ((PASSED++))
    else
        FAILED=1
    fi
done

# Print full output for reference
log "\nFull test output:"
log "----------------"
log "$TEST_OUTPUT"
log "----------------"

# Test Summary
log "\nTest Summary:"
log "----------------"
log "Total Tests: $TOTAL"
log "Passed: $PASSED"
log "Failed: $((TOTAL-PASSED))"
log "Success Rate: $(( (PASSED * 100) / TOTAL ))%"
log "Time Completed: $(date)"

# Final result
if [ $FAILED -eq 0 ] && [ $EXIT_CODE -eq 0 ]; then
    log "\n${GREEN}All tests passed!${NC}"
    log "\nLog file created at: $LOG_FILE"
    exit 0
else
    log "\n${RED}Some tests failed!${NC}"
    log "\nLog file created at: $LOG_FILE"
    exit 1
fi
