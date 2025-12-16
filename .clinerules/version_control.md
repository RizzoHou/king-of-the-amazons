# Version Control

Rules for git and version control practices.

## Git Best Practices

- **Git Commits**: Use git after every major change to the project. Commit frequently with clear, descriptive messages that explain what changed and why.

## Git Commit Message Best Practices

Follow these guidelines for effective commit messages:

### Core Principles
- **50/72 Rule**: Keep subject line ≤50 characters, body lines ≤72 characters
- **Imperative Mood**: Use "Add", "Fix", "Update" (not "Added", "Fixed", "Updated")
- **Explain Why**: Focus on the problem solved, not just the implementation

### Conventional Commits Format
```
<type>[optional scope]: <description>

[optional body]

[optional footer]
```

**Common Types**:
- `feat`: New feature (MINOR version bump)
- `fix`: Bug fix (PATCH version bump)  
- `docs`: Documentation changes
- `refactor`: Code restructuring without behavior change
- `test`: Adding or updating tests
- `chore`: Maintenance tasks, dependency updates
- `style`: Code formatting, whitespace changes
- `build`: Build system or external dependency changes
- `ci`: Continuous integration configuration
- `perf`: Performance improvements

### Examples
**Good**:
```
feat(auth): add social login via Google and GitHub
fix: prevent race condition in request handling
docs: update README with deployment instructions
```

**Bad**:
```
updates
fixed bug
try again
```

### Breaking Changes
Indicate with `!` after type/scope or `BREAKING CHANGE:` in footer:
```
feat(api)!: remove deprecated endpoint
```

### Reference
For comprehensive guidance, see: `docs/guidance/git-commit-message-best-practices.md`
