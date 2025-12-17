#pragma once

#include "core/GameState.hpp"
#include <string>
#include <filesystem>

namespace amazons {

class Serializer {
public:
    Serializer() = default;
    
    // Save game state to file
    bool saveGame(const GameState& gameState, const std::string& filename) const;
    
    // Load game state from file
    std::unique_ptr<GameState> loadGame(const std::string& filename) const;
    
    // Get list of saved games
    std::vector<std::string> getSavedGames() const;
    
    // Check if a save file exists
    bool saveExists(const std::string& filename) const;
    
    // Delete a save file
    bool deleteSave(const std::string& filename) const;
    
private:
    // Helper methods for JSON serialization
    std::string serializeGameState(const GameState& gameState) const;
    std::unique_ptr<GameState> deserializeGameState(const std::string& json) const;
    
    // File operations
    std::string getSaveDirectory() const;
    std::string getFullPath(const std::string& filename) const;
    
    // JSON field names
    static constexpr const char* FIELD_BOARD = "board";
    static constexpr const char* FIELD_CURRENT_PLAYER = "current_player";
    static constexpr const char* FIELD_TURN_NUMBER = "turn_number";
    static constexpr const char* FIELD_MOVE_HISTORY = "move_history";
    static constexpr const char* FIELD_PLAYER_WHITE = "white";
    static constexpr const char* FIELD_PLAYER_BLACK = "black";
    static constexpr const char* FIELD_EMPTY = "empty";
    static constexpr const char* FIELD_ARROW = "arrow";
};

} // namespace amazons
