# King of the Amazons - Game Manual

## Introduction

Welcome to **King of the Amazons**, a C++ implementation of the classic abstract strategy board game "Game of the Amazons" (also known as "Queen's Game"). This project is a university assignment that demonstrates comprehensive C++ programming skills, game development principles, and artificial intelligence implementation.

### Current Status
- **Phase**: Phase 2 Complete (Complete Game System)
- **Version**: 1.0.0
- **Last Updated**: December 18, 2025
- **Overall Completion**: 50% (Phase 1 & 2 complete, ready for Phase 3)

## Getting Started

### System Requirements
- **Operating System**: Windows, macOS, or Linux
- **C++ Compiler**: GCC ≥ 9.0, Clang ≥ 10.0, or MSVC ≥ 2019
- **Build System**: CMake ≥ 3.16
- **Memory**: Minimum 512MB RAM
- **Storage**: 50MB free space

### Installation & Building

#### Using CMake (Recommended)
```bash
# Clone the repository (if applicable)
# git clone <repository-url>

# Navigate to project directory
cd king-of-the-amazons

# Create build directory
mkdir build
cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### Using Makefile (Alternative)
```bash
make all          # Build everything
make test         # Run tests
make clean        # Clean build artifacts
```

### Running the Game
```bash
# After building
./build/amazons   # Or equivalent executable name
```

## Game Rules

### Overview
The Game of Amazons is a two-player abstract strategy game played on a 10×10 board. Each player controls 4 Amazons (queen-like pieces). The goal is to control more territory than your opponent by limiting their movement options.

### Board Layout
- **Size**: 10×10 grid
- **Coordinates**: Rows (0-9) and columns (0-9)
- **Starting Positions**:
  - **Black Amazons (B)**: (0,3), (0,6), (3,0), (3,9)
  - **White Amazons (W)**: (6,0), (6,9), (9,3), (9,6)

### Turn Structure
Each turn consists of **two parts**:
1. **Amazon Move**: Move one of your Amazons like a chess queen (any distance in 8 directions)
2. **Arrow Shot**: From the new position, shoot an arrow like a chess queen (any distance, 8 directions)

### Movement Rules
1. **Amazon Movement**:
   - Move like a queen in chess (horizontal, vertical, or diagonal)
   - Any distance as long as the path is clear
   - Cannot land on occupied squares (other Amazons or arrows)

2. **Arrow Shooting**:
   - Must shoot from the Amazon's new position
   - Same movement rules as Amazon movement
   - Arrow becomes a permanent obstacle on the board
   - Cannot shoot through occupied squares

3. **Restrictions**:
   - Cannot move through or land on occupied squares
   - Cannot shoot through occupied squares
   - Arrow must land on an empty square

### Winning Conditions
- The game ends when a player cannot make a legal move
- The **winner** is the player with more **territory** (reachable squares)
- If both players have equal territory, the game is a draw

### Territory Calculation
- **Territory**: All squares reachable by a player's Amazons
- **Reachable**: Squares that can be reached through queen-like movement
- **Blocked**: Squares occupied by Amazons, arrows, or outside the board

## User Interface Guide

### Main Menu
When you start the game, you'll see the main menu:

```
=== King of the Amazons ===
Phase 2: Complete Game System
=============================
1. New Game
2. Load Game
3. Save Game
4. Exit

Select an option (1-4):
```

**Options**:
1. **New Game**: Start a new game with various modes
2. **Load Game**: Load a previously saved game
3. **Save Game**: Save the current game in progress
4. **Exit**: Quit the application

### New Game Menu
Selecting "New Game" presents additional options:

```
=== New Game ===
1. Human vs Human
2. Human vs AI
3. AI vs AI
4. Back to Main Menu

Select game mode (1-4):
```

**Game Modes**:
1. **Human vs Human**: Two human players take turns
2. **Human vs AI**: Human plays as White, AI plays as Black
3. **AI vs AI**: Watch two AI opponents play (limited to 200 moves)

### Board Display
The game board is displayed with coordinates and symbols:

```
=== King of the Amazons ===
Turn: 1
Current player: White

   0  1  2  3  4  5  6  7  8  9 
0  .  .  .  B  .  .  B  .  .  .  0
1  .  .  .  .  .  .  .  .  .  .  1
2  .  .  .  .  .  .  .  .  .  .  2
3  B  .  .  .  .  .  .  .  .  B  3
4  .  .  .  .  .  .  .  .  .  .  4
5  .  .  .  .  .  .  .  .  .  .  5
6  W  .  .  .  .  .  .  .  .  W  6
7  .  .  .  .  .  .  .  .  .  .  7
8  .  .  .  .  .  .  .  .  .  .  8
9  .  .  .  W  .  .  W  .  .  .  9
   0  1  2  3  4  5  6  7  8  9 
```

**Symbols**:
- `.` : Empty square
- `W` : White Amazon
- `B` : Black Amazon
- `X` : Arrow (blocked square)

**Coordinates**:
- Rows: 0-9 (top to bottom)
- Columns: 0-9 (left to right)
- Position format: `(row,column)`

### Input Format
When it's your turn, you'll be prompted:

```
White's turn.
Enter your move as 6 numbers: from_row from_col to_row to_col arrow_row arrow_col
Or enter 'help' to see legal moves, or 'undo' to undo last move:
```

**Move Format**: `from_row from_col to_row to_col arrow_row arrow_col`
- `from_row from_col`: Starting position of your Amazon (row col)
- `to_row to_col`: Destination for the Amazon move (row col)
- `arrow_row arrow_col`: Destination for the arrow shot (row col)

**Example**: `9 6 8 5 7 4`
- Move Amazon from (9,6) to (8,5)
- Shoot arrow from (8,5) to (7,4)

### Special Commands
During gameplay, you can use these commands:

| Command | Aliases | Description |
|---------|---------|-------------|
| `help` | `h` | Show all legal moves for current position |
| `undo` | `u` | Undo the last move (if available) |
| `exit` | `quit`, `q` | Exit the current game and return to main menu |

## Game Modes Detailed

### Human vs Human
**Description**: Classic two-player gameplay
**Turn Order**: White moves first
**Features**:
- Full move validation
- Undo functionality
- Help command for legal moves
- Complete game flow with win detection

**Example Gameplay**:
```
White's turn.
Enter your move: 9 6 8 5 7 4
Move made: 9 6 8 5 7 4

Black's turn.
Enter your move: 0 3 1 4 2 5
Move made: 0 3 1 4 2 5
```

### Human vs AI
**Description**: Play against computer opponent
**Player Roles**:
- **Human**: Plays as White (moves first)
- **AI**: Plays as Black (greedy algorithm)

**AI Characteristics**:
- **Algorithm**: Greedy with mobility heuristic
- **Strategy**: Maximizes own mobility while minimizing opponent's
- **Speed**: Makes decisions with small delay for realism
- **Difficulty**: Basic level (Phase 2 implementation)

**Example Gameplay**:
```
White's turn (Human).
Enter your move: 9 6 8 5 7 4
Move made: 9 6 8 5 7 4

Black's turn (AI).
AI is thinking...
AI made move: 0 3 1 4 2 5
```

### AI vs AI
**Description**: Watch two AI opponents play
**Features**:
- Automated gameplay
- Move limit: 200 moves (safety precaution)
- Delay between moves for visibility
- Game result display

**Example Output**:
```
White AI's turn.
AI is thinking...
AI made move: 9 6 8 5 7 4

Black AI's turn.
AI is thinking...
AI made move: 0 3 1 4 2 5
```

## Save/Load System

### Saving Games
1. From main menu, select **"3. Save Game"**
2. Enter a name for your save file
3. If a save with that name exists, you'll be asked to overwrite
4. Game is saved to `data/saves/<name>.json`

**Example**:
```
=== Save Game ===
Existing saved games:
  test

Enter a name for your save (or press Enter to cancel): mygame
Game saved successfully as 'mygame'.
```

### Loading Games
1. From main menu, select **"2. Load Game"**
2. View list of available saves
3. Select a game by number (0 to go back)
4. Game loads and continues from saved state

**Example**:
```
=== Load Game ===
Saved games:
  1. mygame
  2. test
  0. Back to Main Menu

Select a game to load (0-2): 1
Game loaded successfully.
```

### Save File Format
Games are saved in JSON format for readability:
```json
{
  "game_state": {
    "turn": 1,
    "current_player": "WHITE",
    "board": "..."
  }
}
```

## Advanced Features

### Undo System
- **Command**: `undo` or `u`
- **Limitation**: Can only undo moves that have been made
- **Multiple Undos**: Not supported in current version
- **Availability**: Shown in prompt when undo is possible

### Help System
- **Command**: `help` or `h`
- **Output**: Lists all legal moves for current position
- **Format**: Shows moves in standard `from_row from_col to_row to_col arrow_row arrow_col` format
- **Empty List**: Shows "No legal moves available" if game is over

### Error Handling
The game provides helpful error messages:

| Error Type | Example Message | Solution |
|------------|-----------------|----------|
| Invalid Format | `Invalid move format: expected 6 numbers` | Use correct format: `from_row from_col to_row to_col arrow_row arrow_col` (6 numbers) |
| Illegal Move | `Invalid move: Move is not legal in current position.` | Check if path is clear and positions are valid |
| Save Error | `Failed to save game.` | Check write permissions in `data/saves/` directory |
| Load Error | `Failed to load game.` | Save file may be corrupted or missing |

## Troubleshooting

### Common Issues

#### 1. Build Errors
**Problem**: CMake or make fails
**Solutions**:
- Ensure CMake ≥ 3.16 is installed
- Check C++ compiler compatibility
- Clean build directory and rebuild: `rm -rf build && mkdir build && cd build && cmake .. && make`

#### 2. Runtime Errors
**Problem**: Game crashes or doesn't start
**Solutions**:
- Verify all dependencies are installed
- Check `data/saves/` directory exists and is writable
- Run with debug: `./build/amazons 2>&1 | tee error.log`

#### 3. Input Issues
**Problem**: Moves not accepted
**Solutions**:
- Use exact format: `from_row from_col to_row to_col arrow_row arrow_col` (6 numbers)
- Ensure coordinates are within 0-9 range
- Check that path is not blocked by arrows or other Amazons

#### 4. Save/Load Problems
**Problem**: Cannot save or load games
**Solutions**:
- Check `data/saves/` directory permissions
- Ensure enough disk space
- Verify JSON files are not manually edited/corrupted

### Performance Tips
- **AI Games**: AI vs AI mode may be slow with many moves
- **Large Games**: Games with many arrows may have slower move generation
- **Memory**: Undo history is stored in memory

## Development Information

### Project Structure
```
king-of-the-amazons/
├── src/                    # Source code
│   ├── core/              # Core game logic (Board, GameState, Move, etc.)
│   ├── ai/                # AI implementations (BasicAI)
│   ├── ui/                # User interface (MenuController, TextDisplay)
│   ├── utils/             # Utilities (Serializer)
│   └── features/          # Enhanced features (future)
├── include/               # Header files
├── tests/                 # Test suite
├── docs/                  # Documentation
├── memorybank/            # Project memory and context
├── data/                  # Game data (saves, config, logs)
└── scripts/               # Build and utility scripts
```

### Testing
Run the test suite:
```bash
cd build
./bin/unit_tests
```

### Memory Bank Documentation
The project maintains comprehensive documentation in `memorybank/`:
- `projectbrief.md`: Project goals and requirements
- `productContext.md`: User experience and problem statement
- `systemPatterns.md`: Architecture and design patterns
- `techContext.md`: Technologies and setup
- `activeContext.md`: Current work and recent changes
- `progress.md`: Status tracking and accomplishments

## Game Strategy Tips

### Basic Strategy
1. **Control Center**: Central squares are more valuable
2. **Mobility**: Keep your Amazons mobile
3. **Blocking**: Use arrows to limit opponent's movement
4. **Territory**: Think in terms of reachable squares

### Opening Principles
1. **Develop Amazons**: Move Amazons to active positions
2. **Control Space**: Use arrows to claim territory
3. **Maintain Options**: Keep multiple movement paths open
4. **Watch Corners**: Corners can trap Amazons

### Common Tactics
1. **Arrow Placement**: Place arrows to create barriers
2. **Fork Threats**: Position Amazons to threaten multiple areas
3. **Sacrifice**: Sometimes sacrificing mobility gains territory
4. **Endgame**: Count reachable squares carefully

## Frequently Asked Questions

### Q: What is the Game of Amazons?
**A**: It's an abstract strategy game invented in 1988 by Walter Zamkauskas. It combines elements of chess (queen movement) with territory control.

### Q: How is the winner determined?
**A**: The player with more reachable squares (territory) when no legal moves remain wins.

### Q: Can I play against a friend on the same computer?
**A**: Yes! Use "Human vs Human" mode for local two-player gameplay.

### Q: How good is the AI?
**A**: The Phase 2 AI uses a greedy algorithm with mobility heuristic. It's competent but not expert-level. Advanced AI algorithms are planned for Phase 3.

### Q: Can I save mid-game and continue later?
**A**: Yes! Use the save/load system from the main menu.

### Q: What happens if I enter an invalid move?
**A**: The game will show an error message and prompt you to try again.

### Q: Is there a time limit for moves?
**A**: No, you can take as long as you need for each move.

### Q: Can I undo multiple moves?
**A**: Currently, only the last move can be undone.

## Version History

### Phase 1 (Dec 17-23, 2025) - Core Foundation
- ✅ Project setup with CMake build system
- ✅ Core game components (Board, GameState, Move, Player)
- ✅ Text-based user interface
- ✅ Human vs Human gameplay
- ✅ 30 unit tests passing

### Phase 2 (Dec 24-30, 2025) - Complete Game System
- ✅ Enhanced menu system (New Game, Load Game, Save Game, Exit)
- ✅ Save/load functionality with JSON serialization
- ✅ Basic AI opponent (greedy algorithm)
- ✅ Human vs AI and AI vs AI modes
- ✅ Complete game flow integration
- ✅ Ready for First Review Session (Dec 30, 2025)

### Phase 3 (Planned) - Advanced AI and Features
- Advanced AI algorithms (Minimax with alpha-beta pruning)
- Enhanced features (game analysis, improved undo/redo)
- Performance optimization

### Phase 4 (Planned) - Polish and Finalization
- Comprehensive testing and quality assurance
- Enhanced features polish (statistics, replay system)
- User interface refinement

## Credits & License

### Development
- **Project**: King of the Amazons
- **Purpose**: University assignment for computational introduction course
- **Language**: C++17
- **Architecture**: MVC pattern implementation

### Educational Use
This project serves as:
1. Learning example for C++ programming
2. Demonstration of software engineering principles
3. Reference for game development concepts
4. AI algorithm implementation showcase

### License
Educational use only - part of university coursework.

---

**Need Help?** Refer to the troubleshooting section or check the
