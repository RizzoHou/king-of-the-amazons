#pragma once

#include "core/GameState.hpp"
#include "core/Move.hpp"
#include <vector>
#include <memory>

namespace amazons {

class BasicAI {
public:
    BasicAI() = default;
    
    // Get the best move for the given game state
    Move getBestMove(const GameState& gameState) const;
    
    // Get a random move (fallback)
    Move getRandomMove(const GameState& gameState) const;
    
private:
    // Simple heuristic evaluation of a move
    int evaluateMove(const GameState& gameState, const Move& move) const;
    
    // Evaluate board position for a player
    int evaluateBoardForPlayer(const GameState& gameState, Player player) const;
    
    // Count available moves for a player
    int countAvailableMoves(const GameState& gameState, Player player) const;
};

} // namespace amazons
