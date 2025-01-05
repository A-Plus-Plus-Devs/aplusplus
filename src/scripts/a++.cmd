@echo off
:: A++ Programming Language Launcher Script (Windows CMD)
:: 
:: This script serves as the Windows command prompt launcher for the A++ compiler.
:: It handles compiler compilation if needed and forwards arguments to the compiler.
:: 
:: Key features:
:: - Automatic compiler building
:: - Path resolution
:: - Argument forwarding
:: - Error handling
::
:: Original Author: Paul Kabulu
:: Created: JAnuary 2025
:: 
:: Edited by:
::
:: File: src/scripts/a++.cmd

setlocal

rem Get the script directory and project root
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%..\.."
set "COMPILER_PATH=%PROJECT_ROOT%\build\bin\a++c.exe"

rem Check if compiler needs to be rebuilt (if doesn't exist)
if not exist "%COMPILER_PATH%" (
    echo Compiling A++ compiler...
    pushd "%PROJECT_ROOT%"
    make
    popd
)

rem Run the compiler with all arguments passed to this script
"%COMPILER_PATH%" %* 