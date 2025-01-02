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