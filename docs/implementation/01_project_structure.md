# Project Structure: King of the Amazons

## Overview
This document defines the organized and clear file hierarchy for the "King of the Amazons" project. The structure follows software engineering best practices, separating concerns, and enabling modular development.

## Complete File Hierarchy

```
king-of-the-amazons/
├── CMakeLists.txt                 # Main CMake configuration
├── Makefile                       # Alternative build system
├── README.md                      # Project overview and setup instructions
├── .gitignore                     # Git ignore rules
├── .clinerules/                   # Cline operation rules
│   ├── cline_operations.md
│   ├── development_workflow.md
│   ├── documentation.md
│   ├── memorybank.md
│   ├── project_setup.md
│   └── version_control.md
├── docs/                          # Documentation
│   ├── implementation/            # Implementation plan (this directory)
│   │   ├── 01_project_structure.md
│   │   ├── 02_implementation_phases.md
│   │   ├── 03_technical_specifications.md
│   │   ├── 04_testing_strategy.md
│   │   ├── 05_risk_management.md
│   │   └── 06_success_criteria.md
│   ├── instructions/              # Course instructions
│   │   ├── requirements_cn.txt    # Original Chinese requirements
│   │   └── requirements_en.txt    # English translation
│   ├── api/                       # API documentation (generated)
│   └── reports/                   # Project reports
├── memorybank/                    # Project memory bank
│   ├── projectbrief.md
│   ├── productContext.md
│   ├── systemPatterns.md
│   ├── techContext.md
│   ├── activeContext.md
│   └── progress.md
├── src/                           # Source code
│   ├── core/                      # Core game logic
│   │   ├── Board.cpp/.hpp
│   │   ├── GameState.cpp/.hpp
│   │   ├── Player.cpp/.hpp
│   │   ├── Move.cpp/.hpp
│   │   ├── RulesEngine.cpp/.hpp
│   │   └── GameController.cpp/.hpp
│   ├── ai/                        # AI implementations
│   │   ├── AIStrategy.cpp/.hpp
│   │   ├── MinimaxAI.cpp/.hpp
│   │   ├── MonteCarloAI.cpp/.hpp
│   │   ├── Heuristics.cpp/.hpp
│   │   └── MoveGenerator.cpp/.hpp
│   ├── ui/                        # User interface
│   │   ├── Display.cpp/.hpp
│   │   ├── TextDisplay.cpp/.hpp
│   │   ├── GraphicalDisplay.cpp/.hpp (optional enhancement)
│   │   ├── MenuController.cpp/.hpp
│   │   └── InputHandler.cpp/.hpp
│   ├── utils/                     # Utilities
│   │   ├── Serializer.cpp/.hpp
│   │   ├── Logger.cpp/.hpp
│   │   ├── Config.cpp/.hpp
│   │   └── Timer.cpp/.hpp
│   ├── features/                  # Enhanced features (undo/redo, analysis, etc.)
│   │   ├── UndoManager.cpp/.hpp
│   │   ├── GameAnalyzer.cpp/.hpp
│   │   ├── ReplaySystem.cpp/.hpp
│   │   └── Statistics.cpp/.hpp
│   └── main.cpp                   # Entry point
├── include/                       # Public headers
│   ├── core/
│   ├── ai/
│   ├── ui/
│   ├── utils/
│   └── features/
├── tests/                         # Test suite
│   ├── unit/                      # Unit tests
│   │   ├── BoardTest.cpp
│   │   ├── GameStateTest.cpp
│   │   ├── MoveTest.cpp
│   │   └── AITest.cpp
│   ├── integration/               # Integration tests
│   │   ├── GameFlowTest.cpp
│   │   └── SaveLoadTest.cpp
│   ├── system/                    # System tests
│   │   └── EndToEndTest.cpp
│   └── CMakeLists.txt             # Test configuration
├── data/                          # Game data
│   ├── saves/                     # Save files
│   ├── config/                    # Configuration files
│   └── logs/                      # Log files
├── scripts/                       # Build and utility scripts
│   ├── build.sh
│   ├── test.sh
│   ├── format.sh
│   └── coverage.sh
└── third_party/                   # External dependencies
    ├── json/                      # JSON library (if needed)
    └── googletest/                # Google Test framework
```

## Directory Descriptions

### Root Level
- **CMakeLists.txt**: Main build configuration using CMake
- **Makefile**: Alternative build system for simple compilation
- **README.md**: Project overview, setup instructions, and key information
- **.gitignore**: Git ignore rules for build artifacts and temporary files
- **.clinerules/**: Cline-specific operation rules and guidelines

### Documentation (`docs/`)
- **implementation/**: Comprehensive implementation plan documents
- **instructions/**: Course requirements and specifications
- **api/**: Generated API documentation (Doxygen output)
- **reports/**: Project reports and documentation

### Memory Bank (`memorybank/`)
- **projectbrief.md**: Project overview, requirements, and goals
- **productContext.md**: User needs, problem statement, and value proposition
- **systemPatterns.md**: Architecture, design patterns, and component relationships
- **techContext.md**: Technology stack, development setup, and constraints
- **activeContext.md**: Current work focus, recent changes, and next steps
- **progress.md**: Status tracking, milestones, and completion metrics

### Source Code (`src/`)
- **core/**: Core game logic and state management
- **ai/**: Artificial intelligence implementations and algorithms
- **ui/**: User interface components and display systems
- **utils/**: Utility functions and helper classes
- **features/**: Enhanced features (undo/redo, game analysis, replay system, statistics)
- **main.cpp**: Application entry point

### Public Headers (`include/`)
- Organized mirror of `src/` structure for public API headers
- Separation of interface and implementation

### Testing (`tests/`)
- **unit/**: Unit tests for individual components
- **integration/**: Tests for component interactions
- **system/**: End-to-end system tests
- **CMakeLists.txt**: Test configuration and setup

### Data (`data/`)
- **saves/**: Game save files
- **config/**: Configuration files
- **logs/**: Application log files

### Scripts (`scripts/`)
- Build automation and utility scripts
- Code formatting and quality checks
- Test coverage analysis

### Third Party (`third_party/`)
- External dependencies and libraries
- Managed via git submodules or CMake FetchContent

## Build System Organization

### CMake Configuration
```
CMakeLists.txt (root)
├── src/CMakeLists.txt
├── include/CMakeLists.txt
├── tests/CMakeLists.txt
└── third_party/CMakeLists.txt
```

### Build Types
- **Debug**: Development builds with debugging symbols
- **Release**: Optimized production builds
- **RelWithDebInfo**: Release builds with debugging information
- **MinSizeRel**: Minimum size release builds

## Development Workflow

### Code Organization Principles
1. **Separation of Concerns**: Clear division between game logic, AI, UI, and utilities
2. **Modular Design**: Independent components with minimal dependencies
3. **Header/Source Separation**: Public interfaces in `include/`, implementations in `src/`
4. **Test-Driven Development**: Tests alongside implementation code

### File Naming Conventions
- **Classes**: PascalCase (e.g., `GameState`, `Board`)
- **Files**: Match class names (e.g., `GameState.cpp`, `GameState.hpp`)
- **Functions**: camelCase (e.g., `validateMove`, `calculateScore`)
- **Variables**: snake_case (e.g., `player_score`, `board_size`)
- **Constants**: UPPER_SNAKE_CASE (e.g., `BOARD_SIZE`, `MAX_PLAYERS`)

### Version Control Strategy
- **Main Branch**: Stable, production-ready code
- **Feature Branches**: Individual feature development
- **Release Tags**: Version milestones and releases
- **Commit Messages**: Conventional commits with clear descriptions

## Implementation Guidelines

### Code Quality Standards
1. **C++ Standards**: C++17/20 features where appropriate
2. **Memory Management**: Smart pointers, RAII principles
3. **Error Handling**: Exception safety, comprehensive error reporting
4. **Documentation**: Doxygen comments for public APIs

### Performance Considerations
1. **Move Generation**: Efficient algorithms for legal move calculation
2. **AI Search**: Optimized search algorithms with pruning
3. **Memory Usage**: Efficient data structures for game state
4. **I/O Operations**: Asynchronous file operations for save/load

### Platform Compatibility
1. **Operating Systems**: Linux, macOS, Windows
2. **Compilers**: GCC, Clang, MSVC
3. **Dependencies**: Minimal external dependencies

## Maintenance and Evolution

### Future Extensions
1. **Graphical Interface**: SDL2 or SFML integration (enhancement)
2. **Network Play**: Multiplayer over network
3. **Advanced AI**: Machine learning approaches
4. **Mobile Port**: iOS/Android versions

### Documentation Updates
- Update implementation documents as architecture evolves
- Maintain memory bank as single source of truth
- Generate API documentation with code changes

### Code Review Process
- Peer review for major changes
- Automated testing and quality checks
- Performance benchmarking for critical components

This project structure provides a clear, organized foundation for implementing the Game of Amazons while maintaining flexibility for future enhancements and ensuring maintainability throughout the development process.
