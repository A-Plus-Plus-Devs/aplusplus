# A++ Syntax Highlighter Installation Script
# 
# Packages and installs the A++ VSCode syntax highlighting extension
# 
# Original Author: Paul Kabulu
# Created: Jan 2025
#
# File: install-syntax.ps1

Write-Host "Installing A++ Syntax Highlighter for VSCode..."

# Check if vsce is installed
if (!(Get-Command vsce -ErrorAction SilentlyContinue)) {
    Write-Host "Installing vsce..."
    npm install -g @vscode/vsce
}

# Navigate to syntax directory
Set-Location -Path "syntax"

# Install dependencies
npm install

# Package the extension
vsce package

# Install the extension
code --install-extension (Get-Item "a-plus-plus-*.vsix").Name

Write-Host "A++ Syntax Highlighter installation complete!" 