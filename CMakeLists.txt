cmake_minimum_required(VERSION 3.15)
project(OpenGL-Learn VERSION 0.1.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Configure Assimp build options (before add_subdirectory)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)              # Build static library
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)            # Don't build tests
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)     # Don't build tools
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)                # Don't install (submodule)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)     # Avoid build errors from warnings

# Add Assimp subdirectory
add_subdirectory(external/assimp)

# Source files
set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/VertexBuffer.cpp
    src/ElementBuffer.cpp
    external/glad/src/glad.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    glm::glm
    assimp
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy resources to build directory
set(RESOURCE_DIRS shaders textures)
foreach(dir ${RESOURCE_DIRS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${dir}
            ${CMAKE_BINARY_DIR}/${dir}
        COMMENT "Copying ${dir} to build directory"
    )
endforeach()

# Copy materials.json
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/materials.json
        ${CMAKE_BINARY_DIR}/materials.json
    COMMENT "Copying materials.json to build directory"
)

# Print build info
message(STATUS "OpenGL-Learn ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
