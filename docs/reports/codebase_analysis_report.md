# King of the Amazons: Codebase Analysis Report

## Executive Summary

### Project Overview
King of the Amazons is a C++ implementation of the abstract strategy board game "Game of the Amazons" (also known as "Queen's Game"). This project serves as a university assignment demonstrating mastery of C++ programming, software engineering principles, and game development concepts.

### Key Achievements
- **Complete Game Implementation**: Full implementation of Game of Amazons rules with 8x8 board
- **Dual Interface System**: Both graphical (SFML-based) and text-based console interfaces
- **AI Opponent**: Two AI implementations (BasicAI and BotzoneAI integration)
- **Enhanced Features**: Save/load system, undo functionality, side selection, modern GUI
- **Robust Architecture**: Clean MVC pattern with modular design

### Technical Stack
- **Language**: C++17/20 with modern features
- **Graphics**: SFML 3.0 for graphical interface
- **Build System**: CMake for cross-platform compilation
- **Testing**: Google Test framework with 30 unit tests
- **Serialization**: JSON-based save file format

## Project Architecture

### Model-View-Controller (MVC) Pattern
The project follows a clean MVC architecture:

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│     VIEW        │    │   CONTROLLER    │    │     MODEL       │
│                 │◄──►│                 │◄──►│                 │
│ - Display       │    │ - Input         │    │ - Game State    │
│ - GUI/Text UI   │    │ - Game Flow     │    │ - Rules Engine  │
│ - Menu System   │    │ - AI Interface  │    │ - Move Validation│
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Directory Structure
```
king-of-the-amazons/
├── src/                    # Source code implementation
│   ├── core/              # Core game logic
│   │   ├── GameState.cpp  # Complete game state management
│   │   ├── Board.cpp      # 8x8 board representation
│   │   ├── Move.cpp       # Move structure and validation
│   │   ├── Position.cpp   # Coordinate handling
│   │   └── Player.cpp     # Player information and game modes
│   ├── ai/                # AI implementations
│   │   ├── BasicAI.cpp    # Greedy algorithm AI
│   │   ├── BotzoneAI.cpp  # External bot integration
│   │   └── BotProcess.cpp # Process communication
│   ├── ui/                # User interface
│   │   ├── GraphicalController.cpp # SFML GUI controller
│   │   ├── MenuController.cpp      # Text menu controller
│   │   ├── TextDisplay.cpp         # Console display
│   │   └── InputHandler.cpp        # User input processing
│   └── utils/             # Utilities
│       └── Serializer.cpp # JSON serialization for save/load
├── include/               # Header files (mirrors src/ structure)
├── tests/                 # 30 unit tests across 4 test suites
├── docs/                  # Comprehensive documentation
├── data/                  # Configuration and save files
└── scripts/               # Test and utility scripts
```

### Build System Configuration
The project uses CMake for cross-platform compilation:
```cmake
# Key CMake configurations:
- C++17 standard requirement
- SFML integration for graphical interface
- Google Test integration for unit testing
- Separate compilation for each module
- Debug/Release build configurations
```

## Core Game Implementation

### Board Representation
The game uses an 8x8 board (not the traditional 10x10) with the following cell types:

```cpp
enum class Cell {
    EMPTY,          // Empty square
    ARROW,          // Arrow (blocked square)
    WHITE_AMAZON,   // White player's Amazon
    BLACK_AMAZON    // Black player's Amazon
};
```

**Board Initialization**: Standard starting position with:
- White Amazons: Bottom rows (rows 6-7 in 8x8 board)
- Black Amazons: Top rows (rows 0-1 in 8x8 board)
- Symmetric placement for balanced gameplay

### Move Validation System
A complete move in Amazons consists of three positions:
1. **From**: Starting position of Amazon
2. **To**: Destination for Amazon movement
3. **Arrow**: Position where arrow is shot

**Validation Logic**:
```cpp
bool GameState::isValidMove(const Move& move) const {
    // 1. Check if it's the correct player's turn
    // 2. Verify Amazon movement is queen-style (8 directions, any distance)
    // 3. Ensure movement path is clear (no pieces or arrows)
    // 4. Validate arrow shot from new position
    // 5. Verify arrow path is clear (treating vacated square as empty)
    // 6. Arrow must land on empty square or vacated square
}
```

**Key Implementation Detail**: The arrow validation correctly treats the vacated square (original Amazon position) as empty, allowing arrows to target or pass through that square.

### Game State Management
The `GameState` class manages:
- Current board configuration
- Active player (Black moves first)
- Turn number tracking
- Move history for undo functionality
- Game over detection

**Undo System**:
```cpp
void GameState::undoLastMove() {
    if (moveHistory.empty()) throw std::logic_error("No moves to undo");
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    // Reverse the move: remove arrow, move Amazon back, restore original position
    // Update player turn and turn number
}
```

**Special Undo Behavior**: In AI vs Human mode, pressing 'U' during human's turn undoes both the AI's move and the human's previous move with a single key press.

## AI System Design

### BasicAI Implementation
The `BasicAI` class implements a greedy algorithm with mobility-based heuristic:

```cpp
Move BasicAI::getBestMove(const GameState& gameState) const {
    auto legalMoves = gameState.getLegalMoves();
    Move bestMove = legalMoves[0];
    int bestScore = std::numeric_limits<int>::min();
    
    for (const auto& move : legalMoves) {
        int score = evaluateMove(gameState, move);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}
```

**Heuristic Evaluation**:
```cpp
int BasicAI::evaluateMove(const GameState& gameState, const Move& move) const {
    // Simulate the move
    GameState simulatedState = gameState;
    simulatedState.makeMove(move);
    
    // Calculate mobility difference
    int ourMobility = countAvailableMoves(simulatedState, currentPlayer);
    int opponentMobility = countAvailableMoves(simulatedState, opponent);
    
    // Prefer moves that increase our mobility and decrease opponent's
    return ourMobility - opponentMobility;
}
```

**Additional Heuristics**:
- Center control bonus for Amazons in central squares (rows/cols 3-6)
- Random move fallback when no heuristic advantage is found

### BotzoneAI Integration
The `BotzoneAI` class provides integration with an external advanced AI (`bot003.cpp` from the amazing-amazons project):

**Architecture**:
```
BotzoneAI → BotProcess → External bot003 executable
    ↑           ↓
    └───── stdin/stdout ─────┘
```

**Key Features**:
- Configurable bot path via `data/config/bot_config.json`
- Timeout handling for AI thinking
- Protocol compatibility with Botzone platform
- Support for AI as either Black or White

### AI Decision Process
1. **Move Generation**: Generate all legal moves for current position
2. **Position Evaluation**: Apply heuristic to each possible move
3. **Move Selection**: Choose move with highest evaluation score
4. **Time Management**: Ensure decision within reasonable time limits

## User Interface Implementation

### Dual-Mode Architecture
The program supports two interface modes:

**Graphical Mode (Default)**:
- SFML-based windowed application
- Mouse-driven interaction
- Modern color scheme with hover effects
- Visual feedback for selections and valid moves

**Text Mode** (`--text` flag):
- Console-based interface
- Keyboard input for moves
- Simple character-based board display
- Full menu system with save/load functionality

**Entry Point** (`main.cpp`):
```cpp
int main(int argc, char* argv[]) {
    // Parse command line arguments
    // --graphical: Use graphical interface (default)
    // --text: Use text interface
    // --help: Show usage information
    
    // Initialize appropriate controller based on mode
    if (runMode == RunMode::GRAPHICAL) {
        amazons::GraphicalController controller;
        controller.run();
    } else {
        auto display = std::make_unique<amazons::TextDisplay>();
        amazons::MenuController controller(std::move(display));
        controller.run();
    }
}
```

### GraphicalController Implementation
The `GraphicalController` class manages the SFML-based GUI:

**State Machine**:
```cpp
enum class SelectionState {
    NO_SELECTION,      // No piece selected
    AMAZON_SELECTED,   // Amazon selected, showing valid moves
    MOVE_SELECTED      // Move destination selected, showing valid arrows
};
```

**Three-Step Interaction**:
1. **Select Amazon**: Click on player's Amazon to select it
2. **Choose Destination**: Click on highlighted teal cell for move destination
3. **Shoot Arrow**: Click on highlighted coral cell for arrow position

**Visual Design**:
- **Board Colors**: Light blue-gray (232,235,239) and slate blue (125,135,150)
- **Highlights**: 
  - Selected Amazon: Gold (255,215,0,120)
  - Valid moves: Soft teal-blue (100,200,225,100)
  - Move destination: Steel blue (70,130,180,120)
  - Valid arrows: Muted coral red (220,100,100,100)
- **Pieces**: White Amazons with gray outline, Black Amazons with light gray outline

**Key Features**:
- Hover effects on board cells
- Status messages and game mode indicators
- Save button integrated into game UI
- Scrollable load screen for many saved games
- "Continue Previous Game" feature

### MenuController Implementation
The `MenuController` class manages the text-based interface:

**Menu System**:
```
Main Menu:
1. New Game
2. Load Game
3. Save Game
4. Exit

New Game Submenu:
1. Human vs Human
2. Human vs AI
3. AI vs AI
```

**Input Handling**:
- Move input: Six numbers (row col row col row col)
- Commands: "save", "undo", "exit", "help"
- Error handling with clear feedback messages

**Game Flow**:
```cpp
void MenuController::gameLoop() {
    while (!gameState->isGameOver()) {
        showGameStatus();
        if (!playerTurn()) break; // User wants to exit
        if (currentGameMode == HUMAN_VS_AI) makeAIMove();
    }
    showGameResult();
}
```

## Enhanced Features Implementation

### Save/Load System
The `Serializer` class provides JSON-based game state persistence:

**Save File Format** (JSON):
```json
{
  "board": "string representation of 8x8 board",
  "current_player": "white|black",
  "turn_number": 5,
  "game_mode": "HUMAN_VS_AI_HUMAN_WHITE",
  "move_history": [...]
}
```

**Key Methods**:
```cpp
bool Serializer::saveGame(const GameState& gameState, GameMode gameMode, 
                         const std::string& filename) const;
std::pair<std::unique_ptr<GameState>, GameMode> 
Serializer::loadGameWithMode(const std::string& filename) const;
```

**Features**:
- Timestamp-based filenames: `game_YYYYMMDD_HHMMSS.json`
- Backward compatibility with old save files
- Save directory management (`data/saves/`)
- Error handling for file operations

### Side Selection System
Players can choose which side to play in AI vs Human mode:

**Game Mode Enum**:
```cpp
enum class GameMode {
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI_HUMAN_WHITE,  // Human as White, AI as Black
    HUMAN_VS_AI_HUMAN_BLACK   // Human as Black, AI as White
};
```

**Implementation**:
- Graphical side selection screen with "Play as Black" and "Play as White" buttons
- Text menu for side selection in console mode
- Botzone protocol adjustment based on AI color
- Proper handling of who moves first (Black always moves first in Amazons)

### Modern GUI Improvements

**Color Harmonization**:
- Side selection buttons use colors that match board squares:
  - "Play as Black": Dark slate blue (90,100,120) matches dark squares
  - "Play as White": Light blue-gray (210,220,230) matches light squares
- Improved contrast for "Play as White" button text (dark gray on light background)

**Scrollable Load Screen**:
- Shows 5 visible saves at a time
- Scroll buttons (▲/▼) for navigation
- Position indicator (e.g., "1-5/10")
- Disabled buttons at list boundaries

**UI State Management**:
- Fixed bug where returning to menu didn't reset game mode state
- Proper handling of ESC key (returns to menu instead of closing)
- Dynamic button layout based on saved game presence

## Code Organization Patterns

### Header/Source Separation
Clean separation between interface and implementation:
- Header files in `include/` directory with `.hpp` extension
- Source files in `src/` directory with `.cpp` extension
- Consistent naming: `ClassName.hpp` and `ClassName.cpp`

### Namespace Usage
All project code is in the `amazons` namespace:
```cpp
namespace amazons {
    class GameState { ... };
    class Board { ... };
    // ...
}
```

This prevents naming conflicts and clearly identifies project-specific code.

### Error Handling Strategy
- **Exceptions** for programming errors and invalid operations
- **Return values** for expected error conditions (file I/O, user input)
- **Assertions** in debug builds for invariant checking
- **Comprehensive error messages** for user feedback

### Memory Management
- **Smart pointers**: `std::unique_ptr` for exclusive ownership
- **RAII pattern**: Resource acquisition in constructors, release in destructors
- **Move semantics**: Efficient transfer of game states and displays
- **Stack allocation**: Where appropriate for small, short-lived objects

### Testing Strategy
**30 Unit Tests** across 4 test suites:
1. `PositionTest`: Coordinate validation and conversion
2. `BoardTest`: Board initialization and move generation
3. `GameStateTest`: Game flow and move validation
4. `MoveTest`: Move structure and serialization

**Test Organization**:
- Tests in `tests/unit/` directory
- Google Test framework integration
- Comprehensive coverage of core functionality
- Regular test execution during development

## Technical Challenges & Solutions

### 1. Arrow Validation Bug
**Problem**: Initial implementation incorrectly prevented arrows from targeting or passing through vacated squares.

**Solution**: Implemented `isArrowPathClearWithVacated()` helper function that treats the vacated square (original Amazon position) as empty during arrow validation.

```cpp
bool isArrowPathClearWithVacated(const Board& board, 
                                 const Position& arrowFrom, 
                                 const Position& arrowTo,
                                 const Position& vacatedSquare) {
    // Special handling: vacatedSquare is treated as empty
    if (board.getCell(arrowTo) != Board::Cell::EMPTY && 
        !(arrowTo == vacatedSquare)) {
        return false;
    }
    // ... path checking with vacated square handling
}
```

### 2. GUI State Management
**Problem**: After returning to main menu from interrupted AI vs Human game, clicking the AI vs Human button showed empty board instead of side selection screen.

**Root Cause**: `currentGameMode` variable wasn't reset to `NOT_SELECTED` when returning to menu.

**Solution**: Fixed three locations in `GraphicalController.cpp`:
```cpp
// In handleKeyPress() (ESC handler):
currentGameMode = GameModeGUI::NOT_SELECTED;

// In handleModeSelection() (Human vs AI button):
currentGameMode = GameModeGUI::NOT_SELECTED;

// In handleSideSelection() (Back button):
currentGameMode = GameModeGUI::NOT_SELECTED;
```

### 3. AI vs Human Mode Undo Behavior
**Problem**: In AI vs Human mode, pressing 'U' once only undid AI's move, requiring second press to undo human's move.

**Solution**: Modified undo logic to detect AI vs Human mode and human's turn, then undo both moves with single 'U' press:

```cpp
if (isHumanVsAI && gameState->getCurrentPlayer() == humanColor) {
    // Undo AI move
    if (gameState->canUndo()) gameState->undoLastMove();
    // Undo human move
    if (gameState->canUndo()) gameState->undoLastMove();
}
```

### 4. Human vs Human Mode Fix
**Problem**: Human vs Human mode was infeasible - users could not move any Amazons.

**Root Cause**: `handleMouseClick()` function returned early for non-AI modes.

**Solution**: Restructured logic to only check for AI's turn when in AI modes:

```cpp
// Only check AI's turn in AI modes
if (currentGameMode == GameModeGUI::HUMAN_VS_AI_HUMAN_BLACK || 
    currentGameMode == GameModeGUI::HUMAN_VS_AI_HUMAN_WHITE) {
    // Check if it's AI's turn
    if (gameState->getCurrentPlayer() == aiColor) return;
}
// For HUMAN_VS_HUMAN mode, process click normally
```

### 5. Cross-Platform Compatibility
**Challenges**:
- SFML library availability on different platforms
- File path differences (Windows vs Unix)
- Font availability for graphical interface

**Solutions**:
- **CMake Configuration**: Automatic SFML detection with fallback to text mode
- **Filesystem Abstraction**: Use `std::filesystem` for cross-platform file operations
- **Font Fallbacks**: Multiple font loading attempts with system-specific paths
- **Conditional Compilation**: Platform-specific code sections with preprocessor directives

### 6. Board Size Compatibility
**Problem**: Initial implementation had hardcoded 10x10 board size, but migrated AI expected 8x8 board.

**Solution**: Updated `Position.hpp` and `Position.cpp` to use `Board::SIZE` constant:
```cpp
// Position.hpp - Forward declaration
class Board;

struct Position {
    bool isValid() const; // Implementation in Position.cpp
};

// Position.cpp
#include "core/Board.hpp"
bool Position::isValid() const {
    return row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE;
}
```

**Test Updates**: Modified `GameStateTest.cpp` to check correct positions for 8x8 board.

## Design Patterns and Best Practices

### Factory Pattern for Display Creation
```cpp
std::unique_ptr<Display> MenuController::createDisplay(bool useGraphical) {
    if (useGraphical) {
        return std::make_unique<GraphicalDisplay>();
    } else {
        return std::make_unique<TextDisplay>();
    }
}
```

### Strategy Pattern for AI Algorithms
```cpp
class AIStrategy {
public:
    virtual Move getBestMove(const GameState& state) = 0;
};

class BasicAI : public AIStrategy { /* Greedy algorithm */ };
class BotzoneAI : public AIStrategy { /* External bot integration */ };
```

### Command Pattern for Move Execution
```cpp
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class MoveCommand : public Command {
    GameState& state;
    Move move;
public:
    void execute() override { state.makeMove(move); }
    void undo() override { state.undoLastMove(); }
};
```

### Observer Pattern for State Updates
```cpp
class GameStateObserver {
public:
    virtual void onStateChanged(const GameState& newState) = 0;
};

class GraphicalController : public GameStateObserver {
    void onStateChanged(const GameState& newState) override {
        // Update UI based on new game state
    }
};
```

## Performance Considerations

### Move Generation Optimization
- **Pre-computation**: Cache legal moves for each position when possible
- **Early Termination**: Stop move generation when game over condition detected
- **Efficient Data Structures**: Use `std::vector` with reserve for move lists

### Memory Management
- **Smart Pointers**: `std::unique_ptr` for exclusive ownership of game states
- **Move Semantics**: Transfer large objects (like GameState) efficiently
- **Stack Allocation**: Use stack for small, frequently accessed objects

### Graphical Rendering
- **60 FPS Target**: SFML window with framerate limit
- **Efficient Drawing**: Batch draw calls where possible
- **Texture Management**: Reuse textures for board and pieces

## Future Improvements and Enhancements

### Planned Features
1. **Statistics Tracking**: Game history, win/loss records, move analysis
2. **Replay System**: Record and playback complete games
3. **Advanced AI**: Minimax with alpha-beta pruning, Monte Carlo Tree Search
4. **Network Play**: Online multiplayer functionality
5. **Customizable Rules**: Variable board sizes, alternative starting positions

### Code Quality Improvements
1. **Additional Unit Tests**: Cover UI components and edge cases
2. **Integration Tests**: End-to-end game flow testing
3. **Performance Profiling**: Identify and optimize bottlenecks
4. **Code Documentation**: Doxygen comments for public APIs
5. **Static Analysis**: Regular clang-tidy and clang-format runs

### User Experience Enhancements
1. **Tutorial System**: Interactive guide for new players
2. **Difficulty Settings**: Adjustable AI strength
3. **Theme Customization**: User-selectable color schemes
4. **Sound Effects**: Audio feedback for moves and game events
5. **Animation System**: Smooth piece movement and arrow shooting

## Conclusion

### Project Success Assessment
The King of the Amazons project successfully demonstrates:

1. **Technical Proficiency**: Clean C++ implementation with modern features
2. **Software Architecture**: Well-structured MVC pattern with clear separation of concerns
3. **Game Development Skills**: Complete implementation of complex game rules
4. **User Interface Design**: Dual-mode interface with professional graphical UI
5. **Problem-Solving Ability**: Effective solutions to technical challenges

### Code Quality Evaluation
- **Readability**: Clear naming conventions, consistent formatting
- **Maintainability**: Modular design, comprehensive documentation
- **Testability**: 30 unit tests covering core functionality
- **Performance**: Efficient algorithms, appropriate data structures
- **Robustness**: Comprehensive error handling, edge case management

### Educational Value
This project serves as an excellent example of:
- Object-oriented design in C++
- Game development principles
- AI algorithm implementation
- User interface programming
- Software engineering best practices

### Recommendations for Future Development
1. **Focus on Enhanced Features**: Complete statistics, replay, and analysis systems
2. **Improve AI Strength**: Implement more advanced search algorithms
3. **Expand Testing**: Add integration tests for graphical interface
4. **Documentation Maintenance**: Keep documentation synchronized with code changes
5. **Community Engagement**: Consider open-sourcing for collaborative development

The King of the Amazons codebase represents a solid foundation for a professional-quality game implementation, with clear architecture, robust implementation, and room for future enhancement.
