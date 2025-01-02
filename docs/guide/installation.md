# Installing A++

This guide will walk you through installing A++ on your system.

## System Requirements

- Operating System: macOS, Linux, or Windows
- C Compiler (gcc or clang)
- Make build system
- Git (optional, for building from source)

## Installation Methods

### Method 1: Building from Source

#### macOS

Before installing A++, ensure you have:
- GCC or Clang compiler
- Make build system
- Git (for downloading the source)

1. Clone the repository:
```bash
git clone https://github.com/A-Plus-Plus-Devs/aplusplus.git
cd aplusplus
```

2. Build the project:
```bash
make
```

This will create the A++ compiler in the `build/bin` directory.

3. Set Up Global Access

To run A++ files and manage versions from anywhere on your Mac, follow these steps:

1. Create symbolic links:
```bash
# Link the A++ compiler
sudo ln -s "$(pwd)/src/scripts/a++" /usr/local/bin/a++

# Link the version manager
sudo ln -s "$(pwd)/scripts/version.sh" /usr/local/bin/a++version

# Make both scripts executable
chmod +x src/scripts/a++
chmod +x scripts/version.sh
```

2. Test the installation:
```bash
# Test compiler
a++ --version

# Test version manager
a++version current
```

#### macOS/Linux
1. Create symbolic links:
```bash
# Link the A++ compiler
sudo ln -s "$(pwd)/src/scripts/a++" /usr/local/bin/a++

# Link the version manager
sudo ln -s "$(pwd)/scripts/version.sh" /usr/local/bin/a++version

# Make both scripts executable
chmod +x src/scripts/a++
chmod +x scripts/version.sh
```

#### Windows (PowerShell)
1. Create symbolic links (run PowerShell as Administrator):
```powershell
# Create a symbolic link in a directory that's in your PATH
New-Item -ItemType SymbolicLink -Path "$env:USERPROFILE\AppData\Local\Microsoft\WindowsApps\a++.ps1" -Target "$(Get-Location)\src\scripts\a++.ps1"

# Optional: Create version manager link
New-Item -ItemType SymbolicLink -Path "$env:USERPROFILE\AppData\Local\Microsoft\WindowsApps\a++version.ps1" -Target "$(Get-Location)\scripts\version.ps1"
```

2. Test the installation:
```bash
# On macOS/Linux
a++ --version
a++version current

# On Windows
a++.ps1 --version
a++version.ps1 current
```
<!-- 
#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install aplusplus
``` -->

## Running A++ Programs

Now you can run A++ files from any directory:

```bash
# Run a file
a++ your_program.a++
```

## Troubleshooting

### Common Issues

1. If you get a "Permission denied" error:
```bash
# On Bash/Zsh
chmod +x /usr/local/bin/a++

# On Windows (PowerShell as Administrator)
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser

# On Windows (CMD)
# No action needed, .cmd files are executable by default
```

2. If the compiler isn't found:
```bash
# Rebuild the compiler
make clean && make
```

3. If `/usr/local/bin` doesn't exist (macOS/Linux):
```bash
sudo mkdir -p /usr/local/bin
```

4. On Windows, if you can't create symbolic links:
- Make sure you're running PowerShell as Administrator
- Enable Developer Mode in Windows Settings > Update & Security > For developers

## Uninstallation

To uninstall A++:

```bash
# On Bash
sudo rm /usr/local/bin/a++
sudo rm /usr/local/bin/a++version

# On Zsh
sudo rm /usr/local/bin/a++
sudo rm /usr/local/bin/a++version

# On Windows (PowerShell as Administrator)
Remove-Item "$env:USERPROFILE\AppData\Local\Microsoft\WindowsApps\a++.ps1"
Remove-Item "$env:USERPROFILE\AppData\Local\Microsoft\WindowsApps\a++version.ps1"

# On Windows (CMD as Administrator)
del "%USERPROFILE%\AppData\Local\Microsoft\WindowsApps\a++.cmd"
del "%USERPROFILE%\AppData\Local\Microsoft\WindowsApps\a++version.cmd"

# Optionally, remove the entire project
rm -rf path/to/aplusplus
```

## IDE Support

A++ works with many popular IDEs and text editors:

### Visual Studio Code
Sytnax higlighting guide coming soon...
<!-- 1. Install the A++ extension from the marketplace
2. Open any `.a++` file to activate syntax highlighting -->

### Vim/Neovim
No syntax highlighting at the moment.
<!-- Add syntax highlighting by installing our vim plugin:
```bash
git clone https://github.com/A-Plus-Plus-Devs/vim-aplusplus ~/.vim/pack/plugins/start/vim-aplusplus
``` -->

### Sublime Text
No syntax highlighting at the moment.
<!-- 1. Install Package Control
2. Search for "A++ Syntax" and install -->

## Next Steps

- Read the [Getting Started Guide](/guide/getting-started)
- Try your [First Program](/guide/first-program)
- Explore [Language Features](/guide/variables-types)
