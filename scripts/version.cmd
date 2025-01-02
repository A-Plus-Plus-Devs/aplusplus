@echo off
setlocal EnableDelayedExpansion

rem Get the project root directory
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%\.."
set "VERSION_FILE=%PROJECT_ROOT%\src\common\version.h"

if "%1"=="" goto :usage
if "%1"=="bump" goto :bump
if "%1"=="set" goto :set
if "%1"=="list" goto :list
if "%1"=="current" goto :current
if "%1"=="checkout" goto :checkout
goto :usage

:bump
if "%2"=="" goto :usage
call :get_current_version
for /f "tokens=1,2,3 delims=." %%a in ("!CURRENT_VERSION!") do (
    set MAJOR=%%a
    set MINOR=%%b
    set PATCH=%%c
)

if "%2"=="major" (
    set /a MAJOR+=1
    set MINOR=0
    set PATCH=0
) else if "%2"=="minor" (
    set /a MINOR+=1
    set PATCH=0
) else if "%2"=="patch" (
    set /a PATCH+=1
) else (
    echo Error: Invalid bump type. Use major, minor, or patch
    exit /b 1
)

call :update_version !MAJOR! !MINOR! !PATCH!
goto :eof

:set
if "%2"=="" goto :usage
for /f "tokens=1,2,3 delims=." %%a in ("%2") do (
    call :update_version %%a %%b %%c
)
goto :eof

:list
echo Available versions:
git tag -l "v*" --sort=-v:refname
goto :eof

:current
call :get_current_version
echo Current version: !CURRENT_VERSION!
goto :eof

:checkout
if "%2"=="" (
    echo Error: Please specify a version
    exit /b 1
)
set VERSION=%2
if not "%VERSION:~0,1%"=="v" set VERSION=v%VERSION%
git checkout %VERSION%
echo Switched to version %VERSION%
goto :eof

:update_version
(
echo #ifndef VERSION_H
echo #define VERSION_H
echo.
echo #define A_PLUS_PLUS_VERSION_MAJOR %1
echo #define A_PLUS_PLUS_VERSION_MINOR %2
echo #define A_PLUS_PLUS_VERSION_PATCH %3
echo #define A_PLUS_PLUS_VERSION_STRING "%1.%2.%3"
echo.
echo #endif // VERSION_H
) > "%VERSION_FILE%"

echo Updated version.h to %1.%2.%3

git add "%VERSION_FILE%"
git commit -m "bump: version %1.%2.%3"
git tag -a "v%1.%2.%3" -m "Version %1.%2.%3"

echo Created Git tag v%1.%2.%3
goto :eof

:get_current_version
for /f "tokens=3" %%i in ('findstr "VERSION_MAJOR" "%VERSION_FILE%"') do set MAJOR=%%i
for /f "tokens=3" %%i in ('findstr "VERSION_MINOR" "%VERSION_FILE%"') do set MINOR=%%i
for /f "tokens=3" %%i in ('findstr "VERSION_PATCH" "%VERSION_FILE%"') do set PATCH=%%i
set CURRENT_VERSION=%MAJOR%.%MINOR%.%PATCH%
goto :eof

:usage
echo A++ Version Management
echo.
echo Usage:
echo   version.cmd bump major^|minor^|patch  # Increment version number
echo   version.cmd set X.Y.Z              # Set specific version
echo   version.cmd list                   # List all versions
echo   version.cmd current                # Show current version
echo   version.cmd checkout VERSION       # Switch to specific version
goto :eof 