#include "ui/GraphicalController.hpp"
#include "ui/Display.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

namespace amazons {

GraphicalController::GraphicalController()
    : savedGameMode(GameModeGUI::NOT_SELECTED),
      selectionState(SelectionState::NO_SELECTION),
      selectedPosition(-1, -1),
      moveToPosition(-1, -1),
      currentGameMode(GameModeGUI::NOT_SELECTED),
      showModeSelection(true),
      showLoadScreen(false),
      statusMessage("Welcome to King of the Amazons!"),
      selectedSaveIndex(-1),
      scrollOffset(0) {
    
    if (!initialize()) {
        throw std::runtime_error("Failed to initialize GraphicalController");
    }
}

GraphicalController::~GraphicalController() {
    if (window && window->isOpen()) {
        window->close();
    }
}

bool GraphicalController::initialize() {
    // Create window
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)),
        "King of the Amazons - Graphical Mode",
        sf::Style::Titlebar | sf::Style::Close
    );
    window->setFramerateLimit(60);
    
    // Load resources
    if (!loadResources()) {
        std::cerr << "Warning: Could not load all resources\n";
    }
    
    // Initialize AI
    ai = std::make_unique<BasicAI>();
    
    return true;
}

bool GraphicalController::loadResources() {
    // Try to load system fonts
    if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        if (!font.openFromFile("/System/Library/Fonts/Arial.ttf")) {
            std::cerr << "Warning: Could not load font\n";
            return false;
        }
    }
    return true;
}

void GraphicalController::run() {
    while (window->isOpen()) {
        handleEvents();
        render();
    }
}

void GraphicalController::handleEvents() {
    while (auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
        }
        
        if (auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                handleMouseClick(mouseButton->position.x, mouseButton->position.y);
            }
        }
        
        if (auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyPress(keyPress->code);
        }
    }
}

void GraphicalController::handleMouseClick(int x, int y) {
    if (showModeSelection) {
        handleModeSelection(x, y);
        return;
    }
    
    if (showLoadScreen) {
        handleLoadScreenClick(x, y);
        return;
    }
    
    // Check if Save button was clicked
    if (x >= SAVE_BUTTON_X && x <= SAVE_BUTTON_X + SAVE_BUTTON_WIDTH &&
        y >= SAVE_BUTTON_Y && y <= SAVE_BUTTON_Y + SAVE_BUTTON_HEIGHT) {
        saveCurrentGame();
        return;
    }
    
    if (!gameState || gameState->isGameOver()) {
        return;
    }
    
    // Check if AI's turn
    if (currentGameMode == GameModeGUI::HUMAN_VS_AI && 
        gameState->getCurrentPlayer() == Player::BLACK) {
        return;
    }
    
    // AI vs AI mode has been removed from the graphical interface
    
    auto pos = getBoardPosition(x, y);
    if (!pos.has_value()) {
        return;
    }
    
    switch (selectionState) {
        case SelectionState::NO_SELECTION:
            selectAmazon(pos.value());
            break;
        case SelectionState::AMAZON_SELECTED:
            selectMoveDestination(pos.value());
            break;
        case SelectionState::MOVE_SELECTED:
            selectArrowDestination(pos.value());
            break;
    }
}

void GraphicalController::handleModeSelection(int x, int y) {
    // Button dimensions and positions
    const int buttonWidth = 300;
    const int buttonHeight = 60;
    const int buttonX = (WINDOW_WIDTH - buttonWidth) / 2;
    
    // Calculate button Y positions based on whether there's a saved game
    int continueY = 250;
    int humanVsHumanY = savedGameState ? 350 : 250;
    int humanVsAIY = savedGameState ? 450 : 350;
    int loadGameY = savedGameState ? 550 : 450;
    
    // Check "Continue Previous Game" button if there's a saved game
    if (savedGameState && x >= buttonX && x <= buttonX + buttonWidth &&
        y >= continueY && y <= continueY + buttonHeight) {
        continueGame();
        return;
    }
    
    // Human vs Human button
    if (x >= buttonX && x <= buttonX + buttonWidth &&
        y >= humanVsHumanY && y <= humanVsHumanY + buttonHeight) {
        startGame(GameModeGUI::HUMAN_VS_HUMAN);
    }
    // Human vs AI button
    else if (x >= buttonX && x <= buttonX + buttonWidth &&
             y >= humanVsAIY && y <= humanVsAIY + buttonHeight) {
        startGame(GameModeGUI::HUMAN_VS_AI);
    }
    // Load Game button
    else if (x >= buttonX && x <= buttonX + buttonWidth &&
             y >= loadGameY && y <= loadGameY + buttonHeight) {
        openLoadScreen();
    }
}

void GraphicalController::handleKeyPress(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::R:
            // Restart - return to mode selection
            if (gameState) {
                showModeSelection = true;
                gameState.reset();
                resetSelection();
            }
            break;
            
        case sf::Keyboard::Key::U:
            // Undo move
            if (gameState && gameState->canUndo()) {
                gameState->undoLastMove();
                resetSelection();
                updateStatusMessage();
            }
            break;
            
        case sf::Keyboard::Key::Escape:
            // Return to main menu instead of closing window
            if (gameState) {
                // Save current game state for "Continue" feature
                savedGameState = std::make_unique<GameState>(*gameState);
                savedGameMode = currentGameMode;
                
                // Return to mode selection
                showModeSelection = true;
                gameState.reset();
                resetSelection();
                updateStatusMessage();
            }
            break;
            
        default:
            break;
    }
}

void GraphicalController::continueGame() {
    if (!savedGameState) {
        return;
    }
    
    currentGameMode = savedGameMode;
    showModeSelection = false;
    
    // Restore saved game state
    gameState = std::make_unique<GameState>(*savedGameState);
    
    // Reset selection state
    resetSelection();
    updateStatusMessage();
}

void GraphicalController::startGame(GameModeGUI mode) {
    currentGameMode = mode;
    showModeSelection = false;
    
    // Clear saved game when starting a new game
    savedGameState.reset();
    savedGameMode = GameModeGUI::NOT_SELECTED;
    
    // Create new game state
    gameState = std::make_unique<GameState>();
    gameState->initializeStandardGame();
    
    // Reset selection state
    resetSelection();
    updateStatusMessage();
    
    // AI vs AI mode has been removed from the graphical interface
}

void GraphicalController::selectAmazon(const Position& pos) {
    if (!gameState) return;
    
    Board::Cell cell = gameState->getBoard().getCell(pos.row, pos.col);
    Player currentPlayer = gameState->getCurrentPlayer();
    
    // Check if clicked on current player's amazon
    bool isCurrentPlayerAmazon = false;
    if (currentPlayer == Player::WHITE && cell == Board::Cell::WHITE_AMAZON) {
        isCurrentPlayerAmazon = true;
    } else if (currentPlayer == Player::BLACK && cell == Board::Cell::BLACK_AMAZON) {
        isCurrentPlayerAmazon = true;
    }
    
    if (!isCurrentPlayerAmazon) {
        return;
    }
    
    // Get all legal moves
    auto allMoves = gameState->getLegalMoves();
    validMoves.clear();
    
    // Filter moves from this position
    for (const auto& move : allMoves) {
        if (move.from == pos) {
            validMoves.push_back(move.to);
        }
    }
    
    if (validMoves.empty()) {
        return; // No legal moves from this position
    }
    
    selectedPosition = pos;
    selectionState = SelectionState::AMAZON_SELECTED;
    updateStatusMessage();
}

void GraphicalController::selectMoveDestination(const Position& pos) {
    // Check if clicked position is a valid move
    bool isValid = false;
    for (const auto& validPos : validMoves) {
        if (validPos == pos) {
            isValid = true;
            break;
        }
    }
    
    if (!isValid) {
        // Click on different Amazon to select it instead
        selectAmazon(pos);
        return;
    }
    
    moveToPosition = pos;
    selectionState = SelectionState::MOVE_SELECTED;
    
    // Get valid arrow positions for this move
    validArrowPositions.clear();
    auto allMoves = gameState->getLegalMoves();
    
    for (const auto& move : allMoves) {
        if (move.from == selectedPosition && move.to == pos) {
            validArrowPositions.push_back(move.arrow);
        }
    }
    
    updateStatusMessage();
}

void GraphicalController::selectArrowDestination(const Position& pos) {
    // Check if clicked position is a valid arrow position
    bool isValid = false;
    for (const auto& validPos : validArrowPositions) {
        if (validPos == pos) {
            isValid = true;
            break;
        }
    }
    
    if (!isValid) {
        return;
    }
    
    // Create and execute the move
    Move move(selectedPosition, moveToPosition, pos);
    
    makeMove(move);
}

void GraphicalController::makeMove(const Move& move) {
    if (!gameState || !gameState->isValidMove(move)) {
        return;
    }
    
    gameState->makeMove(move);
    resetSelection();
    updateStatusMessage();
    
    // If playing against AI and it's AI's turn, process AI move
    if (currentGameMode == GameModeGUI::HUMAN_VS_AI && 
        gameState->getCurrentPlayer() == Player::BLACK &&
        !gameState->isGameOver()) {
        processAIMove();
    }
    
    // AI vs AI mode has been removed from the graphical interface
}

void GraphicalController::processAIMove() {
    if (!gameState || !ai) return;
    
    // Show AI thinking status
    statusMessage = "AI is thinking...";
    
    // Process AI move asynchronously to avoid blocking UI
    std::thread([this]() {
        try {
            // Simulate AI thinking time (max 3 seconds as requested)
            auto startTime = std::chrono::steady_clock::now();
            Move aiMove = ai->getBestMove(*gameState);
            auto endTime = std::chrono::steady_clock::now();
            
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            
            // Ensure minimum thinking time for visual feedback (300ms)
            if (elapsed < std::chrono::milliseconds(300)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300) - elapsed);
            }
            
            // Execute AI move on main thread (UI thread)
            // Note: In a real implementation, we'd need proper thread synchronization
            // For now, we'll execute directly since SFML events are processed in main thread
            gameState->makeMove(aiMove);
            updateStatusMessage();
            
        } catch (const std::exception& e) {
            std::cerr << "AI error: " << e.what() << "\n";
            statusMessage = "AI error occurred";
        }
    }).detach(); // Detach thread to run independently
}

void GraphicalController::resetSelection() {
    selectionState = SelectionState::NO_SELECTION;
    selectedPosition = Position(-1, -1);
    moveToPosition = Position(-1, -1);
    validMoves.clear();
    validArrowPositions.clear();
}

void GraphicalController::updateStatusMessage() {
    if (!gameState) {
        statusMessage = "Select a game mode to start";
        return;
    }
    
    if (gameState->isGameOver()) {
        try {
            Player winner = gameState->getWinner();
            statusMessage = "Game Over! " + Display::playerToString(winner) + " wins!";
        } catch (const std::exception&) {
            statusMessage = "Game Over!";
        }
        return;
    }
    
    std::string playerStr = Display::playerToString(gameState->getCurrentPlayer());
    statusMessage = "Turn " + std::to_string(gameState->getTurnNumber()) + 
                   " - " + playerStr + "'s turn";
    
    if (selectionState == SelectionState::AMAZON_SELECTED) {
        statusMessage += " - Select destination";
    } else if (selectionState == SelectionState::MOVE_SELECTED) {
        statusMessage += " - Select arrow position";
    }
}

void GraphicalController::render() {
    window->clear(sf::Color(240, 240, 240));
    
    if (showLoadScreen) {
        drawLoadScreen();
    } else if (showModeSelection) {
        drawModeSelection();
    } else {
        drawBoard();
        drawPieces();
        drawHighlights();
        drawUI();
    }
    
    window->display();
}

void GraphicalController::drawModeSelection() {
    // Background
    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(sf::Color(44, 62, 80));
    window->draw(bg);
    
    // Title
    sf::Text title(font, "King of the Amazons", 48);
    title.setFillColor(sf::Color(236, 240, 241));
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({(WINDOW_WIDTH - titleBounds.size.x) / 2, 80});
    window->draw(title);
    
    // Subtitle
    sf::Text subtitle(font, "Graphical Mode", 24);
    subtitle.setFillColor(sf::Color(189, 195, 199));
    sf::FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition({(WINDOW_WIDTH - subtitleBounds.size.x) / 2, 150});
    window->draw(subtitle);
    
    // Get mouse position for hover effects
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    
    // Helper lambda for drawing buttons
    auto drawButton = [&](const std::string& text, float y, sf::Color baseColor) {
        const float width = 300;
        const float height = 60;
        const float x = (WINDOW_WIDTH - width) / 2;
        
        sf::RectangleShape button(sf::Vector2f(width, height));
        button.setPosition({x, y});
        
        // Hover effect
        if (mousePos.x >= x && mousePos.x <= x + width &&
            mousePos.y >= y && mousePos.y <= y + height) {
            button.setFillColor(sf::Color(baseColor.r + 20, baseColor.g + 20, baseColor.b + 20));
            button.setOutlineThickness(2);
            button.setOutlineColor(sf::Color::White);
        } else {
            button.setFillColor(baseColor);
            button.setOutlineThickness(0);
        }
        
        window->draw(button);
        
        // Button text
        sf::Text btnText(font, text, 24);
        btnText.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = btnText.getLocalBounds();
        btnText.setPosition({x + (width - textBounds.size.x) / 2, 
                           y + (height - textBounds.size.y) / 2 - 5});
        window->draw(btnText);
    };
    
    // Draw "Continue Previous Game" button if there's a saved game
    if (savedGameState) {
        drawButton("Continue Previous Game", 250, sf::Color(230, 126, 34));
        // Adjust other button positions
        drawButton("Human vs Human", 350, sf::Color(52, 152, 219));
        drawButton("Human vs AI", 450, sf::Color(46, 204, 113));
        // Load Game button
        drawButton("Load Game", 550, sf::Color(155, 89, 182));
    } else {
        // Original button positions when no saved game
        drawButton("Human vs Human", 250, sf::Color(52, 152, 219));
        drawButton("Human vs AI", 350, sf::Color(46, 204, 113));
        // Load Game button
        drawButton("Load Game", 450, sf::Color(155, 89, 182));
    }
    
    // Calculate Y position for instructions based on last button position
    // Button height is 60 pixels, add 30 pixels spacing
    const int buttonHeight = 60;
    const int spacing = 30;
    
    int lastButtonY = savedGameState ? 550 : 450; // Last button Y position (Load Game)
    int instructionsY = lastButtonY + buttonHeight + spacing;
    
    // Instructions
    sf::Text instructions(font, "Select a game mode to start", 18);
    instructions.setFillColor(sf::Color(189, 195, 199));
    sf::FloatRect instBounds = instructions.getLocalBounds();
    instructions.setPosition({(WINDOW_WIDTH - instBounds.size.x) / 2, static_cast<float>(instructionsY)});
    window->draw(instructions);
    
    // Note: ESC hint removed from main menu - it only appears during gameplay
    // in the drawUI() function as part of the instructions line
}

void GraphicalController::drawBoard() {
    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(sf::Vector2f(BOARD_OFFSET_X + col * CELL_SIZE, 
                           BOARD_OFFSET_Y + row * CELL_SIZE));
            cell.setFillColor(getCellColor(row, col));
            cell.setOutlineColor(sf::Color(150, 150, 150));
            cell.setOutlineThickness(1);
            window->draw(cell);
        }
    }
}

void GraphicalController::drawPieces() {
    if (!gameState) return;
    
    const Board& board = gameState->getBoard();
    
    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            Board::Cell cell = board.getCell(row, col);
            float cx = BOARD_OFFSET_X + col * CELL_SIZE + CELL_SIZE / 2.0f;
            float cy = BOARD_OFFSET_Y + row * CELL_SIZE + CELL_SIZE / 2.0f;
            
            if (cell == Board::Cell::WHITE_AMAZON || cell == Board::Cell::BLACK_AMAZON) {
                float radius = CELL_SIZE / 2.0f - 8.0f;
                sf::CircleShape amazon(radius);
                amazon.setOrigin({radius, radius});
                amazon.setPosition({cx, cy});
                
                if (cell == Board::Cell::WHITE_AMAZON) {
                    amazon.setFillColor(sf::Color(245, 245, 240));
                    amazon.setOutlineColor(sf::Color(80, 80, 80));
                    amazon.setOutlineThickness(2);
                } else {
                    amazon.setFillColor(sf::Color(50, 50, 50));
                    amazon.setOutlineColor(sf::Color(20, 20, 20));
                    amazon.setOutlineThickness(2);
                }
                
                window->draw(amazon);
                
            } else if (cell == Board::Cell::ARROW) {
                float radius = CELL_SIZE / 5.0f;
                sf::CircleShape arrow(radius);
                arrow.setOrigin({radius, radius});
                arrow.setPosition({cx, cy});
                arrow.setFillColor(sf::Color(192, 57, 43));
                arrow.setOutlineColor(sf::Color(146, 43, 33));
                arrow.setOutlineThickness(1);
                window->draw(arrow);
            }
        }
    }
}

void GraphicalController::drawHighlights() {
    if (!gameState) return;
    
    // Hover highlight
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    auto hoverPos = getBoardPosition(mousePos.x, mousePos.y);
    
    if (hoverPos.has_value()) {
        sf::RectangleShape hoverHighlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        hoverHighlight.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + hoverPos->col * CELL_SIZE,
            BOARD_OFFSET_Y + hoverPos->row * CELL_SIZE
        ));
        hoverHighlight.setFillColor(sf::Color(255, 255, 255, 50));
        window->draw(hoverHighlight);
    }

    // Highlight selected Amazon
    if (selectionState == SelectionState::AMAZON_SELECTED && 
        selectedPosition.row >= 0 && selectedPosition.col >= 0) {
        sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        highlight.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + selectedPosition.col * CELL_SIZE,
            BOARD_OFFSET_Y + selectedPosition.row * CELL_SIZE
        ));
        highlight.setFillColor(sf::Color(255, 255, 0, 100));
        window->draw(highlight);
        
        // Highlight valid moves
        for (const auto& pos : validMoves) {
            sf::RectangleShape moveHighlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            moveHighlight.setPosition(sf::Vector2f(
                BOARD_OFFSET_X + pos.col * CELL_SIZE,
                BOARD_OFFSET_Y + pos.row * CELL_SIZE
            ));
            moveHighlight.setFillColor(sf::Color(0, 255, 0, 80));
            window->draw(moveHighlight);
        }
    }
    
    // Highlight move destination
    if (selectionState == SelectionState::MOVE_SELECTED && 
        moveToPosition.row >= 0 && moveToPosition.col >= 0) {
        sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        highlight.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + moveToPosition.col * CELL_SIZE,
            BOARD_OFFSET_Y + moveToPosition.row * CELL_SIZE
        ));
        highlight.setFillColor(sf::Color(0, 0, 255, 100));
        window->draw(highlight);
        
        // Highlight valid arrow positions
        for (const auto& pos : validArrowPositions) {
            sf::RectangleShape arrowHighlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            arrowHighlight.setPosition(sf::Vector2f(
                BOARD_OFFSET_X + pos.col * CELL_SIZE,
                BOARD_OFFSET_Y + pos.row * CELL_SIZE
            ));
            arrowHighlight.setFillColor(sf::Color(255, 0, 0, 80));
            window->draw(arrowHighlight);
        }
    }
}

void GraphicalController::drawUI() {
    // Status message
    sf::Text status(font, statusMessage, 20);
    status.setFillColor(sf::Color::Black);
    status.setPosition({10.f, 15.f});
    window->draw(status);
    
    // Instructions
    std::string instructions = "R: Restart | U: Undo | ESC: Menu";
    if (selectionState == SelectionState::AMAZON_SELECTED) {
        instructions += " | Click on highlighted green cells to move";
    } else if (selectionState == SelectionState::MOVE_SELECTED) {
        instructions += " | Click on highlighted red cells for arrow";
    }
    
    sf::Text instrText(font, instructions, 14);
    instrText.setFillColor(sf::Color(80, 80, 80));
    instrText.setPosition({10.f, 45.f});
    window->draw(instrText);
    
    // Game mode indicator
    if (gameState) {
        std::string modeStr;
        switch (currentGameMode) {
            case GameModeGUI::HUMAN_VS_HUMAN:
                modeStr = "Mode: Human vs Human";
                break;
            case GameModeGUI::HUMAN_VS_AI:
                modeStr = "Mode: Human vs AI";
                break;
            default:
                modeStr = "";
        }
        
        if (!modeStr.empty()) {
            sf::Text modeText(font, modeStr, 16);
            modeText.setFillColor(sf::Color(100, 100, 100));
            modeText.setPosition({WINDOW_WIDTH - 250.f, 15.f});
            window->draw(modeText);
        }
    }
    
    // Draw Save Game button
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::RectangleShape saveButton(sf::Vector2f(SAVE_BUTTON_WIDTH, SAVE_BUTTON_HEIGHT));
    saveButton.setPosition({SAVE_BUTTON_X, SAVE_BUTTON_Y});
    
    bool isHovered = mousePos.x >= SAVE_BUTTON_X && mousePos.x <= SAVE_BUTTON_X + SAVE_BUTTON_WIDTH &&
                    mousePos.y >= SAVE_BUTTON_Y && mousePos.y <= SAVE_BUTTON_Y + SAVE_BUTTON_HEIGHT;
    
    if (isHovered) {
        saveButton.setFillColor(sf::Color(46, 204, 113));
        saveButton.setOutlineThickness(2);
        saveButton.setOutlineColor(sf::Color::White);
    } else {
        saveButton.setFillColor(sf::Color(39, 174, 96));
    }
    
    window->draw(saveButton);
    
    sf::Text saveText(font, "Save Game", 16);
    saveText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = saveText.getLocalBounds();
    saveText.setPosition({SAVE_BUTTON_X + (SAVE_BUTTON_WIDTH - textBounds.size.x) / 2, 
                        SAVE_BUTTON_Y + (SAVE_BUTTON_HEIGHT - textBounds.size.y) / 2 - 5});
    window->draw(saveText);
}

sf::Color GraphicalController::getCellColor(int row, int col) const {
    // Checkerboard pattern
    if ((row + col) % 2 == 0) {
        return sf::Color(238, 238, 210); // Light cream
    } else {
        return sf::Color(118, 150, 86); // Soft green
    }
}

std::optional<Position> GraphicalController::getBoardPosition(int mouseX, int mouseY) const {
    if (mouseX < BOARD_OFFSET_X || mouseX >= BOARD_OFFSET_X + BOARD_SIZE ||
        mouseY < BOARD_OFFSET_Y || mouseY >= BOARD_OFFSET_Y + BOARD_SIZE) {
        return std::nullopt;
    }
    
    int col = (mouseX - BOARD_OFFSET_X) / CELL_SIZE;
    int row = (mouseY - BOARD_OFFSET_Y) / CELL_SIZE;
    
    if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
        return Position(row, col);
    }
    
    return std::nullopt;
}

void GraphicalController::saveCurrentGame() {
    if (!gameState) {
        statusMessage = "No game to save!";
        return;
    }
    
    // Generate timestamp-based filename
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time);
    
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "game_%Y%m%d_%H%M%S", &tm);
    std::string filename(buffer);
    
    // Convert GameModeGUI to GameMode
    GameMode gameMode;
    switch (currentGameMode) {
        case GameModeGUI::HUMAN_VS_HUMAN:
            gameMode = GameMode::HUMAN_VS_HUMAN;
            break;
        case GameModeGUI::HUMAN_VS_AI:
            gameMode = GameMode::HUMAN_VS_AI;
            break;
        default:
            gameMode = GameMode::HUMAN_VS_HUMAN;
    }
    
    if (serializer.saveGame(*gameState, gameMode, filename)) {
        statusMessage = "Game saved: " + filename;
    } else {
        statusMessage = "Failed to save game!";
    }
}

void GraphicalController::loadSavedGamesList() {
    savedGamesList = serializer.getSavedGames();
}

void GraphicalController::openLoadScreen() {
    loadSavedGamesList();
    showLoadScreen = true;
    showModeSelection = false;
    selectedSaveIndex = -1;
    scrollOffset = 0;  // Reset scroll to top
}

void GraphicalController::handleLoadScreenClick(int x, int y) {
    const int buttonWidth = 400;
    const int buttonHeight = 40;
    const int buttonX = (WINDOW_WIDTH - buttonWidth) / 2;
    
    // Fixed positions for action buttons
    const int loadButtonY = WINDOW_HEIGHT - 150;
    const int backButtonY = WINDOW_HEIGHT - 100;
    
    // Scroll button positions (right side of the list)
    const int scrollX = buttonX + buttonWidth + 10;
    const int scrollY = 200;
    const int scrollButtonSize = SCROLL_BUTTON_SIZE;
    
    // Check scroll down button (bottom)
    if (x >= scrollX && x <= scrollX + scrollButtonSize &&
        y >= scrollY + scrollButtonSize && y <= scrollY + scrollButtonSize * 2) {
        // Scroll down
        int maxOffset = static_cast<int>(savedGamesList.size()) - MAX_VISIBLE_SAVES;
        if (maxOffset > 0 && scrollOffset < maxOffset) {
            scrollOffset++;
        }
        return;
    }
    
    // Check scroll up button (top)
    if (x >= scrollX && x <= scrollX + scrollButtonSize &&
        y >= scrollY && y <= scrollY + scrollButtonSize) {
        // Scroll up
        if (scrollOffset > 0) {
            scrollOffset--;
        }
        return;
    }
    
    // Check Back button
    if (x >= buttonX && x <= buttonX + buttonWidth &&
        y >= backButtonY && y <= backButtonY + buttonHeight) {
        showLoadScreen = false;
        showModeSelection = true;
        return;
    }
    
    // Check if clicked on a save game entry (only visible ones)
    int startY = 200;
    int spacing = 50;
    
    for (int i = scrollOffset; i < scrollOffset + MAX_VISIBLE_SAVES && i < static_cast<int>(savedGamesList.size()); ++i) {
        int yPos = startY + (i - scrollOffset) * spacing;
        if (x >= buttonX && x <= buttonX + buttonWidth &&
            y >= yPos && y <= yPos + buttonHeight) {
            selectedSaveIndex = i;
            return;
        }
    }
    
    // Check Load button (only if a save is selected)
    if (selectedSaveIndex >= 0 && selectedSaveIndex < static_cast<int>(savedGamesList.size())) {
        if (x >= buttonX && x <= buttonX + buttonWidth &&
            y >= loadButtonY && y <= loadButtonY + buttonHeight) {
            // Load the selected game
            std::string selectedSave = savedGamesList[selectedSaveIndex];
            auto result = serializer.loadGameWithMode(selectedSave);
            
            if (result.first) {
                gameState = std::move(result.first);
                
                // Convert GameMode to GameModeGUI
                switch (result.second) {
                    case GameMode::HUMAN_VS_HUMAN:
                        currentGameMode = GameModeGUI::HUMAN_VS_HUMAN;
                        break;
                    case GameMode::HUMAN_VS_AI:
                        currentGameMode = GameModeGUI::HUMAN_VS_AI;
                        break;
                    default:
                        currentGameMode = GameModeGUI::HUMAN_VS_HUMAN;
                }
                
                showLoadScreen = false;
                showModeSelection = false;
                resetSelection();
                updateStatusMessage();
                statusMessage = "Game loaded: " + selectedSave;
            } else {
                statusMessage = "Failed to load game!";
            }
        }
    }
}

void GraphicalController::drawLoadScreen() {
    // Background
    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(sf::Color(44, 62, 80));
    window->draw(bg);
    
    // Title
    sf::Text title(font, "Load Saved Game", 36);
    title.setFillColor(sf::Color(236, 240, 241));
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({(WINDOW_WIDTH - titleBounds.size.x) / 2, 80});
    window->draw(title);
    
    // Get mouse position for hover effects
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    
    // Draw saved games list - only visible portion
    const int buttonWidth = 400;
    const int buttonHeight = 40;
    const int buttonX = (WINDOW_WIDTH - buttonWidth) / 2;
    int startY = 200;
    int spacing = 50;
    
    // Scroll button positions
    const int scrollX = buttonX + buttonWidth + 10;
    const int scrollY = 200;
    const int scrollButtonSize = SCROLL_BUTTON_SIZE;
    
    if (savedGamesList.empty()) {
        sf::Text noSaves(font, "No saved games found", 20);
        noSaves.setFillColor(sf::Color(189, 195, 199));
        sf::FloatRect bounds = noSaves.getLocalBounds();
        noSaves.setPosition({(WINDOW_WIDTH - bounds.size.x) / 2, 200});
        window->draw(noSaves);
    } else {
        // Only draw visible saves (respect scroll offset)
        int visibleCount = 0;
        for (int i = scrollOffset; i < scrollOffset + MAX_VISIBLE_SAVES && i < static_cast<int>(savedGamesList.size()); ++i) {
            int yPos = startY + visibleCount * spacing;
            
            sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
            button.setPosition({static_cast<float>(buttonX), static_cast<float>(yPos)});
            
            bool isHovered = mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
                            mousePos.y >= yPos && mousePos.y <= yPos + buttonHeight;
            bool isSelected = i == selectedSaveIndex;
            
            if (isSelected) {
                button.setFillColor(sf::Color(46, 204, 113));
                button.setOutlineThickness(2);
                button.setOutlineColor(sf::Color::White);
            } else if (isHovered) {
                button.setFillColor(sf::Color(52, 152, 219));
                button.setOutlineThickness(2);
                button.setOutlineColor(sf::Color::White);
            } else {
                button.setFillColor(sf::Color(52, 73, 94));
                button.setOutlineThickness(0);
            }
            
            window->draw(button);
            
            // Button text
            sf::Text btnText(font, savedGamesList[i], 18);
            btnText.setFillColor(sf::Color::White);
            sf::FloatRect textBounds = btnText.getLocalBounds();
            btnText.setPosition({buttonX + (buttonWidth - textBounds.size.x) / 2, 
                               static_cast<float>(yPos + (buttonHeight - textBounds.size.y) / 2 - 5)});
            window->draw(btnText);
            
            visibleCount++;
        }
        
        // Draw scroll buttons if there are more saves than visible
        int maxOffset = static_cast<int>(savedGamesList.size()) - MAX_VISIBLE_SAVES;
        
        if (maxOffset > 0) {
            // Draw scroll up button (▲)
            sf::RectangleShape upButton(sf::Vector2f(scrollButtonSize, scrollButtonSize));
            upButton.setPosition({static_cast<float>(scrollX), static_cast<float>(scrollY)});
            bool isUpHovered = mousePos.x >= scrollX && mousePos.x <= scrollX + scrollButtonSize &&
                              mousePos.y >= scrollY && mousePos.y <= scrollY + scrollButtonSize;
            
            if (scrollOffset > 0) {
                if (isUpHovered) {
                    upButton.setFillColor(sf::Color(52, 152, 219));
                    upButton.setOutlineThickness(2);
                    upButton.setOutlineColor(sf::Color::White);
                } else {
                    upButton.setFillColor(sf::Color(41, 128, 185));
                }
            } else {
                upButton.setFillColor(sf::Color(100, 100, 100)); // Disabled
            }
            window->draw(upButton);
            
            sf::Text upText(font, "^", 20);
            upText.setFillColor(sf::Color::White);
            sf::FloatRect upBounds = upText.getLocalBounds();
            upText.setPosition({scrollX + (scrollButtonSize - upBounds.size.x) / 2 - 3, 
                              scrollY + (scrollButtonSize - upBounds.size.y) / 2 - 8});
            window->draw(upText);
            
            // Draw scroll down button (▼)
            sf::RectangleShape downButton(sf::Vector2f(scrollButtonSize, scrollButtonSize));
            downButton.setPosition({static_cast<float>(scrollX), static_cast<float>(scrollY + scrollButtonSize)});
            bool isDownHovered = mousePos.x >= scrollX && mousePos.x <= scrollX + scrollButtonSize &&
                                mousePos.y >= scrollY + scrollButtonSize && mousePos.y <= scrollY + scrollButtonSize * 2;
            
            if (scrollOffset < maxOffset) {
                if (isDownHovered) {
                    downButton.setFillColor(sf::Color(52, 152, 219));
                    downButton.setOutlineThickness(2);
                    downButton.setOutlineColor(sf::Color::White);
                } else {
                    downButton.setFillColor(sf::Color(41, 128, 185));
                }
            } else {
                downButton.setFillColor(sf::Color(100, 100, 100)); // Disabled
            }
            window->draw(downButton);
            
            sf::Text downText(font, "v", 20);
            downText.setFillColor(sf::Color::White);
            sf::FloatRect downBounds = downText.getLocalBounds();
            downText.setPosition({scrollX + (scrollButtonSize - downBounds.size.x) / 2 - 3, 
                                scrollY + scrollButtonSize + (scrollButtonSize - downBounds.size.y) / 2 - 8});
            window->draw(downText);
            
            // Draw scroll position indicator
            std::string scrollInfo = "(" + std::to_string(scrollOffset + 1) + "-" + 
                                     std::to_string(scrollOffset + visibleCount) + "/" + 
                                     std::to_string(savedGamesList.size()) + ")";
            sf::Text scrollInfoText(font, scrollInfo, 14);
            scrollInfoText.setFillColor(sf::Color(189, 195, 199));
            scrollInfoText.setPosition({static_cast<float>(scrollX), static_cast<float>(scrollY + scrollButtonSize * 2 + 5)});
            window->draw(scrollInfoText);
        }
    }
    
    // Load button (only show if a save is selected)
    if (selectedSaveIndex >= 0 && selectedSaveIndex < static_cast<int>(savedGamesList.size())) {
        int loadButtonY = WINDOW_HEIGHT - 150;
        
        sf::RectangleShape loadButton(sf::Vector2f(buttonWidth, buttonHeight));
        loadButton.setPosition({static_cast<float>(buttonX), static_cast<float>(loadButtonY)});
        
        bool isHovered = mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
                        mousePos.y >= loadButtonY && mousePos.y <= loadButtonY + buttonHeight;
        
        if (isHovered) {
            loadButton.setFillColor(sf::Color(46, 204, 113));
            loadButton.setOutlineThickness(2);
            loadButton.setOutlineColor(sf::Color::White);
        } else {
            loadButton.setFillColor(sf::Color(39, 174, 96));
        }
        
        window->draw(loadButton);
        
        sf::Text loadText(font, "Load Selected Game", 20);
        loadText.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = loadText.getLocalBounds();
        loadText.setPosition({buttonX + (buttonWidth - textBounds.size.x) / 2, 
                            static_cast<float>(loadButtonY + (buttonHeight - textBounds.size.y) / 2 - 5)});
        window->draw(loadText);
    }
    
    // Back button
    int backButtonY = WINDOW_HEIGHT - 100;
    sf::RectangleShape backButton(sf::Vector2f(buttonWidth, buttonHeight));
    backButton.setPosition({static_cast<float>(buttonX), static_cast<float>(backButtonY)});
    
    bool isBackHovered = mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
                        mousePos.y >= backButtonY && mousePos.y <= backButtonY + buttonHeight;
    
    if (isBackHovered) {
        backButton.setFillColor(sf::Color(231, 76, 60));
        backButton.setOutlineThickness(2);
        backButton.setOutlineColor(sf::Color::White);
    } else {
        backButton.setFillColor(sf::Color(192, 57, 43));
    }
    
    window->draw(backButton);
    
    sf::Text backText(font, "Back to Menu", 20);
    backText.setFillColor(sf::Color::White);
    sf::FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition({buttonX + (buttonWidth - backBounds.size.x) / 2, 
                        static_cast<float>(backButtonY + (buttonHeight - backBounds.size.y) / 2 - 5)});
    window->draw(backText);
}

} // namespace amazons
