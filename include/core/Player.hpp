#pragma once

namespace amazons {

enum class Player {
    WHITE,
    BLACK
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

} // namespace amazons
