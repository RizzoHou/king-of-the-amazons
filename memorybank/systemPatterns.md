# System Patterns: King of the Amazons

## Architecture Overview
The Game of Amazons implementation follows a **Model-View-Controller (MVC)** architecture pattern to separate concerns and enable modular development:

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│     VIEW        │    │   CONTROLLER    │    │     MODEL       │
│                 │◄──►│                 │◄──►│                 │
│ - Display       │    │ - Input         │    │ - Game State    │
│ - User Interface│    │ - Game Flow     │    │ - Rules Engine  │
│ - Menu System   │    │ - AI Interface  │    │ - Move Validation│
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## Core Components

### 1. Game Model
**Responsibilities**:
- Maintain game state (board configuration, player positions, turn information)
- Enforce game rules and validate moves
- Calculate legal moves for Amazons pieces
- Manage game termination conditions

**Key Classes**:
- `GameState`: Complete representation of game state
- `Board`: 10x10 board representation with piece placement
- `Move`: Representation of a complete move (Amazon movement + arrow shot)
- `Player`: Player information and piece management

### 2. Controller Layer
**Responsibilities**:
- Coordinate between model and view
- Handle user input and translate to game actions
- Manage game flow (menu navigation, game setup, gameplay)
- Interface with AI system for computer moves

**Key Classes**:
- `GameController`: Main game flow controller
- `MenuController`: Menu system navigation
- `AIController`: Bridge between game logic and AI algorithms

### 3. View Layer
**Responsibilities**:
- Display game board and pieces
- Render menu system and user interface
- Show game status and messages
- Handle different display modes (text-based vs graphical)

**Key Classes**:
- `Display`: Abstract display interface
- `TextDisplay`: Character-based console display
- `GraphicalDisplay`: Optional graphical interface (if implemented)

## Design Patterns in Use

### 1. **Factory Pattern** for Display Creation
```cpp
Display* DisplayFactory::createDisplay(DisplayType type) {
    switch(type) {
        case DisplayType::TEXT: return new TextDisplay();
        case DisplayType::GRAPHICAL: return new GraphicalDisplay();
        default: return new TextDisplay();
    }
}
```

### 2. **Strategy Pattern** for AI Algorithms
```cpp
class AIStrategy {
public:
    virtual Move getBestMove(const GameState& state) = 0;
};

class MinimaxAI : public AIStrategy { /* ... */ };
class MonteCarloAI : public AIStrategy { /* ... */ };
```

### 3. **Command Pattern** for Move Execution
```cpp
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class MoveCommand : public Command {
    GameState& state;
    Move move;
    // Implementation...
};
```

### 4. **Observer Pattern** for State Updates
```cpp
class GameStateObserver {
public:
    virtual void onStateChanged(const GameState& newState) = 0;
};
```

## Data Flow Patterns

### Game Initialization
```
User → MenuController → GameController → GameState → Display
```

### Move Execution  
```
User Input → GameController → Move Validation → State Update → Display Update
```

### AI Move Generation
```
GameController → AIController → AIStrategy → Move Selection → GameController
```

### Save/Load Operations
```
GameController → Serializer → File System → Deserializer → GameController
```

## Critical Implementation Paths

### 1. Move Validation Sequence
```
1. Check if source position contains current player's Amazon
2. Verify move direction is valid (8 directions, any distance)
3. Ensure path is clear (no pieces or obstacles)
4. Validate arrow shot from new position
5. Verify arrow path is clear and lands on empty square
6. Update board with Amazon movement and arrow placement
```

### 2. AI Decision Making
```
1. Generate all legal moves for current position
2. Apply heuristic evaluation to each possible move
3. Use search algorithm (Minimax/Alpha-Beta) to look ahead
4. Select move with best evaluation score
5. Return move within time constraints
```

### 3. Game State Serialization
```
1. Capture board configuration (piece positions)
2. Serialize player information and turn order
3. Include game history for undo functionality
4. Write to file in human-readable format (JSON/XML) or binary
```

## Component Relationships

### Dependency Graph
```
GameController ────┬─── GameState
                   ├─── Display
                   ├─── AIController
                   └─── MenuController

AIController ──────┴─── AIStrategy

Display ──────────┬─── TextDisplay
                  └─── GraphicalDisplay (optional)
```

### Communication Protocols
- **Controller ↔ Model**: Direct method calls for state manipulation
- **Controller ↔ View**: Observer pattern for display updates
- **AI ↔ Game Logic**: Strategy pattern for interchangeable algorithms

## Enhanced Features Architecture

### 1. Undo/Redo System
**Pattern**: Command Pattern with History Stack
```cpp
class UndoManager {
    std::stack<GameState> undoStack;
    std::stack<GameState> redoStack;
    
    void recordState(const GameState& state);
    GameState undo(const GameState& current);
    GameState redo(const GameState& current);
};
```

### 2. Game Analysis System
**Pattern**: Strategy Pattern for Analysis Algorithms
```cpp
class GameAnalyzer {
public:
    virtual AnalysisResult analyzePosition(const GameState& state) = 0;
};

class TerritoryAnalyzer : public GameAnalyzer { /* Territory calculation */ };
class ThreatAnalyzer : public GameAnalyzer { /* Threat detection */ };
```

### 3. Statistics Tracking
**Pattern**: Observer Pattern for Game Events
```cpp
class StatisticsTracker : public GameStateObserver {
    GameStatistics stats;
    
    void onStateChanged(const GameState& newState) override;
    void onGameEnd(const GameState& finalState) override;
};
```

### 4. Replay System
**Pattern**: Memento Pattern for Game State Capture
```cpp
class ReplaySystem {
    std::vector<GameState> gameStates;
    size_t currentPosition;
    
    void recordState(const GameState& state);
    GameState previous();
    GameState next();
    GameState jumpTo(size_t position);
};
```

## Performance Considerations
1. **Move Generation**: Pre-compute legal moves where possible
2. **State Evaluation**: Cache evaluation results for common positions
3. **AI Search**: Implement pruning and move ordering optimizations
4. **Memory Usage**: Use efficient data structures for board representation
5. **Enhanced Features Memory**: Manage undo history and replay storage efficiently
