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

# Link libraries
target_include_directories(game_components PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)
