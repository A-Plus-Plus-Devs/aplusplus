cmake_minimum_required(VERSION 3.10)
project(YourLang)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Add source files
file(GLOB SOURCES "src/**/*.c")

# Add executable
add_executable(yourlang ${SOURCES})

# Include directories
include_directories(include)

# Enable testing
enable_testing()
add_subdirectory(tests)
