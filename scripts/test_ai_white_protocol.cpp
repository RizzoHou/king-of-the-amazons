#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "ai/BotzoneAI.hpp"
#include "core/GameState.hpp"
#include "core/Move.hpp"
#include "core/Position.hpp"

using namespace amazons;

int main() {
    std::cout << "Testing BotzoneAI protocol when AI is white...\n";
    
    // Path to bot003
    std::string botPath = "third_party/bots/bot003";
    
    // Create BotzoneAI instance
    BotzoneAI ai(botPath);
    
    // Set AI color to white (moves second)
    ai.setAIColor(Player::WHITE);
    
    // Create a game state
    GameState gameState;
    gameState.initializeStandardGame();
    
    // Black (human) makes a first move
    // Let's simulate a simple first move: (0,2) -> (0,3) -> (3,0)
    Position from(0, 2);  // Black amazon at (0,2)
    Position to(0, 3);    // Move to (0,3)
    Position arrow(3, 0); // Place arrow at (3,0)
    
    Move firstMove(from, to, arrow);
    
    std::cout << "Simulating human (black) first move: " << firstMove.toString() << "\n";
    
    // Make the move
    if (gameState.isValidMove(firstMove)) {
        gameState.makeMove(firstMove);
        std::cout << "Move successful. Current player: " 
                  << (gameState.getCurrentPlayer() == Player::WHITE ? "WHITE" : "BLACK") << "\n";
    } else {
        std::cout << "Invalid move!\n";
        return 1;
    }
    
    try {
        // Now ask AI (white) for a move
        std::cout << "Asking AI (white) for a move...\n";
        Move aiMove = ai.getBestMove(gameState);
        std::cout << "AI move: " << aiMove.toString() << "\n";
        
        // Verify the move is valid
        if (gameState.isValidMove(aiMove)) {
            std::cout << "AI move is valid!\n";
            
            // Make the move
            gameState.makeMove(aiMove);
            std::cout << "AI move executed successfully.\n";
            std::cout << "Current player: " 
                      << (gameState.getCurrentPlayer() == Player::WHITE ? "WHITE" : "BLACK") << "\n";
            
            // Test second move - human (black) moves again
            std::cout << "\nTesting second move...\n";
            
            // Human makes another move
            // Find a legal move for black
            auto legalMoves = gameState.getLegalMoves();
            if (!legalMoves.empty()) {
                Move secondHumanMove = legalMoves[0];
                std::cout << "Human (black) second move: " << secondHumanMove.toString() << "\n";
                
                if (gameState.isValidMove(secondHumanMove)) {
                    gameState.makeMove(secondHumanMove);
                    std::cout << "Second move successful.\n";
                    
                    // Ask AI for another move
                    std::cout << "Asking AI (white) for second move...\n";
                    Move secondAiMove = ai.getBestMove(gameState);
                    std::cout << "AI second move: " << secondAiMove.toString() << "\n";
                    
                    if (gameState.isValidMove(secondAiMove)) {
                        std::cout << "AI second move is valid!\n";
                        std::cout << "\nTest PASSED: AI as white works correctly!\n";
                        return 0;
                    } else {
                        std::cout << "AI second move is invalid!\n";
                        return 1;
                    }
                } else {
                    std::cout << "Second human move is invalid!\n";
                    return 1;
                }
            } else {
                std::cout << "No legal moves for black!\n";
                return 1;
            }
        } else {
            std::cout << "AI move is invalid!\n";
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}