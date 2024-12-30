# Version Management

A++ uses semantic versioning (SemVer) with Git tags for version control. This guide explains how versioning works and how to manage versions of A++.

## Understanding Version Numbers

A++ follows the `MAJOR.MINOR.PATCH` versioning scheme:

- **MAJOR** version: Incremented for incompatible API changes
- **MINOR** version: Incremented for new features in a backward-compatible manner
- **PATCH** version: Incremented for backward-compatible bug fixes

For example, version `1.2.3`:
- `1` is the major version
- `2` is the minor version
- `3` is the patch version

## Version Management Tool

A++ includes a version management script that handles version updates and Git tags automatically.

### Basic Commands

1. **View Current Version**
```bash
a++version current
```

2. **List All Available Versions**
```bash
a++version list
```

3. **Increment Version Numbers**
```bash
# Bump major version (e.g., 1.2.3 -> 2.0.0)
a++version bump major

# Bump minor version (e.g., 1.2.3 -> 1.3.0)
a++version bump minor

# Bump patch version (e.g., 1.2.3 -> 1.2.4)
a++version bump patch
```

4. **Set Specific Version**
```bash
a++version set 1.2.3
```

5. **Switch to a Different Version**
```bash
a++version checkout 1.2.3
```

## How Version Control Works

### Git Tags vs Branches

- **Git Tags**: Each version creates a Git tag (e.g., `v1.2.3`). Tags are like snapshots of your code at specific points in time.
- **Branches**: You don't need separate branches for versions. Tags serve as reference points to specific commits.

### What Happens When You Update a Version

When you run a version update command:

1. The version numbers in `src/common/version.h` are updated
2. A new Git commit is created with the version change
3. A Git tag is created for that version
4. The changes are saved in your Git history

### Reverting to Previous Versions

When you checkout a version:

1. Git will switch to the commit associated with that version tag
2. Your codebase will reflect exactly how it was at that version
3. You'll be in a "detached HEAD" state

To make changes to an old version:

1. Checkout the version:
```bash
a++version checkout 1.2.3
```

2. Create a new branch if you want to make changes:
```bash
git checkout -b feature-branch-from-1.2.3
```

## Best Practices

1. **When to Increment Versions**:
   - **MAJOR**: Breaking changes that require users to modify their code
   - **MINOR**: New features that don't break existing code
   - **PATCH**: Bug fixes and minor improvements

2. **Version Tags**:
   - Always use the version script to create tags
   - Don't manually create version tags
   - Push tags to remote: `git push --tags`

3. **Working with Old Versions**:
   - Always create a new branch when modifying old versions
   - Consider the impact on users before releasing breaking changes

## Example Workflow

1. Making a bug fix:
```bash
# Fix the bug in your code
git add .
git commit -m "fix: description of the bug fix"

# Increment patch version
a++version bump patch
```

2. Adding a new feature:
```bash
# Implement the new feature
git add .
git commit -m "feat: description of new feature"

# Increment minor version
a++version bump minor
```

3. Making breaking changes:
```bash
# Make breaking changes
git add .
git commit -m "BREAKING CHANGE: description"

# Increment major version
a++version bump major
```

## Troubleshooting

1. **Detached HEAD After Checkout**:
   - This is normal when checking out version tags
   - Create a new branch if you need to make changes

2. **Version Conflicts**:
   - Don't modify version.h manually
   - Always use the version script
   - If conflicts occur, resolve them and run the version script again

3. **Lost Changes**:
   - Always commit your changes before switching versions
   - Create branches for changes to old versions
