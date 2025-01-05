#!/bin/bash

# A++ Test Runner Script (Bash)
# 
# This script runs the test suite for the A++ programming language.
# It executes test files and compares outputs against expected results.
# 
# Key features:
# - Automated test execution
# - Expected output validation
# - Error case testing
# - Detailed logging
# - Colorized output
# - Test summary generation
#
# Original Author: Paul Kabulu
# Created: December 2024
# 
# Edited by:
#
# File: tests/run_tests.sh

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Create logs directory if it doesn't exist
mkdir -p logs

# Set up log file with timestamp
LOG_FILE="logs/test_run_$(date +%Y%m%d_%H%M%S).log"
FAILED_TESTS=0
TOTAL_TESTS=0

# Function to log message to both console and file
log() {
    echo -e "$1"
    echo -e "${1//$GREEN/}${1//$RED/}${1//$YELLOW/}${1//$NC/}" >> "$LOG_FILE"
}

# Function to check if output contains a string, handling special characters
check_output() {
    local output="$1"
    local pattern="$2"
    echo "$output" | grep -F -q -- "$pattern"
}

# Function to run a single test file
run_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .a++)
    local test_dir=$(dirname "$test_file")
    local test_failed=0
    
    log "\nRunning test: ${YELLOW}$test_name${NC}"
    TEST_OUTPUT=$(../build/bin/a++c "$test_file" 2>&1)
    EXIT_CODE=$?
    
    # Log the complete output to file
    log "\nTest output:"
    log "$TEST_OUTPUT"
    
    # Process expected outputs
    local expected_file="$test_dir/$test_name.expected"
    if [ -f "$expected_file" ]; then
        while IFS= read -r expected; do
            TOTAL_TESTS=$((TOTAL_TESTS + 1))
            if check_output "$TEST_OUTPUT" "$expected"; then
                log "${GREEN} Found expected output: $expected${NC}"
            else
                log "${RED} Missing expected output: $expected${NC}"
                FAILED_TESTS=$((FAILED_TESTS + 1))
                # Store the failure details
                echo "Test: $test_name" >> "$LOG_FILE.failures"
                echo "Expected: $expected" >> "$LOG_FILE.failures"
                echo "Got: $TEST_OUTPUT" >> "$LOG_FILE.failures"
                echo "---" >> "$LOG_FILE.failures"
            fi
        done < "$expected_file"
    else
        log "${RED}Warning: No expected output file found for $test_file${NC}"
    fi
    
    # Check for error cases if they exist
    local error_file="$test_dir/$test_name.errors"
    if [ -f "$error_file" ]; then
        while IFS= read -r error; do
            TOTAL_TESTS=$((TOTAL_TESTS + 1))
            if check_output "$TEST_OUTPUT" "$error"; then
                log "${GREEN} Found expected error: $error${NC}"
            else
                log "${RED} Missing expected error: $error${NC}"
                FAILED_TESTS=$((FAILED_TESTS + 1))
            fi
        done < "$error_file"
    fi
}

# Main test execution
log "Running A++ Tests..."
log "Test started at: $(date)"
log "\nTest Environment:"
log "----------------"
log "OS: $(uname -s)"
log "Compiler Version: $(../build/bin/a++c --version 2>&1 || echo 'Version info not available')"
log "----------------\n"

# Find and run all test files
while IFS= read -r -d '' test_file; do
    run_test "$test_file"
done < <(find . -name "*.a++" -type f -print0)

# Summary
log "\nTest Summary:"
log "----------------"
log "Total Tests: $TOTAL_TESTS"
log "Failed Tests: $FAILED_TESTS"
log "Passed Tests: $((TOTAL_TESTS - FAILED_TESTS))"

if [ $FAILED_TESTS -eq 0 ]; then
    log "\n${GREEN}All tests passed!${NC}"
    exit 0
else
    log "\n${RED}Some tests failed. See $LOG_FILE.failures for details${NC}"
    exit 1
fi
