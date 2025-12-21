# King of the Amazons

A C++ implementation of the "Game of the Amazons" (also known as "Amazons" or "Queen's Game"), a two-player abstract strategy board game. This project serves as a university assignment for a computational introduction course.

## Project Status

**Current Phase**: Phase 3 Complete - Graphical GUI Implemented  
**Overall Completion**: 75% (Phase 1, Phase 2, and Phase 3 fully implemented and tested, Phase 4 in progress)

### Task Completion Workflow Status (COMPLETED)
1. **✅ Step 1: Memory Bank Review** - COMPLETED (all 6 core files read)
2. **✅ Step 2: Memory Bank Updates** - COMPLETED (activeContext.md and progress.md updated)
3. **✅ Step 3: README.md Update** - COMPLETED
4. **✅ Step 4: Git Status Clear** - COMPLETED (all changes committed, working tree clean)

### What Works
- **Phase 1: Core Foundation COMPLETE** (100%)
  - Complete C++ project structure with CMake build system
  - Core game components (Position, Board, Move, GameState, Player) with full functionality
  - Text-based user interface with console display, input handling, and menu system
  - 30 unit tests across 4 test suites, all passing
  - Functional human vs human gameplay with correct board initialization

- **Phase 2: Complete Game System COMPLETE** (100%)
  - **Enhanced Menu System**: Complete main menu with New Game, Load Game, Save Game, Exit options
  - **Save/Load System**: JSON-based serialization with file I/O operations and error handling
  - **Basic AI**: Greedy algorithm with mobility-based heuristic and center control bonus
  - **Complete Game Flow**: Human vs Human, Human vs AI, and AI vs AI gameplay modes
  - **Integration**: All Phase 2 features working together seamlessly

- **Game Mode Save/Load Feature (Dec 18, 2025)**:
  - **Game mode tracking**: Added GameMode enum (HUMAN_VS_HUMAN, HUMAN_VS_AI, AI_VS_AI)
  - **Save with mode**: Save files now include game mode information in JSON format
  - **Load with mode**: Games load with correct game mode and resume in appropriate mode
  - **Backward compatibility**: Old save files without game_mode field default to HUMAN_VS_HUMAN
  - **Testing**: Comprehensive tests verify all three modes save/load correctly

- **Save Functionality Fix (Dec 18, 2025)**:
  - **Save during gameplay**: Users can type "save" or "s" during gameplay to save current game
  - **Exit behavior**: "exit" command returns to main menu instead of terminating program
  - **Serializer fixes**: Proper board serialization (100-character string) and deserialization
  - **GameState constructor**: Added for restoring saved games
  - **Game loop updates**: All game loops handle save/exit properly
  - **Documentation**: Updated game manual with new commands and FAQ

- **Core Game Features**:
  - 10x10 board with standard starting position (Black top, White bottom)
  - Queen movement validation in 8 directions with path checking
  - Turn-based gameplay with White moving first, undo functionality
  - Console display with coordinates, help command, input validation
  - Error handling and robust validation
- **Save/Load**: Game state persistence with JSON files (now working correctly)
- **AI Opponent**: Basic greedy AI for single-player gameplay
- **Input Format**: Simplified from "()->()->()" to 6 numbers (row col row col row col)

- **Phase 3: Graphical GUI Implementation COMPLETE** (100%):
  - **Pure Graphical Interface**: Created `GraphicalController` class with proper state machine
  - **Mouse Manipulation**: Three-step interaction: select queen → select destination → select arrow
  - **Visual Feedback**: Hover effects, color-coded highlights (yellow/green/blue/red)
  - **Dual Mode Support**: Graphical (default) and text mode (`--text` flag)
  - **Mode Selection Screen**: Graphical menu with Human vs Human, Human vs AI, AI vs AI options
  - **Keyboard Shortcuts**: R (restart), U (undo), ESC (exit)
  - **Build Integration**: Added to CMake configuration, compiles successfully

- **Project Infrastructure**:
  - Build system: `cmake --build . --target amazons` works flawlessly
  - Test execution: `./bin/unit_tests` runs all 30 tests successfully
  - Game execution: `./bin/amazons` provides complete game system with all modes
  - Comprehensive memory bank documentation maintained and updated

- **Project Rules and Documentation**:
  - **Task Preparation Rule**: Added to `.clinerules/project_setup.md` requiring reading of `docs/implementation/`, `memorybank/`, `README.md`, and other critical files before starting new tasks
  - **Documentation Workflows**: Established workflows for memory bank updates, README synchronization, and manual maintenance
  - **Comprehensive Coverage**: Rules cover project setup, development workflow, documentation, version control, and memory bank management

### What's Next (Phase 4: Polish and Finalization)
- **Enhanced Features Polish**: Complete statistics tracking and replay system
- **Testing and Quality Assurance**: Add graphical interface tests and integration tests
- **Documentation Finalization**: Update game manual with graphical interface instructions
- **Critical Deadline**: Second Review Session (Jan 9, 2026)

## Requirements

### Basic Functionality (6 points)
- [x] Menu system (new game, load game, save game, exit) - **Phase 2 Complete**
- [x] Board and piece display (character-based console) - **Phase 1 Complete**
- [x] Complete game flow with ability to stop mid-game - **Phase 1 Complete**
- [x] Save/load functionality for game states - **Phase 2 Complete**

### AI Capability (1 point)
- [x] Implementation of computer opponent using game AI algorithms - **Phase 2 Complete** (greedy algorithm)
- [x] Documentation of AI approach and algorithms - **Phase 2 Complete** (documented in memory bank)

### Enhanced Features (2.5 points)
- [x] User-friendly interface and ease of use - **Phase 2 Complete** (enhanced menu system)
- [x] Aesthetic presentation and interface design - **Phase 3 Complete** (graphical GUI with visual feedback)
- [x] Feature completeness and integration - **Phase 2 Complete** (all basic features integrated)
- [ ] Additional innovative features (undo/redo, game analysis, statistics, replay system)
  - [x] Undo functionality - **Phase 1 Complete** (implemented in GameState)
  - [x] Game analysis - **Phase 3 Complete** (basic status messages in graphical interface)
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
# Graphical mode (default)
./build/amazons

# Text mode (console interface)
./build/amazons --text

# Graphical mode explicitly
./build/amazons --graphical
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

### ✅ Phase 2: Complete Game (Week 2: Dec 24-30, 2025) - **COMPLETE**
- ✅ Enhanced menu system (Load Game, Save Game options)
- ✅ Save/load system development (JSON serialization)
- ✅ Basic AI opponent (greedy algorithm)
- ✅ Complete game flow with win condition detection
- ✅ **Critical Deadline Achieved**: Ready for First Review Session (Dec 30, 2025)

### ✅ Phase 3: Graphical GUI and Features (Week 3: Dec 31, 2025 - Jan 6, 2026) - **COMPLETE**
- ✅ Pure graphical interface with mouse manipulation
- ✅ Three-step interaction: select queen → select destination → select arrow
- ✅ Visual feedback with hover effects and color-coded highlights
- ✅ Dual mode support: graphical (default) and text mode (`--text` flag)
- ✅ Mode selection screen with Human vs Human, Human vs AI, AI vs AI options
- ✅ Keyboard shortcuts: R (restart), U (undo), ESC (exit)
- ✅ **Critical Goal Achieved**: Graphical GUI complete ahead of schedule

### 🟡 Phase 4: Polish and Finalization (Week 4: Jan 7-10, 2026) - **IN PROGRESS**
- Enhanced features polish (statistics tracking, replay system)
- Testing and quality assurance (graphical interface tests, integration tests)
- Documentation finalization (game manual updates with graphical interface)
- User interface refinement and bug fixes
- **Critical Deadlines**: Second Review Session (Jan 9, 2026), Final Review Session and project submission (Jan 10, 2026)

## Technology Stack

- **Language**: C++17 (features used: auto, constexpr, structured bindings)
- **Build System**: CMake (configured for cross-platform builds)
- **Testing Framework**: Google Test (gtest) - 30 unit tests implemented
- **Version Control**: Git with Conventional Commits best practices
- **Development Environment**: Visual Studio Code with C++ extensions
- **Architecture**: MVC pattern successfully implemented
- **Code Quality**: Clean C++ with comprehensive documentation and comments
- **Graphics Library**: SFML 2.5 for graphical interface (window management, rendering, input handling)

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
- **Game Manual**: Comprehensive user guide (`docs/manuals/game_manual.md`) covering rules, interface, modes, troubleshooting
- **Implementation Documentation**: Updated project structure (`docs/implementation/01_project_structure.md`) reflecting actual implementation
- **Requirements**: Detailed requirements available in both Chinese (`docs/instructions/requirements_cn.txt`) and English (`docs/instructions/requirements_en.txt`)
- **Code Documentation**: Comprehensive comments and clear APIs throughout implementation
- **Progress Tracking**: Regular updates in `memorybank/progress.md` and `memorybank/activeContext.md`
- **Documentation Workflow**: Automated update process (`.clinerules/workflows/manual_update.md`) ensures documentation stays synchronized

### Success Criteria
Detailed success metrics and grading requirements documented in `docs/implementation/06_success_criteria.md`. Phase 1 success criteria fully met.

## Contributing

This is an individual university assignment. The code and documentation serve as a learning example for C++ programming and software engineering principles.

## License

Educational use only - part of university coursework.
