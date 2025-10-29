# OpenGL-Learn

A modern C++20 OpenGL learning project demonstrating 3D graphics rendering techniques with Phong lighting, texture mapping, and interactive camera controls.

## Demo

![Demo Video](./assets/learn-open-gl.mov)

## Features

- **Modern OpenGL 3.3 Core Profile** rendering pipeline
- **Phong Lighting Model** with ambient, diffuse, and specular components
- **Multiple Light Sources**: 4 point lights with attenuation + 1 directional light
- **Texture Mapping**: Diffuse and specular maps for realistic materials
- **Interactive FPS Camera** with mouse look and smooth movement
- **Material System**: JSON-based material definitions (metals, plastics, gems, rubber)
- **Clean OOP Architecture**: Shader, Texture, Camera, and Buffer abstraction classes

## Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 10+, or MSVC 2019+)
- CMake 3.15+
- OpenGL 3.3+ capable GPU
- Dependencies:
  - GLFW3 (window management)
  - GLM (mathematics)
  - GLAD (OpenGL function loader, included in `external/`)
  - stb_image (texture loading, included in `external/`)

## Installing Dependencies

**macOS (Homebrew):**
```sh
brew install glfw glm cmake
```

**Linux (Ubuntu/Debian):**
```sh
sudo apt install libglfw3-dev libglm-dev cmake
```

**Linux (Fedora):**
```sh
sudo dnf install glfw-devel glm-devel cmake
```

**Windows:**
- Install [CMake](https://cmake.org/download/)
- Use [vcpkg](https://vcpkg.io/):
  ```sh
  vcpkg install glfw3 glm
  ```

## Building

```sh
cmake -S . -B build
cmake --build build
```

Or using the classic CMake workflow:

```sh
mkdir build && cd build
cmake ..
make
```

The executable will be in `build/` along with copied shaders, textures, and materials.

## Running

```sh
./build/OpenGL-Learn
```

## Controls

| Input | Action |
|-------|--------|
| **W/A/S/D** | Move camera forward/left/backward/right |
| **Mouse** | Look around (FPS-style) |
| **Scroll Wheel** | Zoom in/out (FOV adjustment) |
| **ESC** | Close application |

## Project Structure

```
include/          - Header files (Camera, Shader, Texture, buffers)
src/              - Implementation files
  main.cpp        - Main application and render loop
external/         - Third-party dependencies
  glad/           - OpenGL function loader
  stb/            - stb_image header-only library
shaders/          - GLSL vertex and fragment shaders
textures/         - Texture assets (diffuse and specular maps)
materials.json    - Material property definitions
CMakeLists.txt    - Build configuration
```

## What This Demonstrates

This project showcases progressive OpenGL learning topics:

1. **Core Rendering**: Vertex buffers, shaders, transformations
2. **Camera System**: View/projection matrices, user input handling
3. **Texture Mapping**: Loading and sampling 2D textures
4. **Lighting**: Phong model with multiple light types
5. **Materials**: Physically-based material properties
6. **Modern C++**: RAII, move semantics, smart resource management

## Learning Resources

Based on [LearnOpenGL.com](https://learnopengl.com/) tutorials, covering topics from basic rendering through advanced lighting techniques.
