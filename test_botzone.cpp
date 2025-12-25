#include "ai/BotzoneAI.hpp"
#include "core/GameState.hpp"
#include "core/Board.hpp"
#include <iostream>

int main() {
    try {
        // Create BotzoneAI with bot003 path
        amazons::BotzoneAI bot("/Users/rizzohou/projects/amazing-amazons/bots/bot003");
        
        // Create initial game state
        amazons::GameState gameState;
        gameState.initializeStandardGame();
        
        std::cout << "Testing BotzoneAI with bot003..." << std::endl;
        std::cout << "Bot path: " << bot.getBotPath() << std::endl;
        std::cout << "Bot available: " << (bot.isBotAvailable() ? "Yes" : "No") << std::endl;
        
        // Try to get a move
        std::cout << "Getting best move..." << std::endl;
        amazons::Move move = bot.getBestMove(gameState);
        
        std::cout << "Move found: " << move.toString() << std::endl;
        std::cout << "From: " << move.from.toString() << std::endl;
        std::cout << "To: " << move.to.toString() << std::endl;
        std::cout << "Arrow: " << move.arrow.toString() << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}