#include "ai/BasicAI.hpp"
#include <algorithm>
#include <random>
#include <chrono>

namespace amazons {

Move BasicAI::getBestMove(const GameState& gameState) const {
    auto legalMoves = gameState.getLegalMoves();
    if (legalMoves.empty()) {
        throw std::runtime_error("No legal moves available");
    }
    
    // Simple greedy AI: choose the move with highest heuristic value
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

Move BasicAI::getRandomMove(const GameState& gameState) const {
    auto legalMoves = gameState.getLegalMoves();
    if (legalMoves.empty()) {
        throw std::runtime_error("No legal moves available");
    }
    
    // Use current time as seed for random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, legalMoves.size() - 1);
    
    int randomIndex = distribution(generator);
    return legalMoves[randomIndex];
}

int BasicAI::evaluateMove(const GameState& gameState, const Move& move) const {
    // Simple heuristic: prefer moves that give us more mobility
    // and restrict opponent's mobility
    
    // Make a copy of the game state to simulate the move
    GameState simulatedState = gameState;
    simulatedState.makeMove(move);
    
    Player currentPlayer = gameState.getCurrentPlayer();
    Player opponent = (currentPlayer == Player::WHITE) ? Player::BLACK : Player::WHITE;
    
    // Calculate mobility difference
    int ourMobility = countAvailableMoves(simulatedState, currentPlayer);
    int opponentMobility = countAvailableMoves(simulatedState, opponent);
    
    // Prefer moves that increase our mobility and decrease opponent's mobility
    return ourMobility - opponentMobility;
}

int BasicAI::evaluateBoardForPlayer(const GameState& gameState, Player player) const {
    // Simple board evaluation: count pieces and their positions
    const Board& board = gameState.getBoard();
    int score = 0;
    
    // Center control bonus
    for (int row = 3; row <= 6; ++row) {
        for (int col = 3; col <= 6; ++col) {
            Position pos(row, col);
            Board::Cell cell = board.getCell(pos);
            
            // Check if this cell contains the player's amazon
            if ((player == Player::WHITE && cell == Board::Cell::WHITE_AMAZON) ||
                (player == Player::BLACK && cell == Board::Cell::BLACK_AMAZON)) {
                score += 1; // Bonus for controlling center
            }
        }
    }
    
    return score;
}

int BasicAI::countAvailableMoves(const GameState& gameState, Player player) const {
    // Count legal moves for the player
    auto moves = gameState.getLegalMovesForPlayer(player);
    return static_cast<int>(moves.size());
}

} // namespace amazons
