# Testing Strategy: King of the Amazons

## Overview
This document outlines the comprehensive testing strategy for the "King of the Amazons" project, covering unit testing, integration testing, system testing, and performance testing.

## Testing Philosophy

### Test-Driven Development (TDD) Approach
1. **Write tests first**: Define expected behavior before implementation
2. **Red-Green-Refactor**: Fail, pass, improve cycle
3. **Continuous testing**: Automated tests on every commit
4. **Regression prevention**: Tests catch breaking changes early

### Testing Pyramid
```
        System Tests (10%)
           /      \
          /        \
Integration Tests (20%)
          \        /
           \      /
        Unit Tests (70%)
```

## Unit Testing

### Testing Framework: Google Test
```cpp
#include <gtest/gtest.h>

TEST(BoardTest, Initialization) {
    Board board;
    board.initializeStandardPosition();
    
    // Test initial Amazon positions
    EXPECT_EQ(board.getCell(0, 3), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(3, 0), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(6, 0), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(9, 3), Board::Cell::BLACK_AMAZON);
}

TEST(MoveValidationTest, ValidMove) {
    GameState state;
    Move move = { {0, 3}, {3, 6}, {5, 8} };
    
    EXPECT_TRUE(state.isValidMove(move));
}

TEST(MoveValidationTest, InvalidMoveThroughObstacle) {
    GameState state;
    // Place an arrow in the path
    state.getBoard().setCell(1, 4, Board::Cell::ARROW);
    Move move = { {0, 3}, {3, 6}, {5, 8} };
    
    EXPECT_FALSE(state.isValidMove(move));
}
```

### Core Component Test Coverage

#### Board Class Tests
- **Initialization**: Standard starting position
- **Boundary checking**: Valid/invalid positions
- **Cell operations**: Get/set cell values
- **Move generation**: Legal moves from each position
- **Obstacle detection**: Path blocking by arrows/Amazons

#### GameState Class Tests
- **State transitions**: Move application and undo
- **Win condition detection**: No legal moves
- **Player switching**: Turn management
- **Move history**: Complete history tracking
- **Serialization**: Save/load consistency

#### Move Validation Tests
- **Basic validation**: Amazon movement rules
- **Arrow shooting**: Valid shooting positions
- **Obstacle avoidance**: Cannot move through occupied squares
- **Boundary conditions**: Edge of board cases
- **Special cases**: Complex multi-step validations

#### AI Algorithm Tests
- **Move generation**: All legal moves produced
- **Heuristic evaluation**: Consistent scoring
- **Search algorithms**: Correct minimax results
- **Performance**: Within time constraints
- **Edge cases**: Endgame positions

### Test Data Management

#### Test Fixtures
```cpp
class BoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        board.initializeStandardPosition();
    }
    
    void TearDown() override {
        // Cleanup if needed
    }
    
    Board board;
};

TEST_F(BoardTest, LegalMovesFromStartingPosition) {
    auto moves = board.getLegalMoves({0, 3});
    EXPECT_GT(moves.size(), 0);
    
    // Specific expected moves
    EXPECT_TRUE(std::find(moves.begin(), moves.end(), 
                         Position{1, 4}) != moves.end());
}
```

#### Test Data Files
```
tests/data/
├── board_positions/
│   ├── standard_start.txt
│   ├── midgame_1.txt
│   └── endgame_1.txt
├── move_sequences/
│   ├── valid_moves.txt
│   ├── invalid_moves.txt
│   └── winning_sequence.txt
└── saved_games/
    ├── simple_game.json
    ├── complex_game.json
    └── corrupted_game.json
```

## Integration Testing

### Component Integration Tests

#### Game Flow Integration
```cpp
TEST(GameFlowIntegration, CompleteGame) {
    GameController controller;
    TextDisplay display;
    HumanPlayer player1;
    HumanPlayer player2;
    
    controller.startNewGame(player1, player2);
    
    // Simulate a complete game
    while (!controller.isGameOver()) {
        Move move = controller.getCurrentPlayer().getMove();
        EXPECT_TRUE(controller.makeMove(move));
    }
    
    EXPECT_TRUE(controller.getWinner().has_value());
}
```

#### Save/Load Integration
```cpp
TEST(SaveLoadIntegration, CompleteCycle) {
    GameState originalState;
    // Make some moves
    originalState.makeMove({{0, 3}, {3, 6}, {5, 8}});
    originalState.makeMove({{9, 3}, {6, 0}, {4, 2}});
    
    // Save to file
    Serializer::saveToFile(originalState, "test_save.json");
    
    // Load from file
    GameState loadedState = Serializer::loadFromFile("test_save.json");
    
    // Verify equivalence
    EXPECT_EQ(originalState.getBoard(), loadedState.getBoard());
    EXPECT_EQ(originalState.getCurrentPlayer(), 
              loadedState.getCurrentPlayer());
    EXPECT_EQ(originalState.getMoveHistory().size(),
              loadedState.getMoveHistory().size());
}
```

#### AI Integration
```cpp
TEST(AIIntegration, CompetentPlay) {
    GameState state;
    MinimaxAI ai(3); // Depth 3 search
    
    // AI should make a legal move
    Move aiMove = ai.getBestMove(state);
    EXPECT_TRUE(state.isValidMove(aiMove));
    
    // Move should be made within time limit
    auto start = std::chrono::high_resolution_clock::now();
    ai.getBestMove(state);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start);
    EXPECT_LT(duration.count(), 2000); // 2 second limit
}
```

### Interface Integration Tests

#### Menu System Integration
```cpp
TEST(MenuIntegration, Navigation) {
    MenuController menu;
    MockDisplay display;
    
    // Test menu navigation
    menu.showMainMenu();
    EXPECT_TRUE(display.lastShown().contains("New Game"));
    
    menu.handleSelection(MenuController::MenuOption::NEW_GAME);
    EXPECT_TRUE(gameStarted);
}
```

#### Botzone Interface Integration
```cpp
TEST(BotzoneIntegration, ProtocolCompliance) {
    CompetitionBot bot(new MinimaxAI(2));
    
    // Test standard input format
    std::string input = "0\n.........\n.........\n...W.....\n";
    std::string output = bot.getMove(input);
    
    // Output should match expected format
    EXPECT_TRUE(output.starts_with("MOVE"));
    
    // Parse and validate the move
    Move move = Move::fromString(output.substr(5));
    EXPECT_TRUE(move.isValid());
}
```

## System Testing

### End-to-End Test Scenarios

#### Scenario 1: Complete Human vs Human Game
```
1. Start new game
2. Play legal moves alternately
3. Save game mid-way
4. Load saved game
5. Continue to completion
6. Verify winner determination
```

#### Scenario 2: Human vs AI Game
```
1. Start new game with AI opponent
2. Human makes legal moves
3. AI responds within time limit
4. Game proceeds to completion
5. Verify game state consistency
```

#### Scenario 3: Save/Load Reliability
```
1. Start game and make several moves
2. Save game to file
3. Load game from file
4. Verify identical game state
5. Continue playing
6. Save again and verify consistency
```

#### Scenario 4: Botzone Competition Simulation
```
1. Initialize competition bot
2. Feed standard input format
3. Receive move output
4. Verify output format compliance
5. Repeat for multiple game states
```

### User Interface System Tests

#### Text Interface Tests
```cpp
TEST(TextInterfaceSystem, CompleteInteraction) {
    TextDisplay display;
    GameController controller;
    
    // Test board display
    display.showBoard(controller.getBoard());
    EXPECT_TRUE(captureOutput().contains("W") || 
                captureOutput().contains("B"));
    
    // Test message display
    display.showMessage("Test message");
    EXPECT_TRUE(captureOutput().contains("Test message"));
    
    // Test menu display
    display.showMenu({"Option 1", "Option 2"});
    EXPECT_TRUE(captureOutput().contains("Option 1"));
}
```

#### Error Handling System Tests
```cpp
TEST(ErrorHandlingSystem, InvalidInputRecovery) {
    GameController controller;
    MockInputHandler inputHandler;
    
    // Provide invalid move
    inputHandler.setNextInput("invalid move format");
    controller.processTurn();
    
    // System should recover and prompt again
    EXPECT_TRUE(inputHandler.wasCalledAgain());
    EXPECT_TRUE(controller.isWaitingForInput());
}
```

## Performance Testing

### Benchmark Tests

#### Move Generation Performance
```cpp
TEST(PerformanceBenchmark, MoveGeneration) {
    GameState state;
    auto start = std::chrono::high_resolution_clock::now();
    
    const int ITERATIONS = 1000;
    for (int i = 0; i < ITERATIONS; ++i) {
        auto moves = state.getLegalMoves();
        // Just generate, don't process
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start);
    
    // Should generate moves at ~1000 moves/second
    EXPECT_LT(duration.count(), ITERATIONS); // < 1ms per generation
}
```

#### AI Search Performance
```cpp
TEST(PerformanceBenchmark, AISearch) {
    GameState state;
    MinimaxAI ai(3); // Depth 3
    
    auto start = std::chrono::high_resolution_clock::now();
    Move bestMove = ai.getBestMove(state);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start);
    
    // Must complete within Botzone time limit
    EXPECT_LT(duration.count(), 2000); // 2 seconds
    EXPECT_TRUE(bestMove.isValid());
}
```

#### Memory Usage Tests
```cpp
TEST(PerformanceBenchmark, MemoryUsage) {
    // Test deep search memory consumption
    MinimaxAI ai(4); // Depth 4 for more memory usage
    
    auto memoryBefore = getCurrentMemoryUsage();
    ai.getBestMove(complexGameState);
    auto memoryAfter = getCurrentMemoryUsage();
    
    auto memoryIncrease = memoryAfter - memoryBefore;
    EXPECT_LT(memoryIncrease, 100 * 1024 * 1024); // < 100 MB
}
```

### Load Testing

#### Concurrent Game Sessions
```cpp
TEST(LoadTest, MultipleGames) {
    const int CONCURRENT_GAMES = 10;
    std::vector<std::thread> threads;
    std::atomic<int> completedGames{0};
    
    for (int i = 0; i < CONCURRENT_GAMES; ++i) {
        threads.emplace_back([&completedGames]() {
            GameController controller;
            // Play a quick game
            while (!controller.isGameOver()) {
                controller.makeRandomMove();
            }
            ++completedGames;
        });
    }
    
    // Wait for all games with timeout
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(completedGames, CONCURRENT_GAMES);
}
```

## Test Automation

### Continuous Integration Pipeline

#### GitHub Actions Configuration
```yaml
name: Test Suite
on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake g++ libgtest-dev
    
    - name: Configure CMake
      run: cmake -B build -DCMAKE_BUILD_TYPE=Debug
    
    - name: Build
      run: cmake --build build --config Debug
    
    - name: Run unit tests
      run: ./build/tests/unit_tests
    
    - name: Run integration tests
      run: ./build/tests/integration_tests
    
    - name: Run performance tests
      run: ./build/tests/performance_tests
    
    - name: Generate coverage report
      run: |
        cd build
        make coverage
```

### Test Coverage Reporting

#### Coverage Targets
- **Line Coverage**: ≥ 80% overall
- **Branch Coverage**: ≥ 70% for critical paths
- **Function Coverage**: ≥ 90% for public APIs

#### Coverage Tools
- **gcov/lcov**: Line coverage analysis
- **Google Test**: Test execution framework
- **CMake**: Integration with coverage flags

## Quality Gates

### Pre-commit Checks
1. **Code formatting**: clang-format compliance
2. **Static analysis**: clang-tidy checks
3. **Unit tests**: All unit tests must pass
4. **Build verification**: Successful compilation

### Pre-release Checks
1. **Integration tests**: All integration tests pass
2. **System tests**: End-to-end scenarios successful
3. **Performance tests**: Meet all performance targets
4. **Memory tests**: No leaks, within limits

### Acceptance Criteria
1. **Functional correctness**: All requirements implemented
2. **Performance compliance**: Within Botzone time limits
3. **Reliability**: No crashes in extended testing
4. **Usability**: Intuitive interface and error handling

## Test Environment

### Development Environment
- **OS**: Ubuntu 20.04 / macOS / Windows
- **Compiler**: GCC 9+, Clang 10+, MSVC 2019+
- **Tools**: CMake 3.16+, Google Test, gcov/lcov

### Test Data Generation
```cpp
// Automated test data generation
void generateTestGames(int count) {
    for (int i = 0; i < count; ++i) {
        GameState state;
        std::vector<Move> moves;
        
        while (!state.isGameOver()) {
            auto legalMoves = state.getLegalMoves();
            if (legalMoves.empty()) break;
            
            Move move = selectRandomMove(legalMoves);
            state.makeMove(move);
            moves.push_back(move);
        }
        
        saveGameToFile(state, moves, 
                      fmt::format("test_game_{}.json", i));
    }
}
```

## Bug Tracking and Regression Testing

### Test Case Management
1. **Bug reproduction**: Test cases for every reported bug
2. **Regression suite**: Comprehensive test collection
3. **Priority classification**: Critical path tests run first
4. **Automated execution**: Scheduled test runs

### Test Maintenance
1. **Test updates**: Keep tests synchronized with code changes
2. **Test refactoring**: Improve test clarity and maintainability
3. **Coverage analysis**: Identify untested code paths
4. **Performance monitoring**: Track test execution times

This testing strategy ensures comprehensive validation of the Game of Amazons implementation, from individual components to complete system behavior, with a focus on reliability, performance, and maintainability.
