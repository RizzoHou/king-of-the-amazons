#include "utils/Serializer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <sys/stat.h> // For mkdir
#include <dirent.h>   // For directory operations

namespace amazons {

bool Serializer::saveGame(const GameState& gameState, GameMode gameMode, const std::string& filename) const {
    try {
        std::string json = serializeGameState(gameState, gameMode);
        std::string fullPath = getFullPath(filename);
        
        // Create save directory if it doesn't exist
        std::string saveDir = getSaveDirectory();
        struct stat info;
        if (stat(saveDir.c_str(), &info) != 0) {
            // Directory doesn't exist, create it
            #ifdef _WIN32
                mkdir(saveDir.c_str());
            #else
                mkdir(saveDir.c_str(), 0755);
            #endif
        }
        
        std::ofstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << fullPath << std::endl;
            return false;
        }
        
        file << json;
        file.close();
        
        std::cout << "Game saved to: " << fullPath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<GameState> Serializer::loadGame(const std::string& filename) const {
    try {
        std::string fullPath = getFullPath(filename);
        
        // Check if file exists
        std::ifstream testFile(fullPath);
        if (!testFile.good()) {
            std::cerr << "Error: Save file does not exist: " << fullPath << std::endl;
            return nullptr;
        }
        testFile.close();
        
        std::ifstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << fullPath << std::endl;
            return nullptr;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string json = buffer.str();
        file.close();
        
        auto gameState = deserializeGameState(json);
        if (gameState) {
            std::cout << "Game loaded from: " << fullPath << std::endl;
        }
        
        return gameState;
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return nullptr;
    }
}

std::pair<std::unique_ptr<GameState>, GameMode> Serializer::loadGameWithMode(const std::string& filename) const {
    try {
        std::string fullPath = getFullPath(filename);
        
        // Check if file exists
        std::ifstream testFile(fullPath);
        if (!testFile.good()) {
            std::cerr << "Error: Save file does not exist: " << fullPath << std::endl;
            return {nullptr, GameMode::HUMAN_VS_HUMAN};
        }
        testFile.close();
        
        std::ifstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << fullPath << std::endl;
            return {nullptr, GameMode::HUMAN_VS_HUMAN};
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string json = buffer.str();
        file.close();
        
        auto result = deserializeGameStateWithMode(json);
        if (result.first) {
            std::cout << "Game loaded from: " << fullPath << std::endl;
        }
        
        return result;
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return {nullptr, GameMode::HUMAN_VS_HUMAN};
    }
}

std::vector<std::string> Serializer::getSavedGames() const {
    std::vector<std::string> savedGames;
    
    try {
        std::string saveDir = getSaveDirectory();
        
        DIR* dir = opendir(saveDir.c_str());
        if (dir == nullptr) {
            return savedGames; // Directory doesn't exist
        }
        
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            if (filename.length() > 5 && filename.substr(filename.length() - 5) == ".json") {
                savedGames.push_back(filename.substr(0, filename.length() - 5));
            }
        }
        
        closedir(dir);
        
        // Sort alphabetically
        std::sort(savedGames.begin(), savedGames.end());
        
    } catch (const std::exception& e) {
        std::cerr << "Error listing saved games: " << e.what() << std::endl;
    }
    
    return savedGames;
}

bool Serializer::saveExists(const std::string& filename) const {
    std::string fullPath = getFullPath(filename);
    std::ifstream file(fullPath);
    return file.good();
}

bool Serializer::deleteSave(const std::string& filename) const {
    try {
        std::string fullPath = getFullPath(filename);
        
        // Check if file exists
        std::ifstream testFile(fullPath);
        if (!testFile.good()) {
            std::cerr << "Error: Save file does not exist: " << fullPath << std::endl;
            return false;
        }
        testFile.close();
        
        return std::remove(fullPath.c_str()) == 0;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting save file: " << e.what() << std::endl;
        return false;
    }
}

std::string Serializer::serializeGameState(const GameState& gameState, GameMode gameMode) const {
    std::stringstream ss;
    
    ss << "{\n";
    
    // Serialize board
    ss << "  \"" << FIELD_BOARD << "\": \"";
    const Board& board = gameState.getBoard();
    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            Board::Cell cell = board.getCell(row, col);
            char cellChar = '.';
            switch (cell) {
                case Board::Cell::EMPTY: cellChar = '.'; break;
                case Board::Cell::ARROW: cellChar = 'X'; break;
                case Board::Cell::WHITE_AMAZON: cellChar = 'W'; break;
                case Board::Cell::BLACK_AMAZON: cellChar = 'B'; break;
            }
            ss << cellChar;
        }
    }
    ss << "\",\n";
    
    // Serialize current player
    ss << "  \"" << FIELD_CURRENT_PLAYER << "\": \"" 
       << (gameState.getCurrentPlayer() == Player::WHITE ? "white" : "black") << "\",\n";
    
    // Serialize turn number
    ss << "  \"" << FIELD_TURN_NUMBER << "\": " << gameState.getTurnNumber() << ",\n";
    
    // Serialize game mode
    ss << "  \"" << FIELD_GAME_MODE << "\": \"";
    switch (gameMode) {
        case GameMode::HUMAN_VS_HUMAN: ss << "human_vs_human"; break;
        case GameMode::HUMAN_VS_AI_HUMAN_WHITE: ss << "human_vs_ai_human_white"; break;
        case GameMode::HUMAN_VS_AI_HUMAN_BLACK: ss << "human_vs_ai_human_black"; break;
        case GameMode::AI_VS_AI: ss << "ai_vs_ai"; break;
    }
    ss << "\"\n";
    
    ss << "}\n";
    
    return ss.str();
}

std::unique_ptr<GameState> Serializer::deserializeGameState(const std::string& json) const {
    // For backward compatibility, just parse game state without mode
    auto result = deserializeGameStateWithMode(json);
    return std::move(result.first);
}

std::pair<std::unique_ptr<GameState>, GameMode> Serializer::deserializeGameStateWithMode(const std::string& json) const {
    // Simple JSON parsing for our specific format
    // We expect format: {"board": "..........BB......BB..........WW......WW..........", "current_player": "white", "turn_number": 1, "game_mode": "human_vs_human"}
    
    try {
        // Extract board string
        size_t boardStart = json.find(FIELD_BOARD);
        if (boardStart == std::string::npos) {
            throw std::runtime_error("Board field not found in JSON");
        }
        
        boardStart = json.find('"', boardStart + strlen(FIELD_BOARD) + 3); // Skip "board": "
        if (boardStart == std::string::npos) {
            throw std::runtime_error("Invalid board field format");
        }
        
        size_t boardEnd = json.find('"', boardStart + 1);
        if (boardEnd == std::string::npos) {
            throw std::runtime_error("Invalid board field format");
        }
        
        std::string boardStr = json.substr(boardStart + 1, boardEnd - boardStart - 1);
        if (boardStr.length() != Board::SIZE * Board::SIZE) {
            throw std::runtime_error("Invalid board string length");
        }
        
        // Extract current player
        size_t playerStart = json.find(FIELD_CURRENT_PLAYER);
        if (playerStart == std::string::npos) {
            throw std::runtime_error("Current player field not found in JSON");
        }
        
        playerStart = json.find('"', playerStart + strlen(FIELD_CURRENT_PLAYER) + 3);
        if (playerStart == std::string::npos) {
            throw std::runtime_error("Invalid current player field format");
        }
        
        size_t playerEnd = json.find('"', playerStart + 1);
        if (playerEnd == std::string::npos) {
            throw std::runtime_error("Invalid current player field format");
        }
        
        std::string playerStr = json.substr(playerStart + 1, playerEnd - playerStart - 1);
        Player currentPlayer = (playerStr == "white") ? Player::WHITE : Player::BLACK;
        
        // Extract turn number
        size_t turnStart = json.find(FIELD_TURN_NUMBER);
        if (turnStart == std::string::npos) {
            throw std::runtime_error("Turn number field not found in JSON");
        }
        
        turnStart = json.find(':', turnStart) + 1;
        size_t turnEnd = json.find_first_of(",\n}", turnStart);
        if (turnEnd == std::string::npos) {
            throw std::runtime_error("Invalid turn number field format");
        }
        
        std::string turnStr = json.substr(turnStart, turnEnd - turnStart);
        int turnNumber = std::stoi(turnStr);
        
        // Extract game mode (optional for backward compatibility)
        GameMode gameMode = GameMode::HUMAN_VS_HUMAN; // Default
        size_t modeStart = json.find(FIELD_GAME_MODE);
        if (modeStart != std::string::npos) {
            modeStart = json.find('"', modeStart + strlen(FIELD_GAME_MODE) + 3);
            if (modeStart != std::string::npos) {
                size_t modeEnd = json.find('"', modeStart + 1);
                if (modeEnd != std::string::npos) {
                    std::string modeStr = json.substr(modeStart + 1, modeEnd - modeStart - 1);
                    if (modeStr == "human_vs_human") {
                        gameMode = GameMode::HUMAN_VS_HUMAN;
                    } else if (modeStr == "human_vs_ai_human_white") {
                        gameMode = GameMode::HUMAN_VS_AI_HUMAN_WHITE;
                    } else if (modeStr == "human_vs_ai_human_black") {
                        gameMode = GameMode::HUMAN_VS_AI_HUMAN_BLACK;
                    } else if (modeStr == "human_vs_ai") {
                        // Backward compatibility: old HUMAN_VS_AI maps to HUMAN_VS_AI_HUMAN_BLACK
                        gameMode = GameMode::HUMAN_VS_AI_HUMAN_BLACK;
                    } else if (modeStr == "ai_vs_ai") {
                        gameMode = GameMode::AI_VS_AI;
                    }
                }
            }
        }
        
        // Create board from string
        Board board;
        for (int row = 0; row < Board::SIZE; ++row) {
            for (int col = 0; col < Board::SIZE; ++col) {
                char cellChar = boardStr[row * Board::SIZE + col];
                Board::Cell cell = Board::Cell::EMPTY;
                switch (cellChar) {
                    case '.': cell = Board::Cell::EMPTY; break;
                    case 'X': cell = Board::Cell::ARROW; break;
                    case 'W': cell = Board::Cell::WHITE_AMAZON; break;
                    case 'B': cell = Board::Cell::BLACK_AMAZON; break;
                    default: throw std::runtime_error("Invalid cell character in board string");
                }
                board.setCell(row, col, cell);
            }
        }
        
        // Create game state with restored board, player, and turn number
        auto gameState = std::make_unique<GameState>(board, currentPlayer, turnNumber);
        
        return {std::move(gameState), gameMode};
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        std::cerr << "JSON: " << json << std::endl;
        return {nullptr, GameMode::HUMAN_VS_HUMAN};
    }
}

std::string Serializer::getSaveDirectory() const {
    // Try to find the project root directory
    // First check if data/saves exists in current directory (project root)
    struct stat info;
    if (stat("data/saves/", &info) == 0 && S_ISDIR(info.st_mode)) {
        return "data/saves/";
    }
    // If not, try ../data/saves/ (when running from build/ directory)
    if (stat("../data/saves/", &info) == 0 && S_ISDIR(info.st_mode)) {
        return "../data/saves/";
    }
    // Default to data/saves/ in current directory
    return "data/saves/";
}

std::string Serializer::getFullPath(const std::string& filename) const {
    std::string baseName = filename;
    if (baseName.find(".json") == std::string::npos) {
        baseName += ".json";
    }
    return getSaveDirectory() + baseName;
}

} // namespace amazons
