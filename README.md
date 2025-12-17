# King of the Amazons

A C++ implementation of the "Game of the Amazons" (also known as "Amazons" or "Queen's Game"), a two-player abstract strategy board game. This project serves as a university assignment for a computational introduction course.

## Project Status

**Current Phase**: Phase 1 Complete - Core Foundation Implemented  
**Overall Completion**: 25% (Phase 1 fully implemented and tested, ready for Phase 2)

### What Works
- **Phase 1: Core Foundation COMPLETE** (100%)
  - Complete C++ project structure with CMake build system
  - Core game components (Position, Board, Move, GameState, Player) with full functionality
  - Text-based user interface with console display, input handling, and menu system
  - 30 unit tests across 4 test suites, all passing
  - Functional human vs human gameplay with correct board initialization

- **Core Game Features**:
  - 10x10 board with standard starting position (Black top, White bottom)
  - Queen movement validation in 8 directions with path checking
  - Turn-based gameplay with White moving first, undo functionality
  - Console display with coordinates, help command, input validation
  - Error handling and robust validation

- **Project Infrastructure**:
  - Build system: `cmake --build . --target amazons` works flawlessly
  - Test execution: `./bin/unit_tests` runs all 30 tests successfully
  - Game execution: `./bin/amazons` provides functional human vs human gameplay
  - Comprehensive memory bank documentation maintained

### What's Next (Phase 2: Complete Game System)
- **Enhanced Menu System**: Add Load Game, Save Game options
- **Save/Load System**: Game state serialization (JSON format) with file I/O
- **Basic AI Opponent**: Greedy algorithm implementation
- **Complete Game Flow**: Enhanced turn management and win condition detection
- **Critical Deadline**: First Review Session (Dec 30, 2025)

## Requirements

### Basic Functionality (6 points)
- [x] Menu system (new game, exit) - **Phase 1 Complete**
- [x] Board and piece display (character-based console) - **Phase 1 Complete**
- [x] Complete game flow with ability to stop mid-game - **Phase 1 Complete**
- [ ] Save/load functionality for game states - **Phase 2 Target**

### AI Capability (1 point)
- [ ] Implementation of computer opponent using game AI algorithms - **Phase 2 Target**
- [ ] Documentation of AI approach and algorithms - **Phase 2 Target**

### Enhanced Features (2.5 points)
- [x] User-friendly interface and ease of use - **Phase 1 Complete** (console interface)
- [x] Aesthetic presentation and interface design - **Phase 1 Complete** (formatted board display)
- [ ] Feature completeness and integration - **Phase 2-4 Target**
- [ ] Additional innovative features (undo/redo, game analysis, statistics, replay system)
  - [x] Undo functionality - **Phase 1 Complete** (implemented in GameState)
  - [ ] Game analysis - **Phase 3 Target**
  - [ ] Statistics tracking - **Phase 4 Target**
  - [ ] Replay system - **Phase 4 Target**

## Project Structure

Complete file hierarchy and organization guidelines are documented in `docs/implementation/01_project_structure.md`. The structure follows software engineering best practices with clear separation of concerns:

```
king-of-the-amazons/
├── CMakeLists.txt                 # Main CMake configuration
├── Makefile                       # Alternative build system
├── README.md                      # Project overview and setup instructions
├── .gitignore                     # Git ignore rules
├── .clinerules/                   # Cline operation rules
├── docs/                          # Documentation
│   ├── implementation/            # Implementation plan (6 detailed documents)
│   ├── instructions/              # Course requirements
│   ├── api/                       # API documentation (generated)
│   └── reports/                   # Project reports
├── memorybank/                    # Project memory bank (6 core files)
├── src/                           # Source code (core/, ai/, ui/, utils/, features/)
├── include/                       # Public headers
├── tests/                         # Test suite (unit/, integration/, system/)
├── data/                          # Game data (saves/, config/, logs/)
├── scripts/                       # Build and utility scripts
└── third_party/                   # External dependencies
```

For detailed directory descriptions and development guidelines, refer to the implementation plan documents.

## Development Setup

### Prerequisites
- C++ compiler (GCC ≥ 9.0, Clang ≥ 10.0, or MSVC ≥ 2019)
- CMake ≥ 3.16
- Git

### Build Instructions

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### Using Makefile (Alternative)
```bash
make all          # Build everything
make test         # Run tests
make clean        # Clean build artifacts
```

### Running the Application
```bash
./build/amazons   # Or equivalent executable name
```

## Game Rules (Amazons)

### Board
- 10×10 grid
- Each player starts with 4 Amazons in standard positions

### Movement
1. **Amazon Move**: Move one Amazon like a queen in chess (any distance in 8 directions)
2. **Arrow Shot**: From the new position, shoot an arrow like a queen (any distance, 8 directions)
3. **Restrictions**: Cannot move through or land on occupied squares (other Amazons, arrows, or obstacles)

### Game End
- Game ends when a player cannot make a legal move
- Winner is the player with the most territory (reachable squares)

## Key Deadlines

### Enhanced Features Development
- **Undo/Redo System**: Week 3 (Dec 31 - Jan 6)
- **Game Analysis Features**: Week 3-4 (Dec 31 - Jan 10)
- **Statistics Tracking**: Week 4 (Jan 7-10)
- **Replay System**: Week 4 (Jan 7-10)

### Review Sessions
- **First Review Session** (pre-grading): Dec 30, 2025
- **Second Review Session** (system grading): Jan 9, 2026
- **Third Review Session** (final grading): Jan 10, 2026

## Development Timeline

Detailed timeline and milestones are documented in `docs/implementation/02_implementation_phases.md`:

### ✅ Phase 1: Core Foundation (Week 1: Dec 17-23, 2025) - **COMPLETE**
- ✅ Project setup and CMake configuration
- ✅ Basic game components implementation (Position, Board, Move, GameState, Player)
- ✅ Text interface foundation (TextDisplay, InputHandler, MenuController)
- ✅ **Critical Goal Achieved**: Playable human vs human prototype

### 🟡 Phase 2: Complete Game (Week 2: Dec 24-30, 2025) - **IN PROGRESS**
- Enhanced menu system (Load Game, Save Game options)
- Save/load system development (JSON serialization)
- Basic AI opponent (greedy algorithm)
- Complete game flow with win condition detection
- **Critical Deadline**: First Review Session (Dec 30, 2025) - **13 DAYS REMAINING**

### ⚪ Phase 3: Advanced AI and Features (Week 3: Dec 31, 2025 - Jan 6, 2026) - **PENDING**
- Advanced AI algorithms (minimax with alpha-beta pruning)
- Enhanced features implementation (game analysis, improved undo/redo)
- Performance optimization
- **Critical Goal**: Enhanced features foundation complete

### ⚪ Phase 4: Polish and Finalization (Week 4: Jan 7-10, 2026) - **PENDING**
- Comprehensive testing and quality assurance
- Documentation completion
- Enhanced features polish (statistics, replay system)
- User interface refinement
- **Critical Deadlines**: Second Review Session (Jan 9, 2026), Final Review Session and project submission (Jan 10, 2026)

## Technology Stack

- **Language**: C++17 (features used: auto, constexpr, structured bindings)
- **Build System**: CMake (configured for cross-platform builds)
- **Testing Framework**: Google Test (gtest) - 30 unit tests implemented
- **Version Control**: Git with Conventional Commits best practices
- **Development Environment**: Visual Studio Code with C++ extensions
- **Architecture**: MVC pattern successfully implemented
- **Code Quality**: Clean C++ with comprehensive documentation and comments

## Documentation

### Implementation Plan
Comprehensive planning documents in `docs/implementation/`:
- `01_project_structure.md`: File hierarchy and organization - **Implemented**
- `02_implementation_phases.md`: Timeline and milestones - **Phase 1 Complete**
- `03_technical_specifications.md`: Technical details and APIs - **Phase 1 Implemented**
- `04_testing_strategy.md`: Testing approach and quality assurance - **30 tests passing**
- `05_risk_management.md`: Risk assessment and mitigation
- `06_success_criteria.md`: Success metrics and grading requirements

### Project Documentation
- **Memory Bank**: Complete project documentation in `memorybank/` (6 core files, regularly updated)
- **Requirements**: Detailed requirements available in both Chinese (`docs/instructions/requirements_cn.txt`) and English (`docs/instructions/requirements_en.txt`)
- **Code Documentation**: Comprehensive comments and clear APIs throughout implementation
- **Progress Tracking**: Regular updates in `memorybank/progress.md` and `memorybank/activeContext.md`

### Success Criteria
Detailed success metrics and grading requirements documented in `docs/implementation/06_success_criteria.md`. Phase 1 success criteria fully met.

## Contributing

This is an individual university assignment. The code and documentation serve as a learning example for C++ programming and software engineering principles.

## License

Educational use only - part of university coursework.
