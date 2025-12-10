cmake_minimum_required(VERSION 3.10)
project(SuffixTreeAnimation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(SuffixTreeAnimation 
    SuffixTreeGUI.cpp 
    SuffixTree.cpp
)

# Link SFML libraries
target_link_libraries(SuffixTreeAnimation sfml-graphics sfml-window sfml-system)

# Set output directory
set_target_properties(SuffixTreeAnimation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Installation
install(TARGETS SuffixTreeAnimation DESTINATION bin)

# Print build information
message(STATUS "Building Suffix Tree Animation")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
