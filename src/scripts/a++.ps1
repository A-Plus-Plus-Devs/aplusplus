# A++ Programming Language Launcher Script (PowerShell)
# 
# This script serves as the Windows PowerShell launcher for the A++ compiler.
# It handles compiler compilation if needed and forwards arguments to the compiler.
# 
# Key features:
# - Automatic compiler building
# - Path resolution
# - Argument forwarding
# - Error handling
#
# Original Author: Paul Kabulu
# Created: January 2024
# 
# Edited by:
#
# File: src/scripts/a++.ps1

# Get the absolute path of the script
$SCRIPT_DIR = Split-Path -Parent $MyInvocation.MyCommand.Path
$PROJECT_ROOT = Split-Path -Parent (Split-Path -Parent $SCRIPT_DIR)
$COMPILER_PATH = Join-Path $PROJECT_ROOT "build\bin\a++c.exe"

# Check if make is needed (if compiler doesn't exist)
if (-not (Test-Path $COMPILER_PATH)) {
    Write-Host "Compiling A++ compiler..."
    Push-Location $PROJECT_ROOT
    make
    Pop-Location
}

# Run the compiler with all arguments passed to this script
& $COMPILER_PATH $args 