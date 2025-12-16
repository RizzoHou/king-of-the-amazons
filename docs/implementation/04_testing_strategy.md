# Testing Strategy: King of the Amazons

## Overview
This document outlines the comprehensive testing strategy for the "King of the Amazons" project, covering unit testing, integration testing, system testing, and performance testing, with a focus on the enhanced features and realistic development timeline.

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

TEST(UndoManagerTest, BasicUndoRedo) {
    UndoManager manager;
    GameState state1, state2;
    state2.makeMove({{0, 3}, {3, 6}, {5, 8}});
    
    manager.recordState(state1);
    manager.recordState(state2);
    
    GameState undone = manager.undo(state2);
    EXPECT_EQ(undone.getMoveCount(), 0);
    
    GameState redone = manager.redo(undone);
    EXPECT_EQ(redone.getMoveCount(), 1);
}
```

### Core Component Test Coverage

#### Board Class Tests
- **Initialization**: Standard starting position
- **Boundary checking**: Valid/invalid positions
- **Cell operations**: Get/set cell values
- **Move generation**: Legal moves from each position
- **Obstacle detection**: Path blocking by arrows/Amazons
- **Territory calculation**: Reachable squares count

#### GameState Class Tests
- **State transitions**: Move application and undo
- **Win condition detection**: No legal moves
- **Player switching**: Turn management
- **Move history**: Complete history tracking
- **Serialization**: Save/load consistency
- **Status tracking**: Game progress monitoring

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
- **Transposition tables**: Caching correctness

#### Enhanced Features Tests
- **Undo/Redo system**: State restoration correctness
- **Game analysis**: Accurate position evaluation
- **Statistics tracking**: Data collection and reporting
- **Replay system**: Game recording and playback

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

class GameStateTest : public ::testing::Test {
protected:
    void SetUp() override {
        state = GameState(); // Fresh game state
    }
    
    GameState state;
};

TEST_F(GameStateTest, UndoFunctionality) {
    Move move = {{0, 3}, {3, 6}, {5, 8}};
    EXPECT_TRUE(state.makeMove(move));
    EXPECT_EQ(state.getMoveCount(), 1);
    
    EXPECT_TRUE(state.undoMove());
    EXPECT_EQ(state.getMoveCount(), 0);
}
```

#### Test Data Files
```
tests/data/
├── board_positions/
│   ├── standard_start.txt
│   ├── midgame_1.txt
│   ├── midgame_2.txt
│   └── endgame_1.txt
├── move_sequences/
│   ├── valid_moves.txt
│   ├── invalid_moves.txt
│   ├── winning_sequence.txt
│   └── complex_sequence.txt
├── saved_games/
│   ├── simple_game.json
│   ├── complex_game.json
│   ├── endgame.json
│   └── corrupted_game.json (for error handling tests)
└── analysis_cases/
    ├── balanced_position.txt
    ├── white_advantage.txt
    ├── black_advantage.txt
    └── critical_position.txt
```

## Integration Testing

### Component Integration Tests

#### Game Flow Integration
```cpp
TEST(GameFlowIntegration, CompleteGameWithUndo) {
    GameController controller;
    TextDisplay display;
    HumanPlayer player1;
    HumanPlayer player2;
    
    controller.startNewGame(player1, player2);
    
    // Make a move
    Move move1 = {{0, 3}, {3, 6}, {5, 8}};
    EXPECT_TRUE(controller.makeMove(move1));
    
    // Undo the move
    EXPECT_TRUE(controller.undoMove());
    EXPECT_EQ(controller.getGameState().getMoveCount(), 0);
    
    // Make a different move
    Move move2 = {{0, 3}, {2, 5}, {4, 7}};
    EXPECT_TRUE(controller.makeMove(move2));
    
    // Complete the game
    while (!controller.isGameOver()) {
        Move move = controller.getCurrentPlayer().getMove();
        EXPECT_TRUE(controller.makeMove(move));
    }
    
    EXPECT_TRUE(controller.getWinner().has_value());
}
```

#### Save/Load Integration with Features
```cpp
TEST(SaveLoadIntegration, CompleteCycleWithHistory) {
    GameState originalState;
    // Make several moves
    originalState.makeMove({{0, 3}, {3, 6}, {5, 8}});
    originalState.makeMove({{9, 3}, {6, 0}, {4, 2}});
    originalState.makeMove({{3, 0}, {5, 2}, {7, 4}});
    
    // Save to file
    Serializer::saveToFile(originalState, "test_save_with_history.json");
    
    // Load from file
    GameState loadedState = Serializer::loadFromFile("test_save_with_history.json");
    
    // Verify equivalence including move history
    EXPECT_EQ(originalState.getBoard(), loadedState.getBoard());
    EXPECT_EQ(originalState.getCurrentPlayer(), 
              loadedState.getCurrentPlayer());
    EXPECT_EQ(originalState.getMoveHistory().size(),
              loadedState.getMoveHistory().size());
    
    // Verify move history content
    for (size_t i = 0; i < originalState.getMoveHistory().size(); ++i) {
        EXPECT_EQ(originalState.getMoveHistory()[i],
                  loadedState.getMoveHistory()[i]);
    }
}
```

#### AI Integration with Enhanced Features
```cpp
TEST(AIIntegration, AnalysisFeature) {
    GameState state;
    MinimaxAI ai(3); // Depth 3 search
    GameAnalyzer analyzer;
    
    // AI should make a legal move
    Move aiMove = ai.getBestMove(state);
    EXPECT_TRUE(state.isValidMove(aiMove));
    
    // Analyze the position
    auto analysis = analyzer.analyzePosition(state);
    EXPECT_GE(analysis.whiteTerritory, 0);
    EXPECT_GE(analysis.blackTerritory, 0);
    EXPECT_LE(std::abs(analysis.whiteAdvantage), 1.0);
    
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

#### Menu System Integration with Features
```cpp
TEST(MenuIntegration, EnhancedNavigation) {
    MenuController menu;
    MockDisplay display;
    GameController gameController;
    
    // Test main menu navigation
    menu.showMainMenu();
    EXPECT_TRUE(display.lastShown().contains("New Game"));
    EXPECT_TRUE(display.lastShown().contains("Load Game"));
    EXPECT_TRUE(display.lastShown().contains("Statistics"));
    
    // Test game menu with enhanced features
    menu.showGameMenu();
    EXPECT_TRUE(display.lastShown().contains("Undo Move"));
    EXPECT_TRUE(display.lastShown().contains("Show Analysis"));
    EXPECT_TRUE(display.lastShown().contains("Save Game"));
    
    // Test feature actions
    menu.handleSelection(MenuController::MenuOption::SHOW_ANALYSIS);
    EXPECT_TRUE(analysisDisplayed);
}
```

#### Feature Integration Tests
```cpp
TEST(FeatureIntegration, StatisticsTracking) {
    Statistics stats;
    GameState state;
    
    // Record a complete game
    state.makeMove({{0, 3}, {3, 6}, {5, 8}});
    state.makeMove({{9, 3}, {6, 0}, {4, 2}});
    // ... more moves until game ends
    state.setStatus(GameState::GameStatus::WHITE_WIN);
    
    stats.recordGame(state);
    
    auto recordedStats = stats.getStatistics();
    EXPECT_EQ(recordedStats.gamesPlayed, 1);
    EXPECT_EQ(recordedStats.whiteWins, 1);
    EXPECT_GT(recordedStats.averageMovesPerGame, 0);
}

TEST(FeatureIntegration, ReplaySystem) {
    ReplaySystem replay;
    std::vector<GameState> gameStates;
    
    // Create a sequence of game states
    GameState state1, state2, state3;
    state2.makeMove({{0, 3}, {3, 6}, {5, 8}});
    state3.makeMove({{9, 3}, {6, 0}, {4, 2}});
    state3.makeMove({{3, 0}, {5, 2}, {7, 4}});
    
    gameStates = {state1, state2, state3};
    replay.recordGame(gameStates);
    
    // Test replay navigation
    EXPECT_TRUE(replay.hasPrevious());
    EXPECT_FALSE(replay.hasNext()); // At end
    
    GameState prevState = replay.previous();
    EXPECT_EQ(prevState.getMoveCount(), 1);
    
    GameState nextState = replay.next();
    EXPECT_EQ(nextState.getMoveCount(), 2);
}
```

## System Testing

### End-to-End Test Scenarios

#### Scenario 1: Complete Game with All Features
```
1. Start new game
2. Play several moves with undo/redo testing
3. Save game mid-way
4. Load saved game
5. Use analysis feature to evaluate position
6. Continue to completion
7. View statistics and replay
8. Verify all features work together seamlessly
```

#### Scenario 2: Human vs AI Game with Analysis
```
1. Start new game with AI opponent
2. Human makes legal moves
3. AI responds within time limit
4. Use analysis feature during game
5. Test undo/redo during AI thinking
6. Game proceeds to completion
7. Verify game state consistency and statistics
```

#### Scenario 3: Comprehensive Save/Load Testing
```
1. Start game and make several moves
2. Test undo/redo functionality
3. Save game to file
4. Load game from file
5. Verify identical game state including history
6. Continue playing
7. Save again and verify consistency
8. Test error handling with corrupted files
```

#### Scenario 4: Extended Usage Testing
```
1. Play multiple games consecutively
2. Test statistics accumulation
3. Use replay system for previous games
4. Test analysis on various positions
5. Verify memory usage remains stable
6. Test interface responsiveness throughout
```

### User Interface System Tests

#### Text Interface Tests with Enhanced Features
```cpp
TEST(TextInterfaceSystem, CompleteInteractionWithFeatures) {
    TextDisplay display;
    GameController controller;
    GameAnalyzer analyzer;
    
    // Test board display with coordinates
    display.showBoard(controller.getBoard());
    EXPECT_TRUE(captureOutput().contains("W") || 
                captureOutput().contains("B"));
    EXPECT_TRUE(captureOutput().contains("0")); // Coordinate
    
    // Test analysis display
    auto analysis = analyzer.analyzePosition(controller.getGameState());
    display.showAnalysis(analysis);
    EXPECT_TRUE(captureOutput().contains("Territory"));
    EXPECT_TRUE(captureOutput().contains("Advantage"));
    
    // Test game status display
    display.showGameStatus(controller.getGameState());
    EXPECT_TRUE(captureOutput().contains("Player"));
    EXPECT_TRUE(captureOutput().contains("Moves"));
    
    // Test enhanced menu display
    display.showMenu({"New Game", "Undo", "Analysis", "Statistics", "Exit"});
    EXPECT_TRUE(captureOutput().contains("Analysis"));
    EXPECT_TRUE(captureOutput().contains("Statistics"));
}
```

#### Error Handling System Tests
```cpp
TEST(ErrorHandlingSystem, ComprehensiveErrorRecovery) {
    GameController controller;
    MockInputHandler inputHandler;
    
    // Test invalid move input
    inputHandler.setNextInput("invalid move format");
    controller.processTurn();
    EXPECT_TRUE(inputHandler.wasCalledAgain());
    EXPECT_TRUE(controller.isWaitingForInput());
    
    // Test undo when no moves to undo
    EXPECT_FALSE(controller.undoMove());
    EXPECT_TRUE(errorMessageDisplayed.contains("No moves to undo"));
    
    // Test file operation errors
    EXPECT_FALSE(controller.saveGame("/invalid/path/game.json"));
    EXPECT_TRUE(errorMessageDisplayed.contains("Cannot save"));
    
    // Test AI timeout recovery
    SlowAI slowAI(10); // AI that takes 10 seconds
    controller.setAI(&slowAI);
    Move move = controller.getAIMove();
    EXPECT_TRUE(move.isValid()); // Should return something even on timeout
    EXPECT_TRUE(warningMessageDisplayed.contains("time limit"));
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

#### AI Search Performance with Enhanced Features
```cpp
TEST(PerformanceBenchmark, AISearchWithAnalysis) {
    GameState state;
    MinimaxAI ai(3); // Depth 3
    GameAnalyzer analyzer;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Test AI move selection
    Move bestMove = ai.getBestMove(state);
    
    // Test analysis generation
    auto analysis = analyzer.analyzePosition(state);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start);
    
    // Combined operations should complete within reasonable time
    EXPECT_LT(duration.count(), 3000); // 3 seconds for AI + analysis
    EXPECT_TRUE(bestMove.isValid());
    EXPECT_LE(std::abs(analysis.whiteAdvantage), 1.0);
}
```

#### Memory Usage Tests with History Features
```cpp
TEST(PerformanceBenchmark, MemoryUsageWithUndoHistory) {
    GameController controller;
    
    auto memoryBefore = getCurrentMemoryUsage();
    
    // Play a long game with many undo/redo operations
    for (int i = 0; i < 100; ++i) {
        controller.makeRandomMove();
        if (i % 10 == 0) {
            controller.undoMove();
            controller.redoMove();
        }
    }
    
    auto memoryAfter = getCurrentMemoryUsage();
    auto memoryIncrease = memoryAfter - memoryBefore;
    
    // Memory increase should be reasonable
    EXPECT_LT(memoryIncrease, 50 * 1024 * 1024); // < 50 MB
}
```

### Load Testing

#### Concurrent Feature Usage
```cpp
TEST(LoadTest, MultipleFeaturesSimultaneously) {
    const int CONCURRENT_OPERATIONS = 5;
    std::vector<std::thread> threads;
    std::atomic<int> completedOperations{0};
    
    for (int i = 0; i < CONCURRENT_OPERATIONS; ++i) {
        threads.emplace_back([&completedOperations, i]() {
            // Each thread tests different features
            switch (i % 4) {
                case 0: {
                    GameController controller;
                    controller.startNewGame();
                    for (int j = 0; j < 10; ++j) {
                        controller.makeRandomMove();
                    }
                    break;
                }
                case 1: {
                    GameAnalyzer analyzer;
                    GameState state;
                    auto analysis = analyzer.analyzePosition(state);
                    // Just analyze, don't use result
                    break;
                }
                case 2: {
                    Statistics stats;
                    GameState state;
                    stats.recordGame(state);
                    auto statData = stats.getStatistics();
                    break;
                }
                case 3: {
                    ReplaySystem replay;
                    std::vector<GameState> states(10);
                    replay.recordGame(states);
                    break;
                }
            }
            ++completedOperations;
        });
    }
    
    // Wait for all operations with timeout
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(completedOperations, CONCURRENT_OPERATIONS);
}
```

## Test Automation

### Continuous Integration Pipeline

#### GitHub Actions Configuration (To be implemented after local testing)
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
- **Enhanced Features Coverage**: ≥ 75% for undo/redo, analysis, statistics, replay

#### Coverage Tools
- **gcov/lcov**: Line coverage analysis
- **Google Test**: Test execution framework
- **CMake**: Integration with coverage flags
- **Manual Review**: For complex integration scenarios

## Quality Gates

### Pre-commit Checks
1. **Code formatting**: clang-format compliance
2. **Static analysis**: clang-tidy checks
3. **Unit tests**: All unit tests must pass
4. **Build verification**: Successful compilation
5. **Basic integration**: Core game flow tests pass

### Pre-release Checks
1. **Integration tests**: All integration tests pass
2. **System tests**: End-to-end scenarios successful
3. **Performance tests**: Meet all performance targets
4. **Memory tests**: No leaks, within limits
5. **Feature tests**: All enhanced features working correctly

### Acceptance Criteria
1. **Functional correctness**: All requirements implemented
2. **Performance compliance**: AI moves within 2 seconds, responsive UI
3. **Reliability**: No crashes in extended testing
4. **Usability**: Intuitive interface and error handling
5. **Enhanced features**: Undo/redo, analysis, statistics, replay all functional

## Test Environment

### Development Environment
- **OS**: Ubuntu 20.04 / macOS / Windows
- **Compiler**: GCC 9+, Clang 10+, MSVC 2019+
- **Tools**: CMake 3.16+, Google Test, gcov/lcov
- **Memory Tools**: Valgrind, AddressSanitizer

### Test Data Generation
```cpp
// Automated test data generation for various game states
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
        
        // Also save analysis data
        GameAnalyzer analyzer;
        auto analysis = analyzer.analyzePosition(state);
        saveAnalysisToFile(analysis, 
                          fmt::format("analysis_{}.json", i));
    }
}
```

## Bug Tracking and Regression Testing

### Test Case Management
1. **Bug reproduction**: Test cases for every reported bug
2. **Regression suite**: Comprehensive test collection
3. **Priority classification**: Critical path tests run first
4. **Automated execution**: Scheduled test runs
5. **Feature-specific tests**: Separate test suites for enhanced features

### Test Maintenance
1. **Test updates**: Keep tests synchronized with code changes
2. **Test refactoring**: Improve test clarity and maintainability
3. **Coverage analysis**: Identify untested code paths
4. **Performance monitoring**: Track test execution times
5. **Feature test evolution**: Update tests as features evolve

## Testing Timeline

### Week 1 Testing Focus
- **Unit tests**: Board, GameState, Move validation
- **Integration tests**: Basic game flow
- **Performance tests**: Move generation speed
- **Goal**: ≥ 50% coverage for core components

### Week 2 Testing Focus  
- **Unit tests**: Save/Load, Menu system, Basic AI
- **Integration tests**: Complete game flow, Save/Load cycle
- **System tests**: Human vs Human gameplay
- **Goal**: ≥ 70% coverage, all basic functionality tested

### Week 3 Testing Focus
- **Unit tests**: Advanced AI, Enhanced features
- **Integration tests**: AI vs Human, Feature integration
- **Performance tests**: AI decision time, memory usage
- **System tests**: Complete feature usage scenarios
- **Goal**: ≥ 80% coverage, all features tested

### Week 4 Testing Focus
- **System tests**: Extended usage, edge cases
- **Performance tests**: Load testing, stress testing
- **Regression tests**: Complete test suite execution
- **Acceptance testing**: Final verification against requirements
- **Goal**: ≥ 85% coverage, production-ready quality

## Success Metrics for Testing

### Quantitative Metrics
- **Test Coverage**: ≥ 80% line coverage
- **Test Passing Rate**: 100% for committed code
- **Performance Targets**: AI moves ≤ 2 seconds, generation ≤ 100ms
- **Memory Usage**: ≤ 100 MB peak, no leaks
- **Bug Density**: ≤ 1 critical bug per 1000 lines of code

### Qualitative Metrics
- **Test Comprehensiveness**: All requirements covered
- **Test Maintainability**: Clear, well-organized tests
- **Test Reliability**: Consistent, deterministic results
- **Test Documentation**: Clear purpose and methodology
- **Test Integration**: Seamless with development workflow

This testing strategy ensures comprehensive validation of the Game of Amazons implementation, from individual components to complete system behavior, with a focus on reliability, performance, and maintainability, including thorough testing of enhanced features for improved user experience.
