# Get the project root directory
$SCRIPT_PATH = $MyInvocation.MyCommand.Path
if (Get-Item $SCRIPT_PATH).LinkType -eq "SymbolicLink") {
    $SCRIPT_PATH = (Get-Item $SCRIPT_PATH).Target
}
$SCRIPT_DIR = Split-Path -Parent $SCRIPT_PATH
$PROJECT_ROOT = Split-Path -Parent $SCRIPT_DIR
$VERSION_FILE = Join-Path $PROJECT_ROOT "src\common\version.h"

function Update-Version {
    param (
        [int]$Major,
        [int]$Minor,
        [int]$Patch
    )
    
    # Update version.h
    $versionContent = @"
#ifndef VERSION_H
#define VERSION_H

#define A_PLUS_PLUS_VERSION_MAJOR $Major
#define A_PLUS_PLUS_VERSION_MINOR $Minor
#define A_PLUS_PLUS_VERSION_PATCH $Patch
#define A_PLUS_PLUS_VERSION_STRING "$Major.$Minor.$Patch"

#endif // VERSION_H
"@
    
    Set-Content -Path $VERSION_FILE -Value $versionContent
    Write-Host "Updated version.h to $Major.$Minor.$Patch"
    
    # Create Git tag
    git add $VERSION_FILE
    git commit -m "bump: version $Major.$Minor.$Patch"
    git tag -a "v$Major.$Minor.$Patch" -m "Version $Major.$Minor.$Patch"
    
    Write-Host "Created Git tag v$Major.$Minor.$Patch"
}

function Show-Usage {
    Write-Host "A++ Version Management"
    Write-Host
    Write-Host "Usage:"
    Write-Host "  .\version.ps1 bump major|minor|patch  # Increment version number"
    Write-Host "  .\version.ps1 set X.Y.Z              # Set specific version"
    Write-Host "  .\version.ps1 list                   # List all versions"
    Write-Host "  .\version.ps1 current                # Show current version"
    Write-Host "  .\version.ps1 checkout VERSION       # Switch to specific version"
}

function Get-CurrentVersion {
    $content = Get-Content $VERSION_FILE
    $major = ($content | Select-String "VERSION_MAJOR").ToString().Split(" ")[-1]
    $minor = ($content | Select-String "VERSION_MINOR").ToString().Split(" ")[-1]
    $patch = ($content | Select-String "VERSION_PATCH").ToString().Split(" ")[-1]
    return "$major.$minor.$patch"
}

$command = $args[0]
switch ($command) {
    "bump" {
        $current = Get-CurrentVersion
        $major, $minor, $patch = $current.Split(".")
        
        switch ($args[1]) {
            "major" {
                $major = [int]$major + 1
                $minor = 0
                $patch = 0
            }
            "minor" {
                $minor = [int]$minor + 1
                $patch = 0
            }
            "patch" {
                $patch = [int]$patch + 1
            }
            default {
                Write-Error "Error: Invalid bump type. Use major, minor, or patch"
                exit 1
            }
        }
        Update-Version $major $minor $patch
    }
    
    "set" {
        if ($args[1] -match "^\d+\.\d+\.\d+$") {
            $major, $minor, $patch = $args[1].Split(".")
            Update-Version $major $minor $patch
        }
        else {
            Write-Error "Error: Version must be in format X.Y.Z"
            exit 1
        }
    }
    
    "list" {
        Write-Host "Available versions:"
        git tag -l "v*" --sort=-v:refname
    }
    
    "current" {
        Write-Host "Current version: $(Get-CurrentVersion)"
    }
    
    "checkout" {
        if (-not $args[1]) {
            Write-Error "Error: Please specify a version"
            exit 1
        }
        $version = $args[1]
        if (-not $version.StartsWith("v")) {
            $version = "v$version"
        }
        git checkout $version
        Write-Host "Switched to version $version"
    }
    
    default {
        Show-Usage
    }
} 