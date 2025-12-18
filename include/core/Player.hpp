#pragma once

namespace amazons {

enum class Player {
    WHITE,
    BLACK
};

// Game modes
enum class GameMode {
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI,    // Human as White, AI as Black
    AI_VS_AI
};

// Helper functions
inline Player oppositePlayer(Player player) {
    return (player == Player::WHITE) ? Player::BLACK : Player::WHITE;
}

inline const char* playerToString(Player player) {
    switch (player) {
        case Player::WHITE: return "WHITE";
        case Player::BLACK: return "BLACK";
        default: return "UNKNOWN";
    }
}

inline const char* gameModeToString(GameMode mode) {
    switch (mode) {
        case GameMode::HUMAN_VS_HUMAN: return "HUMAN_VS_HUMAN";
        case GameMode::HUMAN_VS_AI: return "HUMAN_VS_AI";
        case GameMode::AI_VS_AI: return "AI_VS_AI";
        default: return "UNKNOWN";
    }
}

} // namespace amazons
