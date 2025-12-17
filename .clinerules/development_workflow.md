# Development Workflow

Rules for development process and workflow best practices.

## Workflow Best Practices

- **Long-Running Commands**: Use `nohup` for commands that cannot be executed quickly or need to run in the background, ensuring they continue after terminal disconnection.

- **Incremental Development**: Break large coding tasks into smaller components. Complete and test each component individually before proceeding to the next, ensuring each part is production-ready before moving forward.

- **File Organization**: Maintain a clean and organized file hierarchy. Place files in appropriate directories according to their purpose and function within the project.

- **Resource Management**: Consider system resource constraints when running multiple processes. Run resource-intensive operations sequentially rather than in parallel if system memory or CPU limitations are a concern.

- **Git Integration**: Follow git best practices from `.clinerules/version_control.md`. Commit frequently with clear messages, maintain clean working directory using `git_status_clear.md` workflow, and ensure logical commit grouping.

- **Interactive Program Testing**: When testing programs that require user input, create input files and redirect stdin using `echo "input" | ./program` or `./program < input.txt`. For multi-step interactions, use `expect` scripts or create comprehensive test files in `tests/input/` directory.
