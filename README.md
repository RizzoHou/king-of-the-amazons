# King of the Amazons

A C++ implementation of the "Game of the Amazons" (also known as "Amazons" or "Queen's Game"), a two-player abstract strategy board game. This project serves as a university assignment for a computational introduction course.

## Project Status

**Current Phase**: Implementation Planning Complete  
**Overall Completion**: 10% (Documentation and implementation plan complete, ready for development)

### What Works
- Comprehensive memory bank documentation
- Detailed implementation plan in `docs/implementation/`
- Project architecture planning (MVC pattern)
- Technical requirements analysis
- Development workflow establishment (including git commit message best practices)
- Documentation standardization (English terminology)

### What's Next
- Create project structure following `01_project_structure.md`
- Implement core game components as specified in `03_technical_specifications.md`
- Develop basic text-based user interface following UI specifications
- Implement game logic and move validation with comprehensive testing
- Set up testing framework as outlined in `04_testing_strategy.md`

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

Complete file hierarchy and organization guidelines are documented in `docs/implementation/01_project_structure.md`. The structure follows software engineering best practices with clear separation of concerns:

```
king-of-the-amazons/
├── CMakeLists.txt                 # Main CMake configuration
├── Makefile                       # Alternative build system
├── README.md                      # Project overview and setup instructions
├── .gitignore                     # Git ignore rules
├── .clinerules/                   # Cline operation rules
├── docs/                          # Documentation
│   ├── implementation/            # Implementation plan (6 detailed documents)
│   ├── instructions/              # Course requirements
│   ├── api/                       # API documentation (generated)
│   └── reports/                   # Project reports
├── memorybank/                    # Project memory bank (6 core files)
├── src/                           # Source code (core/, ai/, ui/, utils/, botzone/)
├── include/                       # Public headers
├── tests/                         # Test suite (unit/, integration/, system/)
├── data/                          # Game data (saves/, config/, logs/)
├── scripts/                       # Build and utility scripts
└── third_party/                   # External dependencies
```

For detailed directory descriptions and development guidelines, refer to the implementation plan documents.

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

### Review Sessions
- **First Review Session** (pre-grading): Dec 30, 2025
- **Second Review Session** (system grading): Jan 9, 2026
- **Third Review Session** (final grading): Jan 10, 2026

## Development Timeline

Detailed timeline and milestones are documented in `docs/implementation/02_implementation_phases.md`:

### Phase 1: Core Infrastructure (Week 1: Dec 17-23, 2025)
- Project setup and CMake configuration
- Basic game components implementation
- Text interface foundation

### Phase 2: Game Flow (Week 2: Dec 24-30, 2025)
- Complete game loop implementation
- Save/load system development
- Enhanced user interface
- **Critical Deadline**: First Review Session (Dec 30, 2025)

### Phase 3: AI Development (Week 3: Dec 31, 2025 - Jan 6, 2026)
- AI framework and move generation
- Minimax algorithm implementation
- Botzone integration
- **Critical Deadline**: Third Botzone test match (Jan 3, 2026)

### Phase 4: Polish and Integration (Week 4: Jan 7-10, 2026)
- Comprehensive testing and quality assurance
- Documentation completion
- Competition preparation
- **Critical Deadlines**: Second Review Session (Jan 9, 2026), Final Review Session and Botzone formal competition (Jan 10, 2026)

## Technology Stack

- **Language**: C++17/20
- **Build System**: CMake
- **Testing Framework**: Google Test (gtest)
- **Version Control**: Git
- **Development Environment**: Visual Studio Code with C++ extensions

## Documentation

### Implementation Plan
Comprehensive planning documents in `docs/implementation/`:
- `01_project_structure.md`: File hierarchy and organization
- `02_implementation_phases.md`: Timeline and milestones
- `03_technical_specifications.md`: Technical details and APIs
- `04_testing_strategy.md`: Testing approach and quality assurance
- `05_risk_management.md`: Risk assessment and mitigation
- `06_success_criteria.md`: Success metrics and grading requirements

### Project Documentation
- **Memory Bank**: Complete project documentation in `memorybank/` (6 core files)
- **Requirements**: Detailed requirements available in both Chinese (`docs/instructions/requirements_cn.txt`) and English (`docs/instructions/requirements_en.txt`)
- **API Documentation**: To be generated with Doxygen during implementation

### Success Criteria
Detailed success metrics and grading requirements documented in `docs/implementation/06_success_criteria.md`

## Contributing

This is an individual university assignment. The code and documentation serve as a learning example for C++ programming and software engineering principles.

## License

Educational use only - part of university coursework.
