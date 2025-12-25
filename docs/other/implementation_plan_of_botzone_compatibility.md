# Implementation Plan

Update the graphical interface to use the migrated BotzoneAI algorithm from amazing-amazons, configure a default bot executable in the third_party folder, and maintain graphical mode as the default.

The goal is to integrate the migrated AI algorithm (bot003) into the graphical interface, replacing the current BasicAI implementation. This involves updating the GraphicalController to use BotzoneAI, configuring a default bot executable path, and utilizing the third_party folder for bot storage. The graphical mode should remain the default as it already is.

[Types]  
No new type system changes required, but we need to update AI interface usage.

The existing types are sufficient: GameModeGUI enum for game modes, SelectionState for UI state, and BotzoneAI class for AI implementation. We need to ensure BotzoneAI implements the same interface as BasicAI for drop-in replacement.

[Files]
Update GraphicalController to use BotzoneAI instead of BasicAI, add bot configuration, and utilize third_party folder.

Detailed breakdown:
- **New files to be created**:
  1. `third_party/bots/bot003` - Copy of the compiled bot003 executable from amazing-amazons
  2. `data/config/bot_config.json` - Configuration file for bot paths and settings

- **Existing files to be modified**:
  1. `src/ui/GraphicalController.cpp` - Replace BasicAI with BotzoneAI, add bot path configuration
  2. `src/ui/GraphicalController.hpp` - Update AI member variable type and add configuration methods
  3. `include/ai/BotzoneAI.hpp` - Ensure it has a compatible interface with BasicAI
  4. `src/ai/BotzoneAI.cpp` - Fix any compilation issues (unused parameters, missing includes)
  5. `src/ai/BotProcess.cpp` - Fix compilation errors (kill function, readLine method)
  6. `CMakeLists.txt` - Ensure third_party folder structure is created during build

- **Files to be deleted or moved**: None

- **Configuration file updates**:
  1. Create `data/config/bot_config.json` with default bot path and configuration options
  2. Update build system to copy bot executable to third_party folder

[Functions]
Update GraphicalController functions to use BotzoneAI and add bot configuration management.

Detailed breakdown:
- **New functions**:
  1. `GraphicalController::initializeBot()` - Initialize BotzoneAI with configured path
  2. `GraphicalController::loadBotConfig()` - Load bot configuration from file
  3. `GraphicalController::saveBotConfig()` - Save bot configuration to file

- **Modified functions**:
  1. `GraphicalController::initialize()` - Call initializeBot() instead of creating BasicAI
  2. `GraphicalController::processAIMove()` - Use BotzoneAI::getBestMove() instead of BasicAI
  3. `BotzoneAI::getBestMove()` - Ensure it's compatible with asynchronous UI thread usage
  4. `BotProcess::readLine()` - Fix method declaration and implementation
  5. `BotProcess::Impl::cleanup()` - Fix kill() function usage

- **Removed functions**: None

[Classes]
Update GraphicalController class to use BotzoneAI and manage bot configuration.

Detailed breakdown:
- **New classes**: None
- **Modified classes**:
  1. `GraphicalController` - Change AI member from `std::unique_ptr<BasicAI>` to `std::unique_ptr<BotzoneAI>`, add bot configuration methods
  2. `BotzoneAI` - Ensure it can be used as drop-in replacement for BasicAI in GUI context
  3. `BotProcess` - Fix compilation errors and ensure proper process management

- **Removed classes**: None

[Dependencies]
Add JSON library for configuration file parsing if not already available.

The project may need a simple JSON parser for configuration files. Since this is a C++ project, we can use nlohmann/json if available, or implement a simple configuration parser. For simplicity, we can use a basic key-value format or use existing serialization infrastructure.

[Testing]
Test the graphical interface with the new BotzoneAI integration.

Test file requirements:
1. Update existing GUI tests to work with BotzoneAI
2. Create integration test for bot configuration loading
3. Test bot executable copying to third_party folder during build
4. Test Human vs AI gameplay in graphical mode

Existing test modifications:
1. Update `test_botzone.cpp` to use the new third_party bot path
2. Ensure unit tests for BotzoneAI and BotProcess still pass

Validation strategies:
1. Manual testing of graphical interface with Human vs AI mode
2. Verify bot moves are generated correctly
3. Test bot configuration changes through UI (future enhancement)

[Implementation Order]
Sequential implementation to ensure each step builds on the previous.

Numbered steps:
1. Fix BotzoneAI and BotProcess compilation errors (unused parameters, missing includes, kill function)
2. Copy bot003 executable to third_party/bots/ folder
3. Create bot configuration system (data/config/bot_config.json)
4. Update GraphicalController to use BotzoneAI instead of BasicAI
5. Add bot configuration loading/saving to GraphicalController
6. Update CMakeLists.txt to handle third_party folder structure
7. Test the integration with graphical interface
8. Verify default mode is still graphical and works correctly