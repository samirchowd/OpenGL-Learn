# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(OpenGL-Learn)

# Find the GLFW package
find_package(glfw3 REQUIRED)

# Include directories
# Add the system-wide GLAD include directory
include_directories(/usr/local/include)

# Add source files
set(SOURCES
    src/window.cpp  # Your application source file
    /usr/local/src/glad.c  # GLAD source file in the system-wide location
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
# Link GLFW and OpenGL to your project
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

