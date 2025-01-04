# A++ Test Runner Script (PowerShell)
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
# Created: January 2025
# 
# Edited by:
#
# File: tests/run_tests.ps1

# Colors for output
$GREEN = [System.ConsoleColor]::Green
$RED = [System.ConsoleColor]::Red
$YELLOW = [System.ConsoleColor]::Yellow

# Create logs directory if it doesn't exist
New-Item -ItemType Directory -Force -Path "logs" | Out-Null

# Set up log file with timestamp
$LOG_FILE = "logs/test_run_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
$FAILED_TESTS = 0
$TOTAL_TESTS = 0

# Function to log message to both console and file
function Write-Log {
    param([string]$Message, [System.ConsoleColor]$Color = [System.ConsoleColor]::White)
    Write-Host $Message -ForegroundColor $Color
    Add-Content -Path $LOG_FILE -Value $Message.Replace("`e[0m", "").Replace("`e[0;32m", "").Replace("`e[0;31m", "").Replace("`e[1;33m", "")
}

# Function to check if output contains a string
function Test-Output {
    param([string]$Output, [string]$Pattern)
    return $Output -match [regex]::Escape($Pattern)
}

# Function to run a single test file
function Run-Test {
    param([string]$TestFile)
    $testName = [System.IO.Path]::GetFileNameWithoutExtension($TestFile)
    $testDir = [System.IO.Path]::GetDirectoryName($TestFile)
    
    Write-Log "`nRunning test: " -NoNewline
    Write-Log $testName -Color $YELLOW
    
    $testOutput = & "../build/bin/a++c.exe" $TestFile 2>&1
    $exitCode = $LASTEXITCODE
    
    # Log the complete output to file
    Write-Log "`nTest output:"
    Write-Log $testOutput
    
    # Process expected outputs
    $expectedFile = Join-Path $testDir "$testName.expected"
    if (Test-Path $expectedFile) {
        Get-Content $expectedFile | ForEach-Object {
            $expected = $_
            $script:TOTAL_TESTS++
            if (Test-Output $testOutput $expected) {
                Write-Log " Found expected output: $expected" -Color $GREEN
            } else {
                Write-Log " Missing expected output: $expected" -Color $RED
                $script:FAILED_TESTS++
                # Store the failure details
                Add-Content -Path "$LOG_FILE.failures" -Value @"
Test: $testName
Expected: $expected
Got: $testOutput
---
"@
            }
        }
    } else {
        Write-Log "Warning: No expected output file found for $TestFile" -Color $RED
    }
    
    # Check for error cases if they exist
    $errorFile = Join-Path $testDir "$testName.errors"
    if (Test-Path $errorFile) {
        Get-Content $errorFile | ForEach-Object {
            $error = $_
            $script:TOTAL_TESTS++
            if (Test-Output $testOutput $error) {
                Write-Log " Found expected error: $error" -Color $GREEN
            } else {
                Write-Log " Missing expected error: $error" -Color $RED
                $script:FAILED_TESTS++
            }
        }
    }
}

# Main test execution
Write-Log "Running A++ Tests..."
Write-Log "Test started at: $(Get-Date)"
Write-Log "`nTest Environment:"
Write-Log "----------------"
Write-Log "OS: $([System.Environment]::OSVersion.Platform)"
Write-Log "Compiler Version: $((& "../build/bin/a++c.exe" --version) 2>&1)"
Write-Log "----------------`n"

# Find and run all test files
Get-ChildItem -Path "." -Filter "*.a++" -Recurse | ForEach-Object {
    Run-Test $_.FullName
}

# Summary
Write-Log "`nTest Summary:"
Write-Log "----------------"
Write-Log "Total Tests: $TOTAL_TESTS"
Write-Log "Failed Tests: $FAILED_TESTS"
Write-Log "Passed Tests: $($TOTAL_TESTS - $FAILED_TESTS)"

if ($FAILED_TESTS -eq 0) {
    Write-Log "`nAll tests passed!" -Color $GREEN
    exit 0
} else {
    Write-Log "`nSome tests failed. See $LOG_FILE.failures for details" -Color $RED
    exit 1
} 