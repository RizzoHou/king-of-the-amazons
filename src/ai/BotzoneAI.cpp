#include "ai/BotzoneAI.hpp"
#include "ai/BotProcess.hpp"
#include "core/GameState.hpp"
#include "core/Move.hpp"
#include "core/Position.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace amazons {

BotzoneAI::BotzoneAI(const std::string& botPath) 
    : botPath(botPath), keepRunningMode(false) {
    if (!botPath.empty()) {
        botProcess = std::make_unique<BotProcess>(botPath);
    }
}

BotzoneAI::~BotzoneAI() {
    // BotProcess will clean up in its destructor
}

Move BotzoneAI::getBestMove(const GameState& gameState) {
    if (botPath.empty() || !botProcess) {
        throw std::runtime_error("BotzoneAI: No bot path configured");
    }
    
    try {
        // Convert game state to Botzone input
        std::string input = convertGameStateToBotzoneInput(gameState);
        
        // Get move history
        std::vector<std::string> history = getMoveHistory(gameState);
        
        // Check if we need to start or restart the bot
        if (!botProcess->isRunning() || !keepRunningMode) {
            // Start bot and send first turn
            if (!botProcess->start()) {
                throw std::runtime_error("Failed to start bot process");
            }
            
            if (!botProcess->sendFirstTurn(history)) {
                throw std::runtime_error("Failed to send first turn to bot");
            }
            
            keepRunningMode = true;
        } else {
            // Send opponent's last move (if any)
            if (!history.empty()) {
                std::string lastMove = history.back();
                if (!botProcess->sendTurn(lastMove)) {
                    throw std::runtime_error("Failed to send turn to bot");
                }
            }
        }
        
        // Read move from bot
        std::string moveOutput = botProcess->readMove(5.0); // 5 second timeout
        if (moveOutput.empty()) {
            throw std::runtime_error("Bot timed out or returned empty move");
        }
        
        // Read keep-running signal
        std::string keepSignal = botProcess->readKeepRunning(0.5);
        if (keepSignal != ">>>BOTZONE_REQUEST_KEEP_RUNNING<<<") {
            // Bot didn't send keep-running signal, need to restart next time
            keepRunningMode = false;
        }
        
        // Convert output to Move
        Move move = convertBotzoneOutputToMove(moveOutput);
        
        // Add move to history
        moveHistory.push_back(moveOutput);
        
        return move;
        
    } catch (const std::exception& e) {
        std::cerr << "BotzoneAI error: " << e.what() << std::endl;
        keepRunningMode = false;
        throw;
    }
}

void BotzoneAI::setBotPath(const std::string& path) {
    botPath = path;
    if (!path.empty()) {
        botProcess = std::make_unique<BotProcess>(path);
    } else {
        botProcess.reset();
    }
    keepRunningMode = false;
    moveHistory.clear();
}

std::string BotzoneAI::getBotPath() const {
    return botPath;
}

bool BotzoneAI::isBotAvailable() const {
    return !botPath.empty() && botProcess != nullptr;
}

std::string BotzoneAI::convertGameStateToBotzoneInput(const GameState& /*gameState*/) const {
    // This method builds the Botzone protocol input
    // For now, return empty string - actual implementation will be in getBestMove
    return "";
}

Move BotzoneAI::convertBotzoneOutputToMove(const std::string& output) const {
    // Parse "x0 y0 x1 y1 x2 y2" format
    std::istringstream iss(output);
    int x0, y0, x1, y1, x2, y2;
    
    if (!(iss >> x0 >> y0 >> x1 >> y1 >> x2 >> y2)) {
        throw std::invalid_argument("Invalid move format: " + output);
    }
    
    // Check for special "no move" case
    if (x0 == -1 && y0 == -1 && x1 == -1 && y1 == -1 && x2 == -1 && y2 == -1) {
        throw std::runtime_error("Bot returned no legal moves");
    }
    
    // Create positions and move
    Position from(y0, x0); // Note: Botzone uses (x,y), we use (row,col)
    Position to(y1, x1);
    Position arrow(y2, x2);
    
    return Move(from, to, arrow);
}

std::vector<std::string> BotzoneAI::getMoveHistory(const GameState& /*gameState*/) const {
    // For now, return empty history
    // In a complete implementation, we would track the game history
    // and convert all previous moves to Botzone format
    return moveHistory;
}

} // namespace amazons