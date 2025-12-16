# Git Commit Message Best Practices: A Comprehensive Guide

Writing clear, informative commit messages is one of the most important skills in professional software development. Good commit messages create a valuable historical record that helps teams collaborate effectively, debug issues faster, and understand the evolution of a codebase[^1][^2].

## Why Good Commit Messages Matter

Commit messages serve as **documentation for future developers** (including yourself). They explain not just *what* changed, but *why* it changed. According to industry surveys and developer experiences:

- **Future-proofing**: Clear commit messages save hours of troubleshooting later by providing context that code alone cannot convey[^1]
- **Collaboration**: Teams with consistent commit conventions communicate changes more effectively[^1]
- **Automation**: Structured commit messages enable automated changelog generation, version bumping, and release processes[^3]

## Core Principles: The 50/72 Rule

The **50/72 rule** is a widely adopted standard for commit message formatting:

- **50 characters maximum** for the subject line (title)
- **72 characters per line** for the body text[^2]

These limits come from practical considerations: many Git tools (like `git shortlog`) cut off subject lines longer than 50 characters, while the 72-character body width fits comfortably in standard 80-character terminals with Git's padding[^2].

## Structured Commit Message Format

A well-structured commit message typically contains three parts:

```bash
<Subject> (title line)

<Description> (body)

<Tags and External References> (footer)
```

### 1. Subject Line Guidelines
- **Use imperative mood** (e.g., "Add", "Fix", "Update" not "Added", "Fixed", "Updated")
- **Capitalize the first word** (unless using Conventional Commits lowercase convention)
- **No trailing punctuation**
- **Be specific and concise** within the 50-character limit
- **Focus on the "what" and "why"** not just the "how"[^4]

### 2. Body Content
- **Explain the context and reasoning** behind changes
- **Describe the problem being solved**
- **Note any trade-offs, limitations, or considerations**
- **Reference related issues or tickets**
- **Use multiple paragraphs if needed, separated by blank lines**

### 3. Footer Section
- **Reference issue trackers** (e.g., "Fixes #123", "Closes JIRA-456")
- **Note breaking changes** with `BREAKING CHANGE:`
- **Include metadata** like "Reviewed-by:", "Signed-off-by:", etc.[^3]

## Conventional Commits Specification

The **[Conventional Commits](https://www.conventionalcommits.org/)** specification provides a standardized format that both humans and machines can parse easily[^3]. The basic structure is:

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

### Common Commit Types
| Type | Description | Semantic Version Impact |
|------|-------------|-------------------------|
| `feat` | A new feature | MINOR release |
| `fix` | A bug fix | PATCH release |
| `docs` | Documentation changes | No release impact |
| `style` | Code style changes (formatting, etc.) | No release impact |
| `refactor` | Code restructuring without changing behavior | No release impact |
| `test` | Adding or updating tests | No release impact |
| `chore` | Maintenance tasks, dependency updates | No release impact |
| `build` | Build system or external dependency changes | No release impact |
| `ci` | Continuous integration configuration | No release impact |
| `perf` | Performance improvements | No release impact |

### Scope (Optional)
The scope provides additional context about which part of the codebase was affected:
- `feat(auth): add two-factor authentication`
- `fix(api): resolve null pointer in user endpoint`

### Breaking Changes
Indicate breaking changes in two ways:
1. **Using `!` after type/scope**: `feat(api)!: remove deprecated endpoint`
2. **Footer notation**: Include `BREAKING CHANGE: <description>` in the footer[^3]

## Examples: Good vs. Bad Commit Messages

### Good Examples
```bash
# Conventional Commits format
feat(auth): add support for social login via Google and GitHub

This implements OAuth2 flows for Google and GitHub authentication,
allowing users to sign up/sign in using their existing accounts.

Closes #123
```

```bash
fix: prevent race condition in request handling

Introduce a request ID and reference to latest request to dismiss
incoming responses other than from the latest request.

BREAKING CHANGE: The request API now requires a request ID parameter.

Fixes #456
```

### Bad Examples
```bash
# Vague and unhelpful
fixed bug
update code
oops
try again
```

## Tools for Enforcing Standards

### 1. **Commitizen**
Interactive tool that guides you through creating conventional commit messages[^1].

### 2. **Husky & commitlint**
Git hooks that validate commit messages against your team's conventions[^2].

### 3. **Pull Checklist**
Integrates commit message validation directly into pull requests[^2].

### 4. **GitLens & Git Blame extensions**
Visual Studio Code extensions that display commit history inline, making good messages even more valuable[^1].

## Team Adoption Strategies

1. **Start with basics**: Implement the 50/72 rule and imperative mood
2. **Add Conventional Commits gradually**: Begin with `feat`, `fix`, and `chore` types
3. **Use automation**: Set up commit message validation early
4. **Document your conventions**: Include guidelines in your project's CONTRIBUTING.md
5. **Lead by example**: Maintainers should model good commit practices

## Key Takeaways

1. **Write for your future self** and teammates who will need to understand your changes
2. **Follow the 50/72 rule** for readability across Git tools
3. **Use Conventional Commits** for consistency and automation benefits
4. **Explain the "why" not just the "what"** in the body
5. **Reference related issues** to connect commits with project management
6. **Automate validation** to maintain standards as your team grows

Remember: While these are widely accepted best practices, the most important rule is **consistency within your team**. Choose conventions that work for your workflow and stick to them[^1].

[^1]: [How to Write Better Git Commit Messages – A Step-By-Step Guide](https://www.freecodecamp.org/news/how-to-write-better-git-commit-messages/)
[^2]: [Git Commit Message Conventions: A Complete Guide for Modern Developers](https://www.pullchecklist.com/posts/git-commit-message-conventions-guide-modern-developers)
[^3]: [Conventional Commits Specification](https://www.conventionalcommits.org/en/v1.0.0/)
[^4]: [Best Practices for Git Commit Message](https://www.baeldung.com/ops/git-commit-messages)