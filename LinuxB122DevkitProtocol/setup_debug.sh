#!/bin/bash

# Setup script for debugging environment

echo "========================================="
echo "Setting up debugging environment"
echo "========================================="
echo ""

# Check if gdb is installed
if command -v gdb &> /dev/null; then
    echo "✓ GDB is already installed"
    gdb --version | head -n 1
else
    echo "✗ GDB is not installed"
    echo ""
    echo "Installing GDB..."
    sudo apt-get update
    sudo apt-get install -y gdb

    if command -v gdb &> /dev/null; then
        echo "✓ GDB installed successfully"
        gdb --version | head -n 1
    else
        echo "✗ Failed to install GDB"
        exit 1
    fi
fi

echo ""

# Check if VSCode C++ extension is available
if command -v code &> /dev/null; then
    echo "✓ VSCode is installed"

    # Check if C/C++ extension is installed
    if code --list-extensions | grep -q "ms-vscode.cpptools"; then
        echo "✓ C/C++ extension is already installed"
    else
        echo "✗ C/C++ extension is not installed"
        echo ""
        echo "Installing C/C++ extension..."
        code --install-extension ms-vscode.cpptools

        if code --list-extensions | grep -q "ms-vscode.cpptools"; then
            echo "✓ C/C++ extension installed successfully"
        else
            echo "✗ Failed to install C/C++ extension"
            echo "Please install it manually from VSCode marketplace"
        fi
    fi
else
    echo "✗ VSCode is not installed or not in PATH"
    echo "Install VSCode from: https://code.visualstudio.com/"
fi

echo ""
echo "========================================="
echo "Setup complete!"
echo "========================================="
echo ""
echo "You can now:"
echo "  1. Open VSCode: code ."
echo "  2. Open main.c"
echo "  3. Press F5 to start debugging"
echo ""
