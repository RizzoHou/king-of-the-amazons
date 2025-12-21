cmake_minimum_required(VERSION 3.16)
project(KingOfTheAmazons VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
  else()
    add_compile_options(-O2)
  endif()
endif()

# Option for graphical interface
option(WITH_GRAPHICAL_GUI "Build with SFML-based graphical GUI" ON)

# Find SFML if graphical GUI is enabled
if(WITH_GRAPHICAL_GUI)
  find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
  message(STATUS "SFML found: ${SFML_VERSION}")
  add_definitions(-DWITH_GRAPHICAL_GUI)
endif()

# Google Test setup
enable_testing()

# Find or download Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Create executable
add_executable(amazons src/main.cpp)

# Link game_components library
target_link_libraries(amazons game_components)

# Link SFML if graphical GUI is enabled
if(WITH_GRAPHICAL_GUI)
  target_link_libraries(amazons sfml-graphics sfml-window sfml-system)
  target_include_directories(amazons PRIVATE /opt/homebrew/Cellar/sfml/3.0.2/include)
  target_link_directories(amazons PRIVATE /opt/homebrew/Cellar/sfml/3.0.2/lib)
  message(STATUS "Linking with SFML libraries")
endif()

# Target properties
set_target_properties(amazons PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# Installation
install(TARGETS amazons DESTINATION bin)
install(DIRECTORY data/ DESTINATION data)
