@echo off
setlocal EnableDelayedExpansion

rem Colors for output
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "NC=[0m"

rem Create logs directory if it doesn't exist
if not exist "logs" mkdir logs

rem Set up log file with timestamp
for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /value') do set datetime=%%I
set "LOG_FILE=logs\test_run_%datetime:~0,8%_%datetime:~8,6%.log"
set "FAILED_TESTS=0"
set "TOTAL_TESTS=0"

rem Function to log message to both console and file
:log
echo %GREEN%%~1%NC%
echo %~1 >> "%LOG_FILE%"
exit /b

rem Function to check if output contains a string
:check_output
set "output=%~1"
set "pattern=%~2"
echo !output! | findstr /C:"%pattern%" >nul
exit /b !errorlevel!

rem Function to run a single test file
:run_test
set "test_file=%~1"
for %%F in ("%test_file%") do (
    set "test_name=%%~nF"
    set "test_dir=%%~dpF"
)

call :log "Running test: %YELLOW%!test_name!%NC%"

..\build\bin\a++c.exe "%test_file%" > test_output.tmp 2>&1
set "EXIT_CODE=!errorlevel!"

rem Log the complete output to file
call :log "Test output:"
type test_output.tmp >> "%LOG_FILE%"

rem Process expected outputs
if exist "!test_dir!!test_name!.expected" (
    for /f "usebackq delims=" %%A in ("!test_dir!!test_name!.expected") do (
        set /a TOTAL_TESTS+=1
        set "expected=%%A"
        findstr /C:"!expected!" test_output.tmp >nul
        if !errorlevel! equ 0 (
            call :log "%GREEN% Found expected output: !expected!%NC%"
        ) else (
            call :log "%RED% Missing expected output: !expected!%NC%"
            set /a FAILED_TESTS+=1
            echo Test: !test_name! >> "%LOG_FILE%.failures"
            echo Expected: !expected! >> "%LOG_FILE%.failures"
            type test_output.tmp >> "%LOG_FILE%.failures"
            echo --- >> "%LOG_FILE%.failures"
        )
    )
) else (
    call :log "%RED%Warning: No expected output file found for %test_file%%NC%"
)

rem Check for error cases if they exist
if exist "!test_dir!!test_name!.errors" (
    for /f "usebackq delims=" %%A in ("!test_dir!!test_name!.errors") do (
        set /a TOTAL_TESTS+=1
        set "error=%%A"
        findstr /C:"!error!" test_output.tmp >nul
        if !errorlevel! equ 0 (
            call :log "%GREEN% Found expected error: !error!%NC%"
        ) else (
            call :log "%RED% Missing expected error: !error!%NC%"
            set /a FAILED_TESTS+=1
        )
    )
)

del test_output.tmp
exit /b

rem Main test execution
call :log "Running A++ Tests..."
call :log "Test started at: %date% %time%"
call :log ""
call :log "Test Environment:"
call :log "----------------"
call :log "OS: Windows"
..\build\bin\a++c.exe --version > version.tmp 2>&1
set /p compiler_version=<version.tmp
call :log "Compiler Version: !compiler_version!"
del version.tmp
call :log "----------------"
call :log ""

rem Find and run all test files
for /r %%F in (*.a++) do (
    call :run_test "%%F"
)

rem Summary
call :log ""
call :log "Test Summary:"
call :log "----------------"
call :log "Total Tests: %TOTAL_TESTS%"
call :log "Failed Tests: %FAILED_TESTS%"
set /a PASSED_TESTS=%TOTAL_TESTS%-%FAILED_TESTS%
call :log "Passed Tests: !PASSED_TESTS!"

if %FAILED_TESTS% equ 0 (
    call :log ""
    call :log "%GREEN%All tests passed!%NC%"
    exit /b 0
) else (
    call :log ""
    call :log "%RED%Some tests failed. See %LOG_FILE%.failures for details%NC%"
    exit /b 1
) 