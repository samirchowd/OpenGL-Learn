---
name: Project Overview
description: High-level summary of the OpenGL-Learn C++ project — purpose, tech stack, current state, and in-progress work
type: project
---

OpenGL 3.3 Core Profile learning project in C++20, based on LearnOpenGL.com. Renders the Sponza atrium model with a full Phong lighting pipeline.

**Tech stack:** GLFW3, GLM, Assimp, GLAD (bundled), stb_image (bundled), Dear ImGui v1.91.9 (fetched via CMake FetchContent). Built with CMake 3.15+.

**Architecture:**
- `Camera` — FPS-style camera with keyboard/mouse/scroll input
- `Shader` — GLSL loader/linker with uniform location caching
- `Texture` — stb_image loader, RAII, move-only
- `Mesh` — VAO/VBO/EBO wrapper holding vertices (pos + normal + texcoord), indices, and shared_ptr textures
- `Model` — Assimp scene importer; processes node tree into Mesh list with texture cache (diffuse + specular maps)
- `Light` — free functions to upload `PointLight`, `DirLight`, `SpotLight` structs to shader uniforms
- `Skybox` — header exists (`include/Skybox.h`) but is not yet implemented
- `src/cube_scene.cpp` — older cube scene file, no longer used in main build

**Shader pair in use:** `shaders/model.lit.vertex.glsl` + `shaders/model.lit.fragment.glsl`
- Phong lighting: 1 DirLight + 4 PointLights + 1 SpotLight (camera-attached, toggle with F key)
- **Current state:** FragColor is rendering depth visualization (linearized depth), not full Phong color — the `vec4(result, 1.0)` line is commented out

**Controls:**
- WASD: camera movement
- Mouse: look (FPS)
- Scroll: FOV zoom
- F: toggle flashlight (SpotLight)
- TAB: toggle cursor capture (for ImGui interaction)
- ESC: quit

**In-progress / pending work:**
- `include/Skybox.h` exists (stub only) — skybox textures are in `textures/skybox/` — not yet implemented
- Fragment shader is showing depth buffer instead of Phong result (debug leftover)
- `shaders/model.fragment.glsl` and `model.vertex.glsl` (no suffix) also exist as older variants

**Why:** Learning project following LearnOpenGL.com progression — basic rendering → camera → textures → lighting → model loading → (next: skybox, more advanced effects).
