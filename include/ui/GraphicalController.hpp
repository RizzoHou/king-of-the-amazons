#pragma once

#include "core/GameState.hpp"
#include "core/Board.hpp"
#include "core/Move.hpp"
#include "ai/BasicAI.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace amazons {

enum class SelectionState {
    NO_SELECTION,
    AMAZON_SELECTED,
    MOVE_SELECTED
};

enum class GameModeGUI {
    NOT_SELECTED,
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI,
    AI_VS_AI
};

class GraphicalController {
public:
    GraphicalController();
    ~GraphicalController();
    
    // Main game loop
    void run();
    
private:
    // Window settings
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 700;
    static constexpr int CELL_SIZE = 60;
    static constexpr int BOARD_OFFSET_X = 100;
    static constexpr int BOARD_OFFSET_Y = 100;
    static constexpr int BOARD_SIZE = CELL_SIZE * Board::SIZE;
    
    // SFML objects
    std::unique_ptr<sf::RenderWindow> window;
    sf::Font font;
    
    // Game objects
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<BasicAI> ai;
    
    // Saved game for "Continue" feature
    std::unique_ptr<GameState> savedGameState;
    GameModeGUI savedGameMode;
    
    // Selection state
    SelectionState selectionState;
    Position selectedPosition;
    Position moveToPosition;
    std::vector<Position> validMoves;
    std::vector<Position> validArrowPositions;
    
    // UI state
    GameModeGUI currentGameMode;
    bool showModeSelection;
    std::string statusMessage;
    
    // Initialization
    bool initialize();
    bool loadResources();
    
    // Event handling
    void handleEvents();
    void handleMouseClick(int x, int y);
    void handleModeSelection(int x, int y);
    void handleKeyPress(sf::Keyboard::Key key);
    
    // Rendering
    void render();
    void drawModeSelection();
    void drawBoard();
    void drawPieces();
    void drawHighlights();
    void drawUI();
    
    // Game logic
    void processAIMove();
    void resetSelection();
    void selectAmazon(const Position& pos);
    void selectMoveDestination(const Position& pos);
    void selectArrowDestination(const Position& pos);
    void makeMove(const Move& move);
    
    // Utility
    sf::Color getCellColor(int row, int col) const;
    std::optional<Position> getBoardPosition(int mouseX, int mouseY) const;
    void updateStatusMessage();
    
    // Mode management
    void startGame(GameModeGUI mode);
    void continueGame();
};

} // namespace amazons
