#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

#include "Shader.h"

class Skybox {
  public:
    static constexpr std::array<const char *, 6> FACES = {
        "right", "left", "top", "bottom", "front", "back"};

    Skybox(const std::string &directory, const std::string &extension = "jpg");
    ~Skybox();

    Skybox(const Skybox &) = delete;
    Skybox &operator=(const Skybox &) = delete;
    Skybox(Skybox &&other) noexcept;
    Skybox &operator=(Skybox &&other) noexcept;

    void Draw(const glm::mat4 &view, const glm::mat4 &projection);
    unsigned int getCubemapTexture() const { return cubemapTexture; }

  private:
    unsigned int VAO, VBO;
    unsigned int cubemapTexture;
    Shader shader;

    unsigned int loadCubemap(const std::string &directory, const std::string &extension);
    void setupMesh();
};
