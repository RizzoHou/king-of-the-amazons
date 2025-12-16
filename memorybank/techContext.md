# Tech Context: King of the Amazons

## Technology Stack

### Core Language
- **C++17/20**: Modern C++ standards for improved features and safety
- **Standard Template Library (STL)**: Extensive use of containers, algorithms, and utilities

### Build System
- **CMake** (Preferred): Cross-platform build configuration
  - Minimum version: 3.16
  - Supports multiple build types (Debug, Release, RelWithDebInfo)
- **Make** (Alternative): Simple Makefile for basic compilation
- **Compiler Requirements**:
  - GCC ≥ 9.0 or Clang ≥ 10.0 (Linux/macOS)
  - MSVC ≥ 2019 (Windows)

### Development Environment
- **Primary IDE**: Visual Studio Code with C++ extensions
- **Alternative IDEs**: CLion, Xcode, Visual Studio
- **Debugging Tools**: GDB, LLDB, Valgrind (memory checking)

### Testing Framework
- **Google Test** (gtest): Unit testing framework
- **Catch2**: Alternative testing framework (if preferred)
- **Test Coverage**: gcov/lcov for code coverage analysis

### Version Control
- **Git**: Distributed version control system
- **Repository Hosting**: GitHub, GitLab, or local repository
- **Branch Strategy**: Feature branches with main/master as stable

## Development Setup

### Prerequisites
1. **C++ Compiler**: Installed and configured
2. **CMake**: Version 3.16 or higher
3. **Git**: For version control
4. **Python 3** (optional): For scripting and automation

### Project Structure
```
king-of-the-amazons/
├── src/                    # Source code
│   ├── core/              # Core game logic
│   ├── ai/                # AI implementations
│   ├── ui/                # User interface
│   ├── utils/             # Utility functions
│   └── features/          # Enhanced features (undo/redo, analysis, statistics, replay)
├── include/               # Header files
├── tests/                 # Test files
├── docs/                  # Documentation
├── memorybank/            # Project memory bank
├── CMakeLists.txt         # CMake configuration
├── Makefile               # Alternative build file
└── README.md              # Project overview
```

### Build Instructions

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### Using Makefile (Simple)
```bash
make all          # Build everything
make test         # Run tests
make clean        # Clean build artifacts
```

### Running the Application
```bash
# After building
./build/amazons   # Or equivalent executable name
```

## Dependencies

### Required Libraries
1. **Standard C++ Library**: No external dependencies for basic functionality
2. **Platform-specific libraries**: For optional graphical interface

### Optional Dependencies
1. **SDL2** or **SFML**: For graphical user interface
2. **Boost**: For additional utilities (if needed)
3. **JSON for Modern C++** (nlohmann/json): For save file serialization

### Dependency Management
- **Manual Installation**: System package manager (apt, brew, vcpkg)
- **CMake FetchContent**: For downloading dependencies during build
- **Git Submodules**: For including external libraries

## Development Workflow

### Code Organization
- **Header Files**: `.hpp` or `.h` extensions in `include/` directory
- **Source Files**: `.cpp` extensions in `src/` directory
- **Test Files**: `.cpp` files in `tests/` directory with `_test` suffix

### Coding Standards
- **Naming Conventions**:
  - Classes: PascalCase (e.g., `GameState`)
  - Functions: camelCase (e.g., `validateMove`)
  - Variables: snake_case (e.g., `player_score`)
  - Constants: UPPER_SNAKE_CASE (e.g., `BOARD_SIZE`)
- **Indentation**: 2 or 4 spaces (consistent throughout project)
- **Comments**: Doxygen-style for public APIs, inline for complex logic

### Testing Strategy
1. **Unit Tests**: Individual component testing
2. **Integration Tests**: Component interaction testing
3. **System Tests**: End-to-end game flow testing
4. **AI Tests**: Algorithm correctness and performance testing

## Platform Compatibility

### Supported Operating Systems
- **Linux**: Ubuntu 20.04+, Fedora, Arch
- **macOS**: 10.15+ (Catalina and later)
- **Windows**: 10+ with Visual Studio or MinGW

### Enhanced Features Requirements
- **Undo/Redo System**: Efficient state management for history tracking
- **Game Analysis**: Position evaluation and territory calculation
- **Statistics Tracking**: Game history and performance metrics
- **Replay System**: Game state capture and navigation

## Performance Constraints

### Computational Limits
- **Move Generation**: Must generate legal moves efficiently for 10x10 board
- **AI Search**: Must complete within reasonable time limits (≤2 seconds per move)
- **Memory Usage**: Efficient board representation to support deep search trees

### Optimization Strategies
1. **Bitboard Representation**: For efficient move generation and state evaluation
2. **Transposition Tables**: For caching search results in AI algorithms
3. **Move Ordering**: Heuristics to improve alpha-beta pruning effectiveness
4. **Parallel Processing**: Multi-threading for AI search (if supported by platform)

## Tool Usage Patterns

### Development Tools
- **Clang-Tidy**: Static analysis and code quality
- **Clang-Format**: Automatic code formatting
- **Valgrind**: Memory leak detection
- **GDB/LLDB**: Debugging and profiling

### Build Automation
- **Continuous Integration**: GitHub Actions or GitLab CI
- **Automated Testing**: Run tests on every commit
- **Code Coverage**: Regular coverage reporting

### Documentation Tools
- **Doxygen**: API documentation generation
- **Markdown**: For README and project documentation
- **PlantUML/Mermaid**: For architecture diagrams
