#ifdef WITH_GRAPHICAL_GUI

#include "ui/GraphicalDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

namespace amazons {

GraphicalDisplay::GraphicalDisplay() 
    : windowInitialized(false) {
    // Initialize colors
    backgroundColor = sf::Color(30, 30, 46);     // Dark blue-gray
    boardColor = sf::Color(46, 52, 64);          // Slate gray
    gridColor = sf::Color(67, 76, 94);           // Light slate gray
    whiteAmazonColor = sf::Color(236, 239, 244); // White
    blackAmazonColor = sf::Color(59, 66, 82);    // Dark gray
    arrowColor = sf::Color(191, 97, 106);        // Red
    highlightColor = sf::Color(136, 192, 208);   // Blue
    textColor = sf::Color(236, 239, 244);        // White
    
    currentMessage = "Welcome to King of the Amazons!";
}

GraphicalDisplay::~GraphicalDisplay() {
    if (window && window->isOpen()) {
        window->close();
    }
}

void GraphicalDisplay::initializeWindow() {
    if (!windowInitialized) {
        window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), 
            "King of the Amazons",
            sf::Style::Titlebar | sf::Style::Close
        );
        window->setFramerateLimit(60);
        
        // Try to load font
        if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            // Fallback to Arial if Helvetica not available
            if (!font.openFromFile("/System/Library/Fonts/Arial.ttf")) {
                std::cerr << "Warning: Could not load font. Using default.\n";
            }
        }
        
        windowInitialized = true;
    }
}

void GraphicalDisplay::showBoard(const Board& board) {
    initializeWindow();
    
    // Clear highlighted positions when showing new board
    highlightedPositions.clear();
    
    // Main rendering loop
    while (window->isOpen()) {
        if (!handleEvents()) {
            break;
        }
        
        window->clear(backgroundColor);
        
        // Draw board background
        sf::RectangleShape boardBackground(sf::Vector2f(BOARD_SIZE, BOARD_SIZE));
        boardBackground.setPosition(sf::Vector2f(BOARD_MARGIN, BOARD_MARGIN));
        boardBackground.setFillColor(boardColor);
        window->draw(boardBackground);
        
        // Draw grid
        drawGrid();
        
        // Draw coordinates
        drawCoordinates();
        
        // Draw pieces
        for (int row = 0; row < Board::SIZE; ++row) {
            for (int col = 0; col < Board::SIZE; ++col) {
                drawCell(row, col, board.getCell(row, col));
            }
        }
        
        // Draw highlighted positions
        for (const auto& pos : highlightedPositions) {
            sf::CircleShape highlight(CELL_SIZE / 3);
            highlight.setPosition(sf::Vector2f(
                BOARD_MARGIN + pos.col * CELL_SIZE + CELL_SIZE / 6,
                BOARD_MARGIN + pos.row * CELL_SIZE + CELL_SIZE / 6
            ));
            highlight.setFillColor(sf::Color(highlightColor.r, highlightColor.g, highlightColor.b, 100));
            window->draw(highlight);
        }
        
        // Draw current message
        drawMessage(currentMessage);
        
        window->display();
    }
}

void GraphicalDisplay::showGameState(const GameState& state) {
    initializeWindow();
    
    // Clear highlighted positions
    highlightedPositions.clear();
    
    // Update message
    currentMessage = "Turn " + std::to_string(state.getTurnNumber()) + 
                    " - " + Display::playerToString(state.getCurrentPlayer()) + "'s turn";
    
    if (state.isGameOver()) {
        currentMessage = "Game Over! " + Display::playerToString(state.getWinner()) + " wins!";
    }
    
    // Show the board
    showBoard(state.getBoard());
}

void GraphicalDisplay::showLegalMoves(const GameState& state, const Position& from) {
    initializeWindow();
    
    // Get legal moves from this position
    auto moves = state.getLegalMoves();
    highlightedPositions.clear();
    
    for (const auto& move : moves) {
        if (move.from == from) {
            highlightedPositions.push_back(move.to);
            highlightedPositions.push_back(move.arrow);
        }
    }
    
    // Update message
    currentMessage = "Legal moves from " + from.toString();
    
    // Show the board with highlights
    showBoard(state.getBoard());
}

void GraphicalDisplay::showWinner(Player winner) {
    currentMessage = "Game Over! " + Display::playerToString(winner) + " wins!";
    
    // Keep the window open for a moment
    if (window && window->isOpen()) {
        for (int i = 0; i < 30; ++i) { // ~0.5 seconds at 60 FPS
            if (!handleEvents()) break;
            window->clear(backgroundColor);
            drawMessage(currentMessage);
            window->display();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}

void GraphicalDisplay::showCurrentPlayer(Player player) {
    currentMessage = Display::playerToString(player) + "'s turn";
}

void GraphicalDisplay::showTurnNumber(int turn) {
    // This is handled in showGameState
    (void)turn; // Mark parameter as unused to avoid warning
}

void GraphicalDisplay::showMessage(const std::string& message) {
    currentMessage = message;
    
    // If window is open, display the message briefly
    if (window && window->isOpen()) {
        for (int i = 0; i < 30; ++i) { // ~0.5 seconds at 60 FPS
            if (!handleEvents()) break;
            window->clear(backgroundColor);
            drawMessage(currentMessage);
            window->display();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}

void GraphicalDisplay::showMenu(const std::vector<std::string>& options) {
    // For now, fall back to console for menus
    std::cout << "\n=== Menu ===\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i] << "\n";
    }
    std::cout << "Select an option: ";
}

std::string GraphicalDisplay::getInput() {
    // For now, fall back to console input
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::optional<Position> GraphicalDisplay::getMouseClick() {
    initializeWindow();
    
    if (!window) {
        return std::nullopt;
    }
    
    // Wait for a mouse click
    while (window->isOpen()) {
        while (auto event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
                return std::nullopt;
            }
            
            if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    auto mousePos = sf::Mouse::getPosition(*window);
                    auto pos = convertMouseToPosition(mousePos.x, mousePos.y);
                    if (pos.has_value()) {
                        return pos;
                    }
                }
            }
            
            // Also allow escape key to cancel
            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    return std::nullopt;
                }
            }
        }
        
        // Keep the window responsive
        window->clear(backgroundColor);
        drawMessage("Click on a board position or press ESC to cancel");
        window->display();
    }
    
    return std::nullopt;
}

std::optional<Move> GraphicalDisplay::getMoveInteractively(const GameState& state) {
    initializeWindow();
    
    if (!window) {
        return std::nullopt;
    }
    
    // Step 1: Select source position (queen)
    drawMessage("Click on a queen to move (or ESC to cancel)");
    window->display();
    
    auto from = getMouseClick();
    if (!from.has_value()) {
        return std::nullopt;
    }
    
    // Highlight selected queen
    highlightedPositions.clear();
    highlightedPositions.push_back(from.value());
    
    // Get legal moves from this position
    auto moves = state.getLegalMoves();
    std::vector<Move> legalMovesFromHere;
    for (const auto& move : moves) {
        if (move.from == from.value()) {
            legalMovesFromHere.push_back(move);
        }
    }
    
    if (legalMovesFromHere.empty()) {
        drawMessage("No legal moves from this position. Press any key to continue.");
        window->display();
        waitForContinue();
        return std::nullopt;
    }
    
    // Highlight possible destinations
    std::vector<Position> destinations;
    for (const auto& move : legalMovesFromHere) {
        destinations.push_back(move.to);
    }
    highlightedPositions = destinations;
    
    // Step 2: Select destination
    drawMessage("Click on destination position (or ESC to cancel)");
    window->display();
    
    auto to = getMouseClick();
    if (!to.has_value()) {
        return std::nullopt;
    }
    
    // Find moves that match this destination
    std::vector<Move> movesToDestination;
    for (const auto& move : legalMovesFromHere) {
        if (move.to == to.value()) {
            movesToDestination.push_back(move);
        }
    }
    
    if (movesToDestination.empty()) {
        drawMessage("Invalid destination. Press any key to continue.");
        window->display();
        waitForContinue();
        return std::nullopt;
    }
    
    // If only one possible arrow position, use it
    if (movesToDestination.size() == 1) {
        highlightedPositions.clear();
        return movesToDestination[0];
    }
    
    // Step 3: Select arrow position (if multiple options)
    std::vector<Position> arrowPositions;
    for (const auto& move : movesToDestination) {
        arrowPositions.push_back(move.arrow);
    }
    highlightedPositions = arrowPositions;
    
    drawMessage("Click on arrow position (or ESC to cancel)");
    window->display();
    
    auto arrow = getMouseClick();
    if (!arrow.has_value()) {
        return std::nullopt;
    }
    
    // Find the matching move
    for (const auto& move : movesToDestination) {
        if (move.arrow == arrow.value()) {
            highlightedPositions.clear();
            return move;
        }
    }
    
    drawMessage("Invalid arrow position. Press any key to continue.");
    window->display();
    waitForContinue();
    return std::nullopt;
}

void GraphicalDisplay::clearScreen() {
    // For graphical display, we just clear the window
    if (window && window->isOpen()) {
        window->clear(backgroundColor);
        window->display();
    }
}

void GraphicalDisplay::waitForContinue() {
    // Show a message and wait for any key
    if (window && window->isOpen()) {
        drawMessage("Press any key to continue...");
        window->display();
        
        // Wait for a key press
        bool waiting = true;
        while (waiting && window->isOpen()) {
            while (auto event = window->pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window->close();
                    waiting = false;
                }
                if (event->is<sf::Event::KeyPressed>()) {
                    waiting = false;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    } else {
        // Fall back to console
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string GraphicalDisplay::boardToString(const Board& board) const {
    // Fall back to text representation
    std::ostringstream oss;
    
    oss << "  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        oss << " " << col << " ";
    }
    oss << "\n";
    
    for (int row = 0; row < Board::SIZE; ++row) {
        oss << row << " ";
        for (int col = 0; col < Board::SIZE; ++col) {
            oss << " " << Display::cellToString(board.getCell(row, col)) << " ";
        }
        oss << " " << row << "\n";
    }
    
    oss << "  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        oss << " " << col << " ";
    }
    oss << "\n";
    
    return oss.str();
}

std::string GraphicalDisplay::gameStateToString(const GameState& state) const {
    // Fall back to text representation
    std::ostringstream oss;
    
    oss << "Turn: " << state.getTurnNumber() << "\n";
    oss << "Current player: " << Display::playerToString(state.getCurrentPlayer()) << "\n";
    oss << boardToString(state.getBoard());
    
    if (state.isGameOver()) {
        oss << "Game Over! " << Display::playerToString(state.getWinner()) << " wins!\n";
    }
    
    return oss.str();
}

// Helper methods
void GraphicalDisplay::drawCell(int row, int col, Board::Cell cell) {
    int x = BOARD_MARGIN + col * CELL_SIZE;
    int y = BOARD_MARGIN + row * CELL_SIZE;
    
    // Draw cell background
    sf::RectangleShape cellRect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cellRect.setPosition(sf::Vector2f(x, y));
    cellRect.setFillColor(boardColor);
    cellRect.setOutlineThickness(1);
    cellRect.setOutlineColor(gridColor);
    window->draw(cellRect);
    
    // Draw piece if any
    if (cell != Board::Cell::EMPTY) {
        sf::CircleShape piece(CELL_SIZE / 3);
        piece.setPosition(sf::Vector2f(x + CELL_SIZE / 6, y + CELL_SIZE / 6));
        
        switch (cell) {
            case Board::Cell::WHITE_AMAZON:
                piece.setFillColor(whiteAmazonColor);
                piece.setOutlineThickness(2);
                piece.setOutlineColor(sf::Color::Black);
                break;
            case Board::Cell::BLACK_AMAZON:
                piece.setFillColor(blackAmazonColor);
                piece.setOutlineThickness(2);
                piece.setOutlineColor(sf::Color::White);
                break;
            case Board::Cell::ARROW:
                piece.setFillColor(arrowColor);
                piece.setOutlineThickness(1);
                piece.setOutlineColor(sf::Color::Black);
                break;
            default:
                return;
        }
        
        window->draw(piece);
    }
}

void GraphicalDisplay::drawGrid() {
    // Draw vertical lines
    for (int col = 0; col <= Board::SIZE; ++col) {
        sf::RectangleShape line(sf::Vector2f(1, BOARD_SIZE));
        line.setPosition(sf::Vector2f(BOARD_MARGIN + col * CELL_SIZE, BOARD_MARGIN));
        line.setFillColor(gridColor);
        window->draw(line);
    }
    
    // Draw horizontal lines
    for (int row = 0; row <= Board::SIZE; ++row) {
        sf::RectangleShape line(sf::Vector2f(BOARD_SIZE, 1));
        line.setPosition(sf::Vector2f(BOARD_MARGIN, BOARD_MARGIN + row * CELL_SIZE));
        line.setFillColor(gridColor);
        window->draw(line);
    }
}

void GraphicalDisplay::drawCoordinates() {
    // Draw row numbers
    for (int row = 0; row < Board::SIZE; ++row) {
        drawText(std::to_string(row), 
                 BOARD_MARGIN - 20, 
                 BOARD_MARGIN + row * CELL_SIZE + CELL_SIZE / 2 - 10,
                 16, textColor);
    }
    
    // Draw column numbers
    for (int col = 0; col < Board::SIZE; ++col) {
        drawText(std::to_string(col), 
                 BOARD_MARGIN + col * CELL_SIZE + CELL_SIZE / 2 - 5, 
                 BOARD_MARGIN + BOARD_SIZE + 10,
                 16, textColor);
    }
}

void GraphicalDisplay::drawText(const std::string& text, int x, int y, int size, sf::Color color) {
    sf::Text sfText(font, text, size);
    sfText.setFillColor(color);
    sfText.setPosition(sf::Vector2f(x, y));
    window->draw(sfText);
}

void GraphicalDisplay::drawMessage(const std::string& message) {
    drawText(message, 10, WINDOW_HEIGHT - 40, 18, textColor);
}

bool GraphicalDisplay::handleEvents() {
    if (!window) return false;
    
    while (auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
            return false;
        }
        
        // Allow escape key to close the view
        if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                window->close();
                return false;
            }
        }
    }
    
    return true;
}

std::optional<Position> GraphicalDisplay::convertMouseToPosition(int mouseX, int mouseY) const {
    // Check if click is within board bounds
    if (mouseX < BOARD_MARGIN || mouseX >= BOARD_MARGIN + BOARD_SIZE ||
        mouseY < BOARD_MARGIN || mouseY >= BOARD_MARGIN + BOARD_SIZE) {
        return std::nullopt;
    }
    
    // Convert to board coordinates
    int col = (mouseX - BOARD_MARGIN) / CELL_SIZE;
    int row = (mouseY - BOARD_MARGIN) / CELL_SIZE;
    
    // Ensure coordinates are valid
    if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
        return Position(row, col);
    }
    
    return std::nullopt;
}

} // namespace amazons

#endif // WITH_GRAPHICAL_GUI
