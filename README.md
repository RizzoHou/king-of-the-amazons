# King of the Amazons

A C++ implementation of the "Game of the Amazons" (also known as "Amazons" or "Queen's Game"), a two-player abstract strategy board game. This project serves as a university assignment for a computational introduction course.

## Project Status

**Current Phase**: Project Initialization Complete  
**Overall Completion**: 5% (Documentation complete, implementation not started)

### What Works
- Comprehensive memory bank documentation
- Project architecture planning (MVC pattern)
- Technical requirements analysis
- Development workflow establishment

### What's Next
- Create project structure (CMakeLists.txt, directories)
- Implement core game components (Board, GameState, Player classes)
- Develop basic text-based user interface
- Implement game logic and move validation

## Requirements

### Basic Functionality (6 points)
- [ ] Menu system (new game, load game, save game, exit)
- [ ] Board and piece display (character-based or graphical)
- [ ] Complete game flow with ability to stop mid-game
- [ ] Save/load functionality for game states

### AI Capability (1 point)
- [ ] Implementation of computer opponent using game AI algorithms
- [ ] Documentation of AI approach and algorithms

### Special Features (1.5 points)
- [ ] User-friendly interface and ease of use
- [ ] Aesthetic presentation and interface design
- [ ] Feature completeness and integration
- [ ] Additional innovative features

### Botzone Competition (1.5 points)
- [ ] Participation in online bot tournaments
- [ ] Three test matches and one formal competition

## Project Structure

```
king-of-the-amazons/
├── src/                    # Source code
│   ├── core/              # Core game logic
│   ├── ai/                # AI implementations
│   ├── ui/                # User interface
│   └── utils/             # Utility functions
├── include/               # Header files
├── tests/                 # Test files
├── docs/                  # Documentation
├── memorybank/            # Project memory bank
├── CMakeLists.txt         # CMake configuration
└── README.md              # This file
```

## Development Setup

### Prerequisites
- C++ compiler (GCC ≥ 9.0, Clang ≥ 10.0, or MSVC ≥ 2019)
- CMake ≥ 3.16
- Git

### Build Instructions

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### Using Makefile (Alternative)
```bash
make all          # Build everything
make test         # Run tests
make clean        # Clean build artifacts
```

### Running the Application
```bash
./build/amazons   # Or equivalent executable name
```

## Game Rules (Amazons)

### Board
- 10×10 grid
- Each player starts with 4 Amazons in standard positions

### Movement
1. **Amazon Move**: Move one Amazon like a queen in chess (any distance in 8 directions)
2. **Arrow Shot**: From the new position, shoot an arrow like a queen (any distance, 8 directions)
3. **Restrictions**: Cannot move through or land on occupied squares (other Amazons, arrows, or obstacles)

### Game End
- Game ends when a player cannot make a legal move
- Winner is the player with the most territory (reachable squares)

## Key Deadlines

### Botzone Competitions
- **First Test Match**: Dec 20, 2025
- **Second Test Match**: Dec 27, 2025  
- **Third Test Match**: Jan 3, 2026
- **Formal Competition**: Jan 10, 2026

### 验收 (Review) Sessions
- **First验收** (pre-grading): Dec 30, 2025
- **Second验收** (system grading): Jan 9, 2026
- **Third验收** (final grading): Jan 10, 2026

## Development Timeline

- **Week 1**: Project initialization and planning
- **Week 2**: Core game implementation
- **Week 3**: AI development and advanced features
- **Week 4**: Testing, optimization, and competition preparation

## Technology Stack

- **Language**: C++17/20
- **Build System**: CMake
- **Testing Framework**: Google Test (gtest)
- **Version Control**: Git
- **Development Environment**: Visual Studio Code with C++ extensions

## Documentation

- **Memory Bank**: Complete project documentation in `memorybank/`
- **Requirements**: Detailed requirements in `docs/requirements_cn.txt`
- **API Documentation**: To be generated with Doxygen

## Contributing

This is an individual university assignment. The code and documentation serve as a learning example for C++ programming and software engineering principles.

## License

Educational use only - part of university coursework.
