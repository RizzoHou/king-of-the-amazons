#include "utils/Serializer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <sys/stat.h> // For mkdir
#include <dirent.h>   // For directory operations

namespace amazons {

bool Serializer::saveGame(const GameState& gameState, const std::string& filename) const {
    try {
        std::string json = serializeGameState(gameState);
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

std::string Serializer::serializeGameState(const GameState& gameState) const {
    // For now, implement a simple text-based serialization
    // In a real implementation, we would use a JSON library like nlohmann/json
    std::stringstream ss;
    
    ss << "{\n";
    ss << "  \"" << FIELD_BOARD << "\": \"TODO\",\n";
    ss << "  \"" << FIELD_CURRENT_PLAYER << "\": \"" 
       << (gameState.getCurrentPlayer() == Player::WHITE ? "white" : "black") << "\",\n";
    ss << "  \"" << FIELD_TURN_NUMBER << "\": " << gameState.getTurnNumber() << "\n";
    ss << "}\n";
    
    return ss.str();
}

std::unique_ptr<GameState> Serializer::deserializeGameState(const std::string& json) const {
    // For now, create a new game state
    // In a real implementation, we would parse the JSON
    (void)json; // Mark parameter as used to avoid warning
    
    auto gameState = std::make_unique<GameState>();
    gameState->initializeStandardGame();
    
    // TODO: Parse JSON and restore actual game state
    
    return gameState;
}

std::string Serializer::getSaveDirectory() const {
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
