# Contributing to A++

## Welcome!

Thank you for your interest in contributing to A++! This document provides guidelines and information to help you get started as a contributor.

##  Table of Contents
- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Issue Tracking](#issue-tracking)
- [Documentation Standards](#documentation-standards)
- [Pull Request Process](#pull-request-process)
- [Development Guidelines](#development-guidelines)

## Code of Conduct

We are committed to providing a friendly, safe, and welcoming environment for all contributors. Please be respectful and constructive in your communications and contributions.

## Getting Started

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📊 Issue Tracking

### A++ Issue Tracker Features
- **Issue Creation**: Add detailed issue reports
- **Project Assignment**: Organise into project boards
- **Status Tracking**: Monitor progress
- **Contributor Collaboration**: Streamlined task management

### Creating Issues

1. Navigate to the **Issues** tab
2. Click **New Issue**
3. Provide:
   - Clear, descriptive title
   - Detailed problem/feature description
   - Example code (if applicable)
   - Expected behavior
   - Current behavior (for bugs)

### Example Issue Format
```markdown
# Feature/Bug Title

## Current Issues
- Detailed description of problems
- Examples of incorrect behavior

## Expected Behavior
- What should happen
- Example of correct output

## Implementation Notes
- Affected components
- Suggested fixes
```

### Issue Status Labels
- **Open**: New issue awaiting action
- **In Progress**: Work has started
- **Blocked**: Waiting on dependencies
- **Closed**: Resolved or dismissed

## Documentation Standards

### 1. Feature Documentation
Create a new markdown file in `docs/` for significant features or changes:
```
docs/
├── feature_name.md
├── float_improvements.md
├── for_loop_implementation.md
└── ...
```

Documentation should include:
- Issue/Purpose description
- Implementation details
- Example usage
- Testing information
- Future improvements

### 2. Source File Headers
```c
/*
 * Component Name
 * 
 * Brief description of what this file does.
 * 
 * Key features:
 * - Feature 1
 * - Feature 2
 * 
 * Original Author: [Name]
 * Created: [Date]
 * 
 * Edited by:
 * - [Contributor] - [Date] - [Brief description of changes]
 *
 * File: src/component/file.c
 */
```

### 3. Script Headers
```bash
#!/bin/bash

# Script Name
# 
# Brief description of what this script does.
# 
# Key features:
# - Feature 1
# - Feature 2
#
# Original Author: [Name]
# Created: [Date]
# 
# Edited by:
# - [Contributor] - [Date] - [Brief description of changes]
#
# File: scripts/script.sh
```

##  Pull Request Process

1. Ensure your code follows our documentation standards
2. Update relevant documentation
3. Add tests for new features
4. Update the CONTRIBUTORS.md file if this is your first contribution
5. Reference any related issues in your PR description
6. Wait for review

## Development Guidelines

### Code Style
- Write clear, descriptive variable and function names
- Comment complex logic
- Keep functions focused and concise

### Testing
- Add tests for new features
- Ensure existing tests pass
- Document test cases in feature documentation

### Version Control
- Write clear commit messages
- Keep commits focused and atomic
- Reference issue numbers in commits when applicable

### Adding Yourself as a Contributor
1. Fork the repository
2. Add your information to CONTRIBUTORS.md
3. Submit a pull request

## Questions?

If you have questions about contributing:
1. Check existing issues and documentation
2. Ask in the community discussions

Thank you for contributing to A++! 🫡

---

*This document is maintained by the A++ community. Last updated: January 2025*