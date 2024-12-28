# Installing A++

This guide will walk you through installing A++ on your system.

## System Requirements

- Operating System: macOS, Linux, or Windows
- C Compiler (gcc or clang)
- Make build system
- Git (optional, for building from source)

## Installation Methods

### Method 1: Using Package Manager (Recommended)

#### macOS
```bash
brew tap a-plus-plus/tap
brew install aplusplus
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install aplusplus
```

### Method 2: Building from Source

1. Clone the repository:
```bash
git clone https://github.com/A-Plus-Plus-Devs/aplusplus.git
cd aplusplus
```

2. Build the project:
```bash
make
```

3. Install the binary:
```bash
sudo make install
```

## Verifying Installation

To verify that A++ is installed correctly:

```bash
a++ --version
```

You should see output similar to:
```
A++ Version 1.0.0
```

## IDE Support

A++ works with many popular IDEs and text editors:

### Visual Studio Code
1. Install the A++ extension from the marketplace
2. Open any `.a++` file to activate syntax highlighting

### Vim/Neovim
Add syntax highlighting by installing our vim plugin:
```bash
git clone https://github.com/A-Plus-Plus-Devs/vim-aplusplus ~/.vim/pack/plugins/start/vim-aplusplus
```

### Sublime Text
1. Install Package Control
2. Search for "A++ Syntax" and install

## Troubleshooting

### Common Issues

#### Missing Compiler
If you see an error about missing compiler:
```bash
# macOS
brew install gcc

# Ubuntu/Debian
sudo apt-get install build-essential
```

#### Permission Denied
If you get permission errors during installation:
```bash
# Fix permissions
sudo chown -R $(whoami) /usr/local/bin
```

## Next Steps

- Read the [Getting Started Guide](/guide/getting-started)
- Try your [First Program](/guide/first-program)
- Explore [Language Features](/guide/variables-types)
