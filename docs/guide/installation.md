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

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install aplusplus
```

## Running A++ Programs

Now you can run A++ files from any directory:

```bash
# Run a file
a++ your_program.a++

# Run with specific options
a++ -o output_file your_program.a++

# Run multiple files
a++ file1.a++ file2.a++
```

## Troubleshooting

### Common Issues

1. If you get a "Permission denied" error:
```bash
chmod +x /usr/local/bin/a++
```

2. If the compiler isn't found:
```bash
# Rebuild the compiler
make clean && make
```

3. If `/usr/local/bin` doesn't exist:
```bash
sudo mkdir -p /usr/local/bin
```

## Uninstallation

To uninstall A++:

```bash
# Remove the symbolic link
sudo rm /usr/local/bin/a++

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
