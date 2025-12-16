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
                  └─── GraphicalDisplay (optional)
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
    
public:
    enum class GameStatus { ONGOING, WHITE_WIN, BLACK_WIN, DRAW };
    
    GameState();
    GameStatus getStatus() const;
    Player getCurrentPlayer() const;
    const Board& getBoard() const;
    std::vector<Move> getLegalMoves() const;
    bool makeMove(const Move& move);
    bool undoMove();
    std::string serialize() const;
    static GameState deserialize(const std::string& data);
};
```

### Move Representation
```cpp
struct Position {
    int row;
    int col;
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};

struct Move {
    Position amazonFrom;
    Position amazonTo;
    Position arrowTo;
    
    bool isValid() const;
    std::string toString() const;
    static Move fromString(const std::string& str);
};
```

## AI Algorithms

### Minimax with Alpha-Beta Pruning
```cpp
class MinimaxAI : public AIStrategy {
private:
    int maxDepth;
    HeuristicEvaluator evaluator;
    
    int minimax(GameState& state, int depth, int alpha, int beta, bool maximizing);
    std::vector<Move> orderMoves(const GameState& state, 
                                 const std::vector<Move>& moves);
    
public:
    MinimaxAI(int depth = 3);
    Move getBestMove(const GameState& state) override;
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
};
```

### Move Generation Optimization
```cpp
class MoveGenerator {
private:
    // Pre-computed direction vectors
    static const std::array<Position, 8> DIRECTIONS;
    
public:
    std::vector<Move> generateAllMoves(const GameState& state);
    
private:
    std::vector<Position> getReachablePositions(const Board& board,
                                                const Position& from);
    std::vector<Position> getShootablePositions(const Board& board,
                                                const Position& from);
};
```

## User Interface Specifications

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
    std::string getInput() override;
    
private:
    std::string cellToString(Board::Cell cell) const;
};
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
        SETTINGS,
        EXIT
    };
    
    void showMainMenu();
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
    "game_duration": "01:30:00"
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
    
private:
    static nlohmann::json gameStateToJson(const GameState& state);
    static GameState jsonToGameState(const nlohmann::json& j);
};
```

## Botzone Integration

### Competition Bot Interface
```cpp
class CompetitionBot {
private:
    AIStrategy* ai;
    Timer moveTimer;
    const int TIME_LIMIT_MS = 2000;
    
public:
    CompetitionBot(AIStrategy* aiStrategy);
    std::string getMove(const std::string& input);
    
private:
    GameState parseInput(const std::string& input);
    std::string formatOutput(const Move& move);
    bool isTimeCritical() const;
};
```

### Standard I/O Protocol
```
Input Format:
turn board
MOVE x1 y1 x2 y2 x3 y3

Output Format:
MOVE x1 y1 x2 y2 x3 y3

Where:
- turn: 0 for white, 1 for black
- board: 10x10 grid representation
- MOVE: Command keyword
- (x1, y1): Amazon starting position
- (x2, y2): Amazon destination
- (x3, y3): Arrow destination
```

## Performance Specifications

### Time Constraints
- **AI Move Decision**: ≤ 2 seconds per move (Botzone requirement)
- **Move Generation**: ≤ 100ms for all legal moves
- **State Evaluation**: ≤ 10ms per evaluation
- **Save/Load Operations**: ≤ 1 second for typical game state

### Memory Constraints
- **Game State**: ≤ 1 MB per state
- **Search Tree**: ≤ 100 MB maximum during AI search
- **Save Files**: ≤ 10 KB per saved game

### Computational Complexity
- **Move Generation**: O(n²) where n is board size (10x10)
- **Minimax Search**: O(b^d) where b is branching factor (~2000), d is depth
- **State Evaluation**: O(n²) for board evaluation

## Optimization Strategies

### Move Generation Optimization
1. **Pre-computed Directions**: Store direction vectors for queen moves
2. **Bitboard Representation**: Optional optimization for faster move generation
3. **Caching Legal Moves**: Cache legal moves for each position

### AI Search Optimization
1. **Alpha-Beta Pruning**: Reduce search space exponentially
2. **Move Ordering**: Try best moves first to improve pruning
3. **Transposition Tables**: Cache evaluation results for repeated positions
4. **Iterative Deepening**: Search incrementally deeper within time limits

### Memory Optimization
1. **Flyweight Pattern**: Share common data between game states
2. **Copy-on-Write**: Efficient state copying for search trees
3. **Pool Allocation**: Custom memory management for frequent allocations

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
```

### Recovery Strategies
1. **Invalid Moves**: Prompt user for valid input
2. **File Errors**: Provide fallback options and clear error messages
3. **AI Timeouts**: Return best move found within time limit
4. **Memory Exhaustion**: Graceful degradation of AI search depth

## Testing Specifications

### Unit Test Coverage
- **Board Class**: 100% coverage of public methods
- **GameState Class**: 100% coverage of state management
- **Move Validation**: All edge cases and invalid moves
- **AI Algorithms**: Correctness of search and evaluation

### Integration Test Scenarios
1. **Complete Game Flow**: Start to finish with various outcomes
2. **Save/Load Cycle**: Save game, load game, continue playing
3. **AI vs AI**: Automated games to test AI stability
4. **Botzone Compatibility**: Verify I/O protocol compliance

### Performance Test Benchmarks
1. **Move Generation Speed**: ≥ 1000 moves/second
2. **AI Decision Time**: ≤ 2 seconds for depth 3 search
3. **Memory Usage**: ≤ 100 MB peak during intensive search
4. **Save/Load Performance**: ≤ 1 second for 100-move game

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
- **Testing**: Google Test framework
- **Build**: CMake ≥ 3.16

## Security Considerations

### Input Validation
- Validate all user input before processing
- Sanitize file paths for save/load operations
- Limit AI search depth to prevent excessive resource usage

### Memory Safety
- Use smart pointers for dynamic memory management
- Implement bounds checking for all array accesses
- Validate serialized data before deserialization

### File Operations
- Restrict save file locations to user-writable directories
- Validate file formats before loading
- Implement backup mechanism for corrupted save files

This technical specification provides a comprehensive blueprint for implementing the Game of Amazons with clear performance targets, architectural guidelines, and quality standards.
