#!/usr/bin/env zsh

# Get the absolute path of the script
if [[ -L "$0" ]]; then
    SCRIPT_PATH=$(readlink "$0")
    SCRIPT_DIR=${SCRIPT_PATH:h}
else
    SCRIPT_DIR=${0:a:h}
fi

PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
COMPILER_PATH="$PROJECT_ROOT/build/bin/a++c"

# Check if make is needed (if compiler doesn't exist or isn't executable)
needs_compile=false
if [[ ! -x "$COMPILER_PATH" ]]; then
    needs_compile=true
fi

if [[ "$needs_compile" = true ]]; then
    print "Compiling A++ compiler..."
    (cd "$PROJECT_ROOT" && make)
fi

# Run the compiler with all arguments passed to this script
"$COMPILER_PATH" "$@"