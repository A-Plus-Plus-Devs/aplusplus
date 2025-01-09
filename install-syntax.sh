#!/bin/bash

# A++ Syntax Highlighter Installation Script
# 
# Packages and installs the A++ VSCode syntax highlighting extension
# 
# Original Author: Paul
# Created: Jan 2025
#
# File: install-syntax.sh

echo "Installing A++ Syntax Highlighter for VSCode..."

# Check if vsce is installed
if ! command -v vsce &> /dev/null; then
    echo "Installing vsce..."
    npm install -g @vscode/vsce
fi

# Navigate to syntax directory
cd vscode || exit 1

# Install dependencies
npm install

# Package the extension
vsce package

# Install the extension
code --install-extension a-plus-plus-*.vsix

echo "A++ Syntax Highlighter installation complete!" 