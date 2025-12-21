# Source directories
add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(utils)
add_subdirectory(features)
add_subdirectory(ai)

# Create library for game components
add_library(game_components
  core/Position.cpp
  core/Board.cpp
  core/GameState.cpp
  core/Player.cpp
  core/Move.cpp
  core/GameController.cpp
  ui/TextDisplay.cpp
  ui/InputHandler.cpp
  ui/MenuController.cpp
  utils/Serializer.cpp
  ai/BasicAI.cpp
)

# Add GraphicalDisplay if graphical GUI is enabled
if(WITH_GRAPHICAL_GUI)
  target_sources(game_components PRIVATE ui/GraphicalDisplay.cpp ui/GraphicalController.cpp)
endif()

# Link libraries
target_include_directories(game_components PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

# Add SFML includes if graphical GUI is enabled
if(WITH_GRAPHICAL_GUI)
  target_include_directories(game_components PRIVATE /opt/homebrew/Cellar/sfml/3.0.2/include)
endif()
