#include <gtest/gtest.h>
#include "core/Board.hpp"
#include "core/Position.hpp"
#include "core/Player.hpp"

using namespace amazons;

TEST(BoardTest, DefaultConstructor) {
    Board board;
    // Board should be initialized with all empty cells
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            EXPECT_EQ(board.getCell(row, col), Board::Cell::EMPTY);
        }
    }
}

TEST(BoardTest, InitializeStandardPosition) {
    Board board;
    board.initializeStandardPosition();
    
    // Check white Amazons positions (bottom - rows 6-9)
    EXPECT_EQ(board.getCell(6, 0), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(9, 3), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(9, 6), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(6, 9), Board::Cell::WHITE_AMAZON);
    
    // Check black Amazons positions (top - rows 0-3)
    EXPECT_EQ(board.getCell(0, 3), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(0, 6), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(3, 0), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(3, 9), Board::Cell::BLACK_AMAZON);
    
    // Check some empty positions
    EXPECT_EQ(board.getCell(0, 0), Board::Cell::EMPTY);
    EXPECT_EQ(board.getCell(5, 5), Board::Cell::EMPTY);
    EXPECT_EQ(board.getCell(9, 9), Board::Cell::EMPTY);
}

TEST(BoardTest, IsValidPosition) {
    Board board;
    
    // Valid positions
    EXPECT_TRUE(board.isValidPosition(0, 0));
    EXPECT_TRUE(board.isValidPosition(9, 9));
    EXPECT_TRUE(board.isValidPosition(5, 5));
    
    // Invalid positions
    EXPECT_FALSE(board.isValidPosition(-1, 0));
    EXPECT_FALSE(board.isValidPosition(0, -1));
    EXPECT_FALSE(board.isValidPosition(10, 5));
    EXPECT_FALSE(board.isValidPosition(5, 10));
}

TEST(BoardTest, SetAndGetCell) {
    Board board;
    
    // Set some cells
    board.setCell(0, 0, Board::Cell::WHITE_AMAZON);
    board.setCell(5, 5, Board::Cell::BLACK_AMAZON);
    board.setCell(9, 9, Board::Cell::ARROW);
    
    // Verify
    EXPECT_EQ(board.getCell(0, 0), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(5, 5), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(9, 9), Board::Cell::ARROW);
    
    // Overwrite
    board.setCell(5, 5, Board::Cell::EMPTY);
    EXPECT_EQ(board.getCell(5, 5), Board::Cell::EMPTY);
}

TEST(BoardTest, GetLegalMovesEmptyBoard) {
    Board board;
    // Place a white Amazon at center
    board.setCell(5, 5, Board::Cell::WHITE_AMAZON);
    
    Position from(5, 5);
    auto moves = board.getLegalMoves(from);
    
    // Queen on empty 10x10 board from center should have many moves
    // 7 moves in each of 8 directions = 56 moves
    EXPECT_GT(moves.size(), 0);
    
    // Check some specific moves
    bool foundRight = false;
    bool foundDown = false;
    bool foundDiagonal = false;
    
    for (const auto& move : moves) {
        if (move.row == 5 && move.col == 9) foundRight = true; // Right edge
        if (move.row == 9 && move.col == 5) foundDown = true;  // Bottom edge
        if (move.row == 9 && move.col == 9) foundDiagonal = true; // Bottom-right corner
    }
    
    EXPECT_TRUE(foundRight);
    EXPECT_TRUE(foundDown);
    EXPECT_TRUE(foundDiagonal);
}

TEST(BoardTest, GetLegalMovesWithObstacles) {
    Board board;
    board.initializeStandardPosition();
    
    // From white Amazon at (6,0)
    Position from(6, 0);
    auto moves = board.getLegalMoves(from);
    
    // Should have moves but not through other pieces
    EXPECT_GT(moves.size(), 0);
    
    // Should not be able to move to occupied squares
    for (const auto& move : moves) {
        EXPECT_EQ(board.getCell(move.row, move.col), Board::Cell::EMPTY);
    }
}

TEST(BoardTest, CountReachableSquares) {
    Board board;
    board.initializeStandardPosition();
    
    // In initial position, each Amazon should have some reachable squares
    // This is a basic test - exact count depends on implementation
    int whiteReachable = board.countReachableSquares(Player::WHITE);
    int blackReachable = board.countReachableSquares(Player::BLACK);
    
    EXPECT_GT(whiteReachable, 0);
    EXPECT_GT(blackReachable, 0);
    // Both players should have reasonable number of reachable squares
    // (Not testing for equality since positions may not be perfectly symmetric)
}
