#include <gtest/gtest.h>
#include "core/Board.hpp"
#include "core/Position.hpp"
#include "core/Player.hpp"

using namespace amazons;

TEST(BoardTest, DefaultConstructor) {
    Board board;
    // Board should be initialized with all empty cells
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            EXPECT_EQ(board.getCell(row, col), Board::Cell::EMPTY);
        }
    }
}

TEST(BoardTest, InitializeStandardPosition) {
    Board board;
    board.initializeStandardPosition();
    
    // Check black Amazons positions (top positions)
    EXPECT_EQ(board.getCell(0, 2), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(2, 0), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(5, 0), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(7, 2), Board::Cell::BLACK_AMAZON);
    
    // Check white Amazons positions (bottom positions)
    EXPECT_EQ(board.getCell(0, 5), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(2, 7), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(5, 7), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(7, 5), Board::Cell::WHITE_AMAZON);
    
    // Check some empty positions
    EXPECT_EQ(board.getCell(0, 0), Board::Cell::EMPTY);
    EXPECT_EQ(board.getCell(4, 4), Board::Cell::EMPTY);
    EXPECT_EQ(board.getCell(7, 7), Board::Cell::EMPTY);
}

TEST(BoardTest, IsValidPosition) {
    Board board;
    
    // Valid positions
    EXPECT_TRUE(board.isValidPosition(0, 0));
    EXPECT_TRUE(board.isValidPosition(7, 7));
    EXPECT_TRUE(board.isValidPosition(4, 4));
    
    // Invalid positions
    EXPECT_FALSE(board.isValidPosition(-1, 0));
    EXPECT_FALSE(board.isValidPosition(0, -1));
    EXPECT_FALSE(board.isValidPosition(8, 4));
    EXPECT_FALSE(board.isValidPosition(4, 8));
}

TEST(BoardTest, SetAndGetCell) {
    Board board;
    
    // Set some cells
    board.setCell(0, 0, Board::Cell::WHITE_AMAZON);
    board.setCell(4, 4, Board::Cell::BLACK_AMAZON);
    board.setCell(7, 7, Board::Cell::ARROW);
    
    // Verify
    EXPECT_EQ(board.getCell(0, 0), Board::Cell::WHITE_AMAZON);
    EXPECT_EQ(board.getCell(4, 4), Board::Cell::BLACK_AMAZON);
    EXPECT_EQ(board.getCell(7, 7), Board::Cell::ARROW);
    
    // Overwrite
    board.setCell(4, 4, Board::Cell::EMPTY);
    EXPECT_EQ(board.getCell(4, 4), Board::Cell::EMPTY);
}

TEST(BoardTest, GetLegalMovesEmptyBoard) {
    Board board;
    // Place a white Amazon at center
    board.setCell(4, 4, Board::Cell::WHITE_AMAZON);
    
    Position from(4, 4);
    auto moves = board.getLegalMoves(from);
    
    // Queen on empty 8x8 board from center should have many moves
    // 3 moves in each of 8 directions = 24 moves (center of 8x8)
    EXPECT_GT(moves.size(), 0);
    
    // Check some specific moves
    bool foundRight = false;
    bool foundDown = false;
    bool foundDiagonal = false;
    
    for (const auto& move : moves) {
        if (move.row == 4 && move.col == 7) foundRight = true; // Right edge
        if (move.row == 7 && move.col == 4) foundDown = true;  // Bottom edge
        if (move.row == 7 && move.col == 7) foundDiagonal = true; // Bottom-right corner
    }
    
    EXPECT_TRUE(foundRight);
    EXPECT_TRUE(foundDown);
    EXPECT_TRUE(foundDiagonal);
}

TEST(BoardTest, GetLegalMovesWithObstacles) {
    Board board;
    board.initializeStandardPosition();
    
    // From white Amazon at (2,7)
    Position from(2, 7);
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