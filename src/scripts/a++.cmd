@echo off
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