# Technical Specifications: King of the Amazons

## Overview
This document provides detailed technical specifications for the "King of the Amazons" implementation, including architecture, algorithms, data structures, and APIs.

## System Architecture

### MVC Architecture Pattern
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│     VIEW        │    │   CONTROLLER    │    │     MODEL       │
│                 │◄──►│                 │◄──►│                 │
│ - Display       │    │ - Input         │    │ - Game State    │
│ - User Interface│    │ - Game Flow     │    │ - Rules Engine  │
│ - Menu System   │    │ - AI Interface  │    │ - Move Validation│
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Component Relationships
```
GameController ────┬─── GameState
                   ├─── Display
                   ├─── AIController
                   └─── MenuController

AIController ──────┴─── AIStrategy

Display ──────────┬─── TextDisplay
                  └─── GraphicalDisplay (optional enhancement)

FeatureController ────┬─── UndoManager
                      ├─── GameAnalyzer
                      ├─── ReplaySystem
                      └─── Statistics
```

## Core Data Structures

### Board Representation
```cpp
class Board {
private:
    static const int SIZE = 10;
    std::array<std::array<Cell, SIZE>, SIZE> grid;
    
public:
    enum class Cell { EMPTY, ARROW, WHITE_AMAZON, BLACK_AMAZON };
    
    Board();
    void initializeStandardPosition();
    bool isValidPosition(int row, int col) const;
    Cell getCell(int row, int col) const;
    void setCell(int row, int col, Cell value);
    std::vector<Position> getLegalMoves(const Position& from) const;
    std::vector<Position> getLegalShots(const Position& from) const;
    int countReachableSquares(Player player) const; // For territory calculation
};
```

### Game State
```cpp
class GameState {
private:
    Board board;
    Player currentPlayer;
    std::vector<Move> moveHistory;
    int moveCount;
    GameStatus status;
    
public:
    enum class GameStatus { ONGOING, WHITE_WIN, BLACK_WIN, DRAW };
    enum class Player { WHITE, BLACK };
    
    GameState();
    GameStatus getStatus() const;
    Player getCurrentPlayer() const;
    const Board& getBoard() const;
    std::vector<Move> getLegalMoves() const;
    bool makeMove(const Move& move);
    bool undoMove(); // For undo functionality
    std::string serialize() const;
    static GameState deserialize(const std::string& data);
    int getMoveCount() const;
    const std::vector<Move>& getMoveHistory() const;
};
```

### Move Representation
```cpp
struct Position {
    int row;
    int col;
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    std::string toString() const;
    static Position fromString(const std::string& str);
};

struct Move {
    Position amazonFrom;
    Position amazonTo;
    Position arrowTo;
    
    bool isValid() const;
    std::string toString() const;
    static Move fromString(const std::string& str);
    bool operator==(const Move& other) const;
};
```

## AI Algorithms

### Minimax with Alpha-Beta Pruning
```cpp
class MinimaxAI : public AIStrategy {
private:
    int maxDepth;
    HeuristicEvaluator evaluator;
    TranspositionTable transpositionTable; // For caching results
    
    int minimax(GameState& state, int depth, int alpha, int beta, bool maximizing);
    std::vector<Move> orderMoves(const GameState& state, 
                                 const std::vector<Move>& moves);
    
public:
    MinimaxAI(int depth = 3);
    Move getBestMove(const GameState& state) override;
    void setDepth(int depth);
    int getNodesEvaluated() const; // For performance tracking
};
```

### Heuristic Evaluation Function
```cpp
class HeuristicEvaluator {
private:
    // Weights for different evaluation factors
    const float MOBILITY_WEIGHT = 0.4f;
    const float TERRITORY_WEIGHT = 0.3f;
    const float POSITION_WEIGHT = 0.2f;
    const float MATERIAL_WEIGHT = 0.1f;
    
public:
    float evaluate(const GameState& state);
    
private:
    float calculateMobility(const GameState& state);
    float calculateTerritory(const GameState& state);
    float calculatePositionScore(const GameState& state);
    float calculateMaterial(const GameState& state);
    float calculateCenterControl(const GameState& state);
};
```

### Move Generation Optimization
```cpp
class MoveGenerator {
private:
    // Pre-computed direction vectors for queen moves
    static const std::array<Position, 8> DIRECTIONS;
    
public:
    std::vector<Move> generateAllMoves(const GameState& state);
    
private:
    std::vector<Position> getReachablePositions(const Board& board,
                                                const Position& from);
    std::vector<Position> getShootablePositions(const Board& board,
                                                const Position& from);
    bool isPathClear(const Board& board, const Position& from, 
                     const Position& to, const Position& direction);
};
```

## Enhanced Features

### Undo/Redo System
```cpp
class UndoManager {
private:
    std::vector<GameState> undoStack;
    std::vector<GameState> redoStack;
    const size_t MAX_HISTORY = 100;
    
public:
    bool canUndo() const;
    bool canRedo() const;
    GameState undo(GameState currentState);
    GameState redo(GameState currentState);
    void recordState(const GameState& state);
    void clear();
};
```

### Game Analysis
```cpp
class GameAnalyzer {
public:
    struct AnalysisResult {
        int whiteTerritory;
        int blackTerritory;
        float whiteAdvantage; // -1.0 to 1.0 scale
        std::vector<Position> criticalPositions;
        std::string recommendation;
    };
    
    AnalysisResult analyzePosition(const GameState& state);
    std::vector<Move> findBestMoves(const GameState& state, int count = 3);
    std::string generateCommentary(const GameState& state);
};
```

### Statistics Tracking
```cpp
class Statistics {
private:
    struct GameStats {
        int gamesPlayed;
        int whiteWins;
        int blackWins;
        int draws;
        double averageMovesPerGame;
        std::map<std::string, int> openingMoves;
    };
    
    GameStats currentStats;
    
public:
    void recordGame(const GameState& finalState);
    void recordMove(const Move& move, int moveNumber);
    GameStats getStatistics() const;
    void saveStatistics(const std::string& filename);
    void loadStatistics(const std::string& filename);
};
```

### Replay System
```cpp
class ReplaySystem {
private:
    std::vector<GameState> gameStates;
    size_t currentIndex;
    
public:
    void recordGame(const std::vector<GameState>& states);
    bool hasNext() const;
    bool hasPrevious() const;
    GameState next();
    GameState previous();
    GameState jumpTo(int moveNumber);
    void saveReplay(const std::string& filename);
    void loadReplay(const std::string& filename);
};
```

## User Interface Specifications

### Display Interface (Abstract Base Class)
```cpp
class Display {
public:
    virtual ~Display() = default;
    
    // Core display methods
    virtual void showBoard(const Board& board) = 0;
    virtual void showMessage(const std::string& message) = 0;
    virtual void showMenu(const std::vector<std::string>& options) = 0;
    virtual void showGameStatus(const GameState& state) = 0;
    virtual void showAnalysis(const GameAnalyzer::AnalysisResult& analysis) = 0;
    
    // Input handling
    virtual std::string getInput() = 0;
    virtual bool hasMouseInput() const = 0;
    virtual std::optional<Position> getMouseClick() = 0;
    
    // Window management (for graphical displays)
    virtual void initializeWindow(int width, int height) { }
    virtual void closeWindow() { }
    virtual bool isWindowOpen() const { return true; }
    virtual void processEvents() { }
    
    // Factory method
    static std::unique_ptr<Display> create(DisplayType type);
};

enum class DisplayType {
    TEXT,
    GRAPHICAL
};
```

### Text Display Interface
```cpp
class TextDisplay : public Display {
private:
    const std::string WHITE_AMAZON = "W";
    const std::string BLACK_AMAZON = "B";
    const std::string ARROW = "X";
    const std::string EMPTY = ".";
    
public:
    void showBoard(const Board& board) override;
    void showMessage(const std::string& message) override;
    void showMenu(const std::vector<std::string>& options) override;
    void showGameStatus(const GameState& state) override;
    void showAnalysis(const GameAnalyzer::AnalysisResult& analysis) override;
    std::string getInput() override;
    bool hasMouseInput() const override { return false; }
    std::optional<Position> getMouseClick() override { return std::nullopt; }
    
private:
    std::string cellToString(Board::Cell cell) const;
    void printBoardWithCoordinates(const Board& board);
};
```

### Graphical Controller Interface (SFML-based Pure GUI)
```cpp
class GraphicalController {
private:
    enum class SelectionState {
        NO_SELECTION,
        AMAZON_SELECTED,
        MOVE_SELECTED
    };
    
    enum class GameModeGUI {
        NOT_SELECTED,
        HUMAN_VS_HUMAN,
        HUMAN_VS_AI,
        AI_VS_AI
    };
    
    std::unique_ptr<sf::RenderWindow> window;
    sf::Font font;
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<GameState> savedGameState;
    GameModeGUI savedGameMode;
    SelectionState selectionState;
    Position selectedPosition;
    Position moveToPosition;
    std::vector<Position> validMoves;
    std::vector<Position> validArrowPositions;
    GameModeGUI currentGameMode;
    bool showModeSelection;
    std::string statusMessage;
    std::unique_ptr<BasicAI> ai;
    
    // Constants
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 800;
    static const int CELL_SIZE = 70;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 100;
    static const int BOARD_SIZE = CELL_SIZE * Board::SIZE;
    
public:
    GraphicalController();
    ~GraphicalController();
    
    bool initialize();
    void run();
    
private:
    void handleEvents();
    void handleMouseClick(int x, int y);
    void handleModeSelection(int x, int y);
    void handleKeyPress(sf::Keyboard::Key key);
    void continueGame();
    void startGame(GameModeGUI mode);
    void selectAmazon(const Position& pos);
    void selectMoveDestination(const Position& pos);
    void selectArrowDestination(const Position& pos);
    void makeMove(const Move& move);
    void processAIMove();
    void resetSelection();
    void updateStatusMessage();
    void render();
    void drawModeSelection();
    void drawBoard();
    void drawPieces();
    void drawHighlights();
    void drawUI();
    sf::Color getCellColor(int row, int col) const;
    std::optional<Position> getBoardPosition(int mouseX, int mouseY) const;
    bool loadResources();
};
```

### Dual Mode Architecture
```cpp
// Main entry point with command-line argument parsing
int main(int argc, char* argv[]) {
    // Check command line arguments for mode preference
    enum class RunMode { GRAPHICAL, TEXT };
    RunMode runMode = RunMode::GRAPHICAL; // Default to graphical
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--graphical" || arg == "-g") {
            runMode = RunMode::GRAPHICAL;
        } else if (arg == "--text" || arg == "-t") {
            runMode = RunMode::TEXT;
        } else if (arg == "--help" || arg == "-h") {
            // Show help
            return 0;
        }
    }
    
    if (runMode == RunMode::GRAPHICAL) {
#ifdef WITH_GRAPHICAL_GUI
        std::cout << "Starting pure graphical interface...\n";
        amazons::GraphicalController controller;
        controller.run();
        return 0;
#else
        std::cout << "Graphical interface not available. Falling back to text interface.\n";
        runMode = RunMode::TEXT;
#endif
    }
    
    // Text mode
    if (runMode == RunMode::TEXT) {
        auto display = std::make_unique<amazons::TextDisplay>();
        amazons::MenuController controller(std::move(display));
        controller.run();
        return 0;
    }
    
    return 0;
}
```

### Menu System
```cpp
class MenuController {
private:
    std::map<int, std::function<void()>> menuActions;
    
public:
    enum class MenuOption {
        NEW_GAME,
        LOAD_GAME,
        SAVE_GAME,
        UNDO_MOVE,
        REDO_MOVE,
        SHOW_ANALYSIS,
        VIEW_STATISTICS,
        REPLAY_MODE,
        SETTINGS,
        EXIT
    };
    
    void showMainMenu();
    void showGameMenu();
    void handleSelection(MenuOption option);
    void registerAction(MenuOption option, std::function<void()> action);
};
```

## Save/Load System

### Serialization Format (JSON)
```json
{
  "game_state": {
    "board": {
      "grid": [
        ["EMPTY", "ARROW", "WHITE_AMAZON", ...],
        ...
      ]
    },
    "current_player": "WHITE",
    "move_count": 42,
    "move_history": [
      {
        "amazon_from": {"row": 0, "col": 3},
        "amazon_to": {"row": 3, "col": 6},
        "arrow_to": {"row": 5, "col": 8}
      },
      ...
    ]
  },
  "metadata": {
    "version": "1.0",
    "save_date": "2025-12-17T12:00:00Z",
    "game_duration": "01:30:00",
    "players": ["Human", "AI (Minimax depth 3)"]
  }
}
```

### Serializer Interface
```cpp
class Serializer {
public:
    static std::string serializeGameState(const GameState& state);
    static GameState deserializeGameState(const std::string& data);
    
    static void saveToFile(const GameState& state, 
                          const std::string& filename);
    static GameState loadFromFile(const std::string& filename);
    
    static std::string serializeStatistics(const Statistics& stats);
    static Statistics deserializeStatistics(const std::string& data);
    
private:
    static nlohmann::json gameStateToJson(const GameState& state);
    static GameState jsonToGameState(const nlohmann::json& j);
};
```

## Performance Specifications

### Time Constraints
- **AI Move Decision**: ≤ 2 seconds per move for depth 3 search
- **Move Generation**: ≤ 100ms for all legal moves in mid-game position
- **State Evaluation**: ≤ 10ms per evaluation
- **Save/Load Operations**: ≤ 1 second for typical game state
- **Undo/Redo Operations**: ≤ 50ms per operation
- **Graphical Rendering**: ≤ 16ms per frame (60 FPS)
- **Mouse Input Response**: ≤ 50ms for click processing
- **Window Resize/Update**: ≤ 100ms for complete redraw

### Memory Constraints
- **Game State**: ≤ 1 MB per state
- **Search Tree**: ≤ 100 MB maximum during AI search
- **Save Files**: ≤ 10 KB per saved game
- **History Storage**: ≤ 10 MB for undo/redo and replay systems
- **Graphical Assets**: ≤ 50 MB for textures and fonts
- **Window Buffer**: ≤ 10 MB for frame buffer

### Computational Complexity
- **Move Generation**: O(n²) where n is board size (10x10)
- **Minimax Search**: O(b^d) where b is branching factor (~2000), d is depth
- **State Evaluation**: O(n²) for board evaluation
- **Territory Calculation**: O(n²) using BFS/DFS

## Optimization Strategies

### Move Generation Optimization
1. **Pre-computed Directions**: Store direction vectors for queen moves
2. **Ray Casting**: Check paths incrementally instead of generating all positions
3. **Caching Legal Moves**: Cache legal moves for each position when board hasn't changed
4. **Early Termination**: Stop generation when maximum moves reached for AI purposes

### AI Search Optimization
1. **Alpha-Beta Pruning**: Reduce search space exponentially
2. **Move Ordering**: Try best moves first (captures, center moves, threats)
3. **Transposition Tables**: Cache evaluation results for repeated positions
4. **Iterative Deepening**: Search incrementally deeper within time limits
5. **Quiescence Search**: Continue search in volatile positions

### Memory Optimization
1. **Copy-on-Write**: Efficient state copying for search trees
2. **Flyweight Pattern**: Share common data between game states
3. **Pool Allocation**: Custom memory management for frequent allocations
4. **History Pruning**: Limit undo/redo stack size

## Error Handling

### Exception Hierarchy
```cpp
class GameException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidMoveException : public GameException {
public:
    InvalidMoveException(const std::string& message);
};

class FileIOException : public GameException {
public:
    FileIOException(const std::string& message);
};

class AIException : public GameException {
public:
    AIException(const std::string& message);
};

class UndoException : public GameException {
public:
    UndoException(const std::string& message);
};
```

### Recovery Strategies
1. **Invalid Moves**: Prompt user for valid input with suggestions
2. **File Errors**: Provide fallback options and clear error messages
3. **AI Timeouts**: Return best move found within time limit
4. **Memory Exhaustion**: Graceful degradation of AI search depth
5. **Corrupted Saves**: Automatic backup restoration

## Testing Specifications

### Unit Test Coverage
- **Board Class**: 100% coverage of public methods
- **GameState Class**: 100% coverage of state management
- **Move Validation**: All edge cases and invalid moves
- **AI Algorithms**: Correctness of search and evaluation
- **Enhanced Features**: Undo/redo, analysis, statistics functionality

### Integration Test Scenarios
1. **Complete Game Flow**: Start to finish with various outcomes
2. **Save/Load Cycle**: Save game, load game, continue playing
3. **AI vs AI**: Automated games to test AI stability
4. **Feature Integration**: Undo/redo during gameplay, analysis generation

### Performance Test Benchmarks
1. **Move Generation Speed**: ≥ 1000 moves/second
2. **AI Decision Time**: ≤ 2 seconds for depth 3 search
3. **Memory Usage**: ≤ 100 MB peak during intensive search
4. **Save/Load Performance**: ≤ 1 second for 100-move game
5. **Undo/Redo Performance**: ≤ 50ms per operation

## Platform Compatibility

### Supported Compilers
- **GCC**: ≥ 9.0
- **Clang**: ≥ 10.0
- **MSVC**: ≥ 2019

### Operating Systems
- **Linux**: Ubuntu 20.04+, Fedora, Arch
- **macOS**: 10.15+ (Catalina and later)
- **Windows**: 10+ with Visual Studio or MinGW

### Dependencies
- **Required**: C++17 standard library
- **Optional**: nlohmann/json for JSON serialization
- **Graphical**: SFML 2.5 (Graphics, Window, System modules)
- **Testing**: Google Test framework
- **Build**: CMake ≥ 3.16

## Security Considerations

### Input Validation
- Validate all user input before processing
- Sanitize file paths for save/load operations
- Limit AI search depth to prevent excessive resource usage
- Validate serialized data before deserialization

### Memory Safety
- Use smart pointers for dynamic memory management
- Implement bounds checking for all array accesses
- Use RAII for resource management
- Regular valgrind checks for memory leaks

### File Operations
- Restrict save file locations to user-writable directories
- Validate file formats before loading
- Implement backup mechanism for corrupted save files
- Sanitize user-provided filenames

This technical specification provides a comprehensive blueprint for implementing the Game of Amazons with clear performance targets, architectural guidelines, and quality standards, including enhanced features for improved user experience.
