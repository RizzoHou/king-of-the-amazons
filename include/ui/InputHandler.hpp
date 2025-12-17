#pragma once

#include "core/Position.hpp"
#include "core/Move.hpp"
#include <string>
#include <optional>

namespace amazons {

class InputHandler {
public:
    InputHandler() = default;
    
    // Parse user input
    std::optional<Position> parsePosition(const std::string& input) const;
    std::optional<Move> parseMove(const std::string& input) const;
    
    // Get user input with prompts
    Position getPosition(const std::string& prompt) const;
    Move getMove(const std::string& prompt) const;
    
    // Menu choices
    int getMenuChoice(const std::string& prompt, int min, int max) const;
    bool getYesNo(const std::string& prompt) const;
    
    // Helper methods
    static bool isValidPositionString(const std::string& str);
    static bool isValidMoveString(const std::string& str);
    
private:
    std::string getLine(const std::string& prompt) const;
    void clearInputBuffer() const;
};

} // namespace amazons
