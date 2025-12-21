#pragma once

#ifdef WITH_GRAPHICAL_GUI

#include "ui/Display.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>

namespace amazons {

class GraphicalDisplay : public Display {
public:
    GraphicalDisplay();
    ~GraphicalDisplay() override;
    
    // Display interface implementation
    void showBoard(const Board& board) override;
    void showGameState(const GameState& state) override;
    void showLegalMoves(const GameState& state, const Position& from) override;
    void showWinner(Player winner) override;
    void showCurrentPlayer(Player player) override;
    void showTurnNumber(int turn) override;
    void showMessage(const std::string& message) override;
    
    void showMenu(const std::vector<std::string>& options) override;
    
    std::string getInput() override;
    std::optional<Position> getMouseClick() override;
    std::optional<Move> getMoveInteractively(const GameState& state) override;
    
    void clearScreen() override;
    void waitForContinue() override;
    
    std::string boardToString(const Board& board) const override;
    std::string gameStateToString(const GameState& state) const override;
    
private:
    // Window and rendering
    std::unique_ptr<sf::RenderWindow> window;
    sf::Font font;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color boardColor;
    sf::Color gridColor;
    sf::Color whiteAmazonColor;
    sf::Color blackAmazonColor;
    sf::Color arrowColor;
    sf::Color highlightColor;
    sf::Color textColor;
    
    // Dimensions
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int BOARD_SIZE = 400;
    static constexpr int CELL_SIZE = BOARD_SIZE / Board::SIZE;
    static constexpr int BOARD_MARGIN = 50;
    
    // Helper methods
    void initializeWindow();
    void drawBoard(const Board& board);
    void drawCell(int row, int col, Board::Cell cell);
    void drawGrid();
    void drawCoordinates();
    void drawText(const std::string& text, int x, int y, int size = 20, 
                  sf::Color color = sf::Color::White);
    void drawStatusBar(const GameState& state);
    void drawMessage(const std::string& message);
    
    // Input handling
    bool handleEvents();
    std::optional<Position> convertMouseToPosition(int mouseX, int mouseY) const;
    
    // State
    std::string currentMessage;
    std::vector<Position> highlightedPositions;
    bool windowInitialized;
};

} // namespace amazons

#endif // WITH_GRAPHICAL_GUI
