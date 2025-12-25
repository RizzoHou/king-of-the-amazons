#include <gtest/gtest.h>
#include "core/GameState.hpp"
#include "core/Position.hpp"
#include "core/Move.hpp"

using namespace amazons;

TEST(GameStateTest, DefaultConstructor) {
    GameState state;
    
    EXPECT_EQ(state.getCurrentPlayer(), Player::WHITE);
    EXPECT_EQ(state.getTurnNumber(), 1);
    EXPECT_FALSE(state.isGameOver());
}

TEST(GameStateTest, InitializeStandardGame) {
    GameState state;
    state.initializeStandardGame();
    
    EXPECT_EQ(state.getCurrentPlayer(), Player::WHITE);
    EXPECT_EQ(state.getTurnNumber(), 1);
    EXPECT_FALSE(state.isGameOver());
    
    // Board should be in standard position
    const Board& board = state.getBoard();
    // Check one white Amazon (actual position in 8x8 board)
    EXPECT_EQ(board.getCell(0, 5), Board::Cell::WHITE_AMAZON);
    // Check one black Amazon (actual position in 8x8 board)
    EXPECT_EQ(board.getCell(0, 2), Board::Cell::BLACK_AMAZON);
}

TEST(GameStateTest, GetLegalMovesInitialPosition) {
    GameState state;
    
    auto moves = state.getLegalMoves();
    // In initial position, white should have some legal moves
    EXPECT_GT(moves.size(), 0);
    
    // All moves should be valid
    for (const auto& move : moves) {
        EXPECT_TRUE(state.isValidMove(move));
    }
}

TEST(GameStateTest, IsValidMove) {
    GameState state;
    
    // Create a valid move (white's first move)
    auto moves = state.getLegalMoves();
    ASSERT_GT(moves.size(), 0);
    Move validMove = moves[0];
    
    EXPECT_TRUE(state.isValidMove(validMove));
    
    // Create invalid move with out of bounds positions
    Move invalidMove(Position(-1, 0), Position(0, 0), Position(0, 0));
    EXPECT_FALSE(state.isValidMove(invalidMove));
}

TEST(GameStateTest, MakeMove) {
    GameState state;
    
    // Get a legal move
    auto moves = state.getLegalMoves();
    ASSERT_GT(moves.size(), 0);
    Move move = moves[0];
    
    // Save initial state
    Player initialPlayer = state.getCurrentPlayer();
    Board initialBoard = state.getBoard();
    
    // Make the move
    state.makeMove(move);
    
    // Player should have switched
    EXPECT_EQ(state.getCurrentPlayer(), oppositePlayer(initialPlayer));
    
    // Turn number should still be 1 (black's turn now)
    EXPECT_EQ(state.getTurnNumber(), 1);
    
    // Board should be different
    EXPECT_NE(state.getBoard(), initialBoard);
    
    // Move should not be valid anymore (piece moved)
    EXPECT_FALSE(state.isValidMove(move));
}

TEST(GameStateTest, MakeInvalidMoveThrows) {
    GameState state;
    
    // Create an obviously invalid move
    Move invalidMove(Position(0, 0), Position(0, 0), Position(0, 0));
    
    EXPECT_THROW(state.makeMove(invalidMove), std::invalid_argument);
}

TEST(GameStateTest, UndoMove) {
    GameState state;
    
    // Get a legal move
    auto moves = state.getLegalMoves();
    ASSERT_GT(moves.size(), 0);
    Move move = moves[0];
    
    // Save initial state
    Board initialBoard = state.getBoard();
    Player initialPlayer = state.getCurrentPlayer();
    
    // Make the move
    state.makeMove(move);
    
    // Undo the move
    EXPECT_TRUE(state.canUndo());
    state.undoLastMove();
    
    // State should be back to initial
    EXPECT_EQ(state.getBoard(), initialBoard);
    EXPECT_EQ(state.getCurrentPlayer(), initialPlayer);
}

TEST(GameStateTest, GameOverDetection) {
    // This test is complex because we need to play a game to completion
    // For now, just test that initial position is not game over
    GameState state;
    EXPECT_FALSE(state.isGameOver());
    
    // Test that getWinner throws when game is not over
    EXPECT_THROW(state.getWinner(), std::logic_error);
}

TEST(GameStateTest, EqualityOperator) {
    GameState state1;
    GameState state2;
    
    EXPECT_TRUE(state1 == state2);
    
    // Make a move in state1
    auto moves = state1.getLegalMoves();
    if (!moves.empty()) {
        state1.makeMove(moves[0]);
        EXPECT_FALSE(state1 == state2);
    }
}