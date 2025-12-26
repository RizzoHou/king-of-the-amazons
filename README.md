# King of the Amazons

A C++ implementation of the "Game of the Amazons" (also known as "Amazons" or "Queen's Game"), a two-player abstract strategy board game. This project serves as a university assignment for a computational introduction course.

## Project Status

**Current Phase**: Phase 3 Complete - Graphical GUI Implemented  
**Overall Completion**: 85% (Phase 1, Phase 2, and Phase 3 fully implemented and tested, Phase 4 in progress)

**Last Updated**: December 26, 2025

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
  - 8x8 board with standard starting position (Black top, White bottom)
  - Queen movement validation in 8 directions with path checking
  - Turn-based gameplay with White moving first, undo functionality
  - Console display with coordinates, help command, input validation
  - Error handling and robust validation
- **Save/Load**: Game state persistence with JSON files (now working correctly)
- **AI Opponent**: Basic greedy AI for single-player gameplay
- **Advanced AI Integration**: BotzoneAI class interfaces with external bot003 executable from amazing-amazons project
- **AI Algorithm Migration**: Successfully migrated advanced AI algorithm from amazing-amazons project (Dec 25, 2025)
- **Input Format**: Simplified from "()->()->()" to 6 numbers (row col row col row col)

- **Phase 3: Graphical GUI Implementation COMPLETE** (100%):
  - **Pure Graphical Interface**: Created `GraphicalController` class with proper state machine
  - **Mouse Manipulation**: Three-step interaction: select queen → select destination → select arrow
  - **Visual Feedback**: Hover effects, color-coded highlights (yellow/green/blue/red)
  - **Dual Mode Support**: Graphical (default) and text mode (`--text` flag)
  - **Mode Selection Screen**: Graphical menu with Human vs Human, Human vs AI options (AI vs AI removed from graphical interface)
  - **Keyboard Shortcuts**: R (restart), U (undo), ESC (return to menu)
  - **Build Integration**: Added to CMake configuration, compiles successfully

- **AI vs AI Mode Removal from Graphical Interface (Dec 23, 2025)**:
  - **Problem Resolution**: Fixed issue documented in `docs/problems/existing_problems_002.md` where AI vs AI mode was "unusable and unnecessary"
  - **Code Changes**: Removed AI vs AI button and all related logic from `GraphicalController.cpp`
  - **Interface Simplification**: Graphical mode selection now only shows: Continue Previous Game (when available), Human vs Human, and Human vs AI
  - **Documentation Updates**: Updated problem documentation to mark issue as RESOLVED
  - **Testing**: Successfully compiled and verified program runs without errors
  - **Rationale**: User explicitly stated "I don't need such a mode" and it was causing game to get stuck at first turn

- **GUI Improvements (Dec 21, 2025)**:
  - **ESC Key Behavior Fix**: Changed ESC from exiting program to returning to main menu
  - **"Continue Previous Game" Feature**: Added button to mode selection when saved game exists
  - **Non-blocking AI Moves**: Asynchronous AI processing with 3-second timeout
  - **Sequential Move Display**: Moves processed and displayed sequentially
  - **UI Instructions Updated**: "Press ESC to return to menu" instead of "Press ESC to exit"
  - **Dynamic Button Layout**: Mode selection adjusts based on saved game state
  - **UI Overlap Fixes**: Fixed overlapping UI elements in mode selection screen
    - Removed ESC hint from main menu (only appears during gameplay)
    - Fixed "Select a game mode to start" instruction overlapping with buttons
    - Implemented precise positioning calculation based on button height and spacing

- **Save/Load GUI Implementation (Dec 24, 2025)**:
  - **Save Game Button**: Added "Save Game" button to game UI (top-right area)
  - **Load Game Feature**: Added "Load Game" button to main menu with dedicated load screen
  - **Scrollable Load Screen**: Implemented scrollable saves list with 5 visible items when many saves exist
  - **Save Button Position Fix**: Moved button from Y=80 to Y=50 to avoid overlapping game board
  - **Implementation Details**: Uses existing Serializer class, compatible with text mode

- **Menu Aesthetics Improvements (Dec 24, 2025)**:
  - **Problem**: Graphical interface had poor color aesthetics with "completely green" valid moves and lack of modern design
  - **Solution**: Implemented modern color scheme with improved visual hierarchy
  - **Key Improvements**: 
    - Board colors: Light blue-gray and slate blue for modern look
    - Valid moves: Soft teal-blue instead of overwhelming green
    - Highlights: Gold for selected Amazon, steel blue for destination, muted coral red for arrows
    - Piece colors: Enhanced white and black Amazons with better outlines
    - Instructions updated to reflect new color names
  - **Testing**: Build compiles successfully, program runs with improved visual aesthetics
  - **File Modified**: src/ui/GraphicalController.cpp
  - **Build Status**: Compiles successfully

- **Arrow Validation Bug Fix (Dec 24, 2025)**:
  - **Problem**: Implementation incorrectly prevented arrows from targeting or passing through vacated squares
  - **Solution**: Updated GameState.cpp with new helper functions for proper arrow validation
  - **Key Improvements**: Arrows can now target vacated squares and pass through them to reach beyond
  - **Testing**: All 30 unit tests pass, user confirms fix works in both text and GUI modes
  - **File Modified**: src/core/GameState.cpp
  - **Build Status**: Compiles successfully, all tests pass

- **Project Reports**: Three comprehensive reports created for course evaluation (Dec 25-26, 2025):
  - English comprehensive report (20,451 bytes)
  - Chinese translation report (18,328 bytes)
  - Simplified Chinese condensed report (6,652 bytes)
  - All reports address the three required evaluation points: program execution, AI implementation, and special features

- **Project Infrastructure**:
  - Build system: `cmake --build . --target amazons` works flawlessly
  - Test execution: `./bin/unit_tests` runs all 30 tests successfully
  - Game execution: `./bin/amazons` provides complete game system with all modes
  - Comprehensive memory bank documentation maintained and updated

- **Project Rules and Documentation**:
  - **Task Preparation Rule**: Added to `.clinerules/project_setup.md` requiring reading of `docs/implementation/`, `memorybank/`, `README.md`, and other critical files before starting new tasks
  - **Documentation Workflows**: Established workflows for memory bank updates, README synchronization, manual maintenance, and implementation plan updates
  - **Implementation Plan Update Workflow**: Created `.clinerules/workflows/implementation_plan_update.md` to ensure `docs/implementation/` stays synchronized with project state
  - **Comprehensive Coverage**: Rules cover project setup, development workflow, documentation, version control, and memory bank management

### What's Next (Phase 4: Polish and Finalization)
- **Enhanced Features Polish**: Complete statistics tracking and replay system
- **Testing and Quality Assurance**: Add graphical interface tests and integration tests
- **Documentation Finalization**: Update game manual with graphical interface instructions
- **Project Reports Finalization**: Ensure all reports are ready for course submission
- **Critical Deadlines**: 
  - Second Review Session (Jan 9, 2026)
  - Final Review Session and project submission (Jan 10, 2026)

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
- [x] Aesthetic presentation and interface design - **Phase 3 Complete** (graphical GUI with modern color scheme)
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
├── .clinerules/                   # Cline operation rules and workflows
├── docs/                          # Comprehensive documentation
│   ├── implementation/            # Implementation plan (6 detailed documents)
│   ├── instructions/              # Course requirements (English and Chinese)
│   ├── manuals/                   # User guides and game manual
│   ├── reports/                   # Project reports for course evaluation
│   ├── guidance/                  # Development guidance and best practices
│   ├── problems/                  # Problem documentation and solutions
│   └── other/                     # Additional planning documents
├── memorybank/                    # Project memory bank (6 core files)
├── src/                           # Source code
│   ├── core/                      # Core game logic (Board, GameState, Move, etc.)
│   ├── ai/                        # AI implementations (BasicAI, BotzoneAI, BotProcess)
│   ├── ui/                        # User interface (TextDisplay, GraphicalController, etc.)
│   ├── utils/                     # Utilities (Serializer, etc.)
│   └── features/                  # Enhanced features (planned)
├── include/                       # Public headers (mirrors src/ structure)
├── tests/                         # Test suite
│   ├── unit/                      # Unit tests for core components
│   ├── integration/               # Integration tests
│   ├── system/                    # System tests
│   └── input/                     # Test input files
├── data/                          # Game data and configuration
│   ├── saves/                     # Saved game files
│   ├── config/                    # Configuration files (bot_config.json)
│   └── logs/                      # Application logs
├── scripts/                       # Build and utility scripts
├── third_party/                   # External dependencies
└── build/                         # Build output directory (generated)
```

For detailed directory descriptions and development guidelines, refer to the implementation plan documents.

## Development Setup

### Prerequisites
- C++ compiler (GCC ≥ 9.0, Clang ≥ 10.0, or MSVC ≥ 2019)
- CMake ≥ 3.16
- Git

### SFML Installation
The graphical interface requires SFML 3.0. Install it based on your platform:

**macOS** (using Homebrew):
```bash
brew install sfml
```

**Linux** (Ubuntu/Debian):
```bash
sudo apt-get install libsfml-dev
```

**Windows**:
- Download from [SFML website](https://www.sfml-dev.org/download.php)
- Or use vcpkg: `vcpkg install sfml`

**Verification**: After installation, CMake should automatically find SFML. If you encounter build errors, ensure SFML is installed and accessible.

**Note**: Text mode (`--text` flag) works without SFML if you only need console interface.

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
- 8×8 grid (standard implementation for this project)
- Each player starts with 4 Amazons in standard positions:
  - Black Amazons: (0, 2), (2, 0), (5, 0), (7, 2)
  - White Amazons: (0, 5), (2, 7), (5, 7), (7, 5)

### Movement
1. **Amazon Move**: Move one Amazon like a queen in chess (any distance in 8 directions)
2. **Arrow Shot**: From the new position, shoot an arrow like a queen (any distance, 8 directions)
3. **Restrictions**: Cannot move through or land on occupied squares (other Amazons, arrows, or obstacles)

### Game End
- Game ends when a player cannot make a legal move
- Winner is the player with the most territory (reachable squares)

## AI Integration

### Advanced AI from amazing-amazons Project
This project includes integration with an advanced AI algorithm from the amazing-amazons project:

1. **BotzoneAI**: Interface class that communicates with external bot003 executable
2. **BotProcess**: Handles external process communication and timeout management
3. **Configuration**: Bot path configured in `data/config/bot_config.json`
4. **Compatibility**: Board size compatibility fixed (8x8 instead of 10x10)

### Available AI Options
1. **BasicAI**: Greedy algorithm with mobility-based heuristic (built-in)
2. **BotzoneAI**: Advanced external bot003 algorithm (requires bot003 executable)

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
- ✅ Visual feedback with hover effects and modern color scheme
- ✅ Dual mode support: graphical (default) and text mode (`--text` flag)
- ✅ Mode selection screen with Human vs Human and Human vs AI options (AI vs AI removed from GUI)
- ✅ Keyboard shortcuts: R (restart), U (undo), ESC (return to menu)
- ✅ **GUI Improvements**: ESC key fix, "Continue Previous Game" feature, non-blocking AI moves, sequential move display
- ✅ **Save/Load GUI**: Save Game button, Load Game screen with scrollable list
- ✅ **Menu Aesthetics**: Modern color scheme with improved visual hierarchy
- ✅ **Arrow Validation Fix**: Corrected arrow targeting for vacated squares
- ✅ **AI Algorithm Migration**: Integrated advanced AI from amazing-amazons project
- ✅ **Project Reports**: Created comprehensive reports for course evaluation
- ✅ **Critical Goal Achieved**: Graphical GUI complete ahead of schedule with all requested improvements

### 🟡 Phase 4: Polish and Finalization (Week 4: Jan 7-10, 2026) - **IN PROGRESS**
- Enhanced features polish (statistics tracking, replay system)
- Testing and quality assurance (graphical interface tests, integration tests)
- Documentation finalization (game manual updates with graphical interface)
- Project reports finalization for course submission
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
- **Graphics Library**: SFML 3.0 for graphical interface (window management, rendering, input handling)

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
- **Project Reports**: Comprehensive documentation for course evaluation in `docs/reports/`:
  - `project_report_en.md`: English comprehensive report (20,451 bytes)
  - `project_report_zh.md`: Chinese translation report (18,328 bytes)
  - `project_report_simplified_zh.md`: Simplified Chinese condensed report (6,652 bytes)
  - `README.md`: Reports directory index and usage guide (4,677 bytes)
  - **Evaluation Points Covered**: All reports address the three required evaluation points:
    1. How to run the program (installation, building, execution)
    2. AI implementation details (BasicAI and BotzoneAI algorithms)
    3. Special features (dual interface system, save/load, enhanced UX features)

### Success Criteria
Detailed success metrics and grading requirements documented in `docs/implementation/06_success_criteria.md`. Phase 1 success criteria fully met.

## Contributing

This is an individual university assignment. The code and documentation serve as a learning example for C++ programming and software engineering principles.

## License

Educational use only - part of university coursework.