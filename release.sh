#!/bin/zsh

source ~/.zshrc

# Function to get the current version from the latest Git tag
get_current_version() {
    git fetch --tags
    latest_tag=$(git describe --tags `git rev-list --tags --max-count=1`)
    if [[ $latest_tag =~ ^v[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        echo $latest_tag
    else
        echo "v0.0.0"
    fi
}

# Function to increment the version number
increment_version() {
    local version=$1
    local part=$2
    local delimiter=.

    local major=$(echo $version | cut -d$delimiter -f1 | cut -c2-)
    local minor=$(echo $version | cut -d$delimiter -f2)
    local patch=$(echo $version | cut -d$delimiter -f3)

    case $part in
        major)
            major=$((major + 1))
            minor=0
            patch=0
            ;;
        minor)
            minor=$((minor + 1))
            patch=0
            ;;
        patch)
            patch=$((patch + 1))
            ;;
        *)
            echo "Unknown part: $part"
            exit 1
            ;;
    esac

    echo "v$major.$minor.$patch"
}

# Function to determine the version bump type
get_bump_type() {
    local message=$1

    if [[ $message == *"breaking:"* || $message == *"major:"* ]]; then
        echo "major"
    elif [[ $message == *"minor:"*  || $message == *"feat:"* ]]; then
        echo "minor"
    elif [[ $message == *"hotfix:"* || $message == *"patch:"* ]]; then
        echo "patch"
    else
        echo "patch"
    fi
}

# Parse arguments
dry_run=false
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --dry-run) dry_run=true ;;
        *) echo "Unknown parameter: $1" ;;
    esac
    shift
done

# Main script
current_version=$(get_current_version)
echo "Current version: $current_version"

latest_commit_message=$(git log -1 --pretty=%B)
echo "Latest commit message: $latest_commit_message"

bump_type=$(get_bump_type "$latest_commit_message")
echo "Version bump type: $bump_type"

new_version=$(increment_version $current_version $bump_type)
echo "New version: $new_version"

# Generate release notes
release_notes="Release notes for $new_version\n\n$latest_commit_message"

if [ "$dry_run" = true ]; then
    echo "Dry run mode: No changes will be made."
    echo "New version tag: $new_version"
    echo -e "Commit message: chore(release): $new_version [skip ci]\n"
    echo -e "Release notes:\n$release_notes"
else
    # Switch to main branch
    git checkout main

    # Create a new git tag
    git tag $new_version

    # Commit release notes
    echo -e $release_notes > RELEASE_NOTES.md
    git add RELEASE_NOTES.md
    git commit -m "chore(release): $new_version [skip ci]"
    git push
    git push --tags

    echo "New version $new_version created and pushed."
fi
