# Test configuration
enable_testing()

# Unit tests
add_executable(unit_tests
  unit/PositionTest.cpp
  unit/BoardTest.cpp
  unit/GameStateTest.cpp
  unit/MoveTest.cpp
  unit/PlayerTest.cpp
  unit/TextDisplayTest.cpp
)

# Link test executable with Google Test and game components
target_link_libraries(unit_tests
  GTest::gtest_main
  game_components
)

# Add test
add_test(NAME unit_tests COMMAND unit_tests)

# Integration tests (to be added later)
# add_executable(integration_tests ...)
# target_link_libraries(integration_tests ...)
# add_test(NAME integration_tests COMMAND integration_tests)
