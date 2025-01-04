#!/bin/bash

# A++ Version Management Script (Bash)
# 
# This script manages version numbers for the A++ programming language.
# It handles version bumping, setting specific versions, and Git tagging.
# 
# Key features:
# - Semantic versioning support (major.minor.patch)
# - Automatic version file updates
# - Git integration for version tagging
# - Version history management
# - Easy version switching
#
# Original Author: Paul Kabulu
# Created: December 2024
# 
# Edited by:
#
# File: scripts/version.sh

# Get the project root directory by following the symlink
if [[ -L "$0" ]]; then
    SCRIPT_PATH=$(readlink "$0")
    SCRIPT_DIR=$(dirname "$SCRIPT_PATH")
else
    SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
fi

PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# If script is called via symlink (e.g., from /usr/local/bin)
if [[ "$PROJECT_ROOT" == "/usr/local" ]]; then
    # Get the actual project root from the real script location
    REAL_SCRIPT=$(readlink -f "$0")
    PROJECT_ROOT=$(dirname $(dirname "$REAL_SCRIPT"))
fi

VERSION_FILE="$PROJECT_ROOT/src/common/version.h"

function update_version() {
    local major=$1
    local minor=$2
    local patch=$3
    
    # Update version.h
    cat > "$VERSION_FILE" << EOF
#ifndef VERSION_H
#define VERSION_H

#define A_PLUS_PLUS_VERSION_MAJOR $major
#define A_PLUS_PLUS_VERSION_MINOR $minor
#define A_PLUS_PLUS_VERSION_PATCH $patch
#define A_PLUS_PLUS_VERSION_STRING "$major.$minor.$patch"

#endif // VERSION_H
EOF
    
    echo "Updated version.h to $major.$minor.$patch"
    
    # Create Git tag
    git add "$VERSION_FILE"
    git commit -m "bump: version $major.$minor.$patch"
    git tag -a "v$major.$minor.$patch" -m "Version $major.$minor.$patch"
    
    echo "Created Git tag v$major.$minor.$patch"
}

function show_usage() {
    echo "A++ Version Management"
    echo
    echo "Usage:"
    echo "  ./version.sh bump major|minor|patch  # Increment version number"
    echo "  ./version.sh set X.Y.Z              # Set specific version"
    echo "  ./version.sh list                   # List all versions"
    echo "  ./version.sh current                # Show current version"
    echo "  ./version.sh checkout VERSION       # Switch to specific version"
}

function get_current_version() {
    local major=$(grep "VERSION_MAJOR" "$VERSION_FILE" | cut -d' ' -f3)
    local minor=$(grep "VERSION_MINOR" "$VERSION_FILE" | cut -d' ' -f3)
    local patch=$(grep "VERSION_PATCH" "$VERSION_FILE" | cut -d' ' -f3)
    echo "$major.$minor.$patch"
}

case "$1" in
    "bump")
        current_version=$(get_current_version)
        major=$(echo $current_version | cut -d. -f1)
        minor=$(echo $current_version | cut -d. -f2)
        patch=$(echo $current_version | cut -d. -f3)
        
        case "$2" in
            "major")
                major=$((major + 1))
                minor=0
                patch=0
                ;;
            "minor")
                minor=$((minor + 1))
                patch=0
                ;;
            "patch")
                patch=$((patch + 1))
                ;;
            *)
                echo "Error: Invalid bump type. Use major, minor, or patch"
                exit 1
                ;;
        esac
        update_version $major $minor $patch
        ;;
        
    "set")
        if [[ $2 =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
            major=$(echo $2 | cut -d. -f1)
            minor=$(echo $2 | cut -d. -f2)
            patch=$(echo $2 | cut -d. -f3)
            update_version $major $minor $patch
        else
            echo "Error: Version must be in format X.Y.Z"
            exit 1
        fi
        ;;
        
    "list")
        echo "Available versions:"
        git tag -l "v*" --sort=-v:refname
        ;;
        
    "current")
        echo "Current version: $(get_current_version)"
        ;;
        
    "checkout")
        if [ -z "$2" ]; then
            echo "Error: Please specify a version"
            exit 1
        fi
        version=$2
        if [[ ! $version =~ ^v ]]; then
            version="v$version"
        fi
        git checkout $version
        echo "Switched to version $version"
        ;;
        
    *)
        show_usage
        ;;
esac
