#include "Skybox.h"
#include "Shader.h"
#include "stb_image.h"
#include <utility>
#include <vector>

Skybox::Skybox(const std::string &directory, const std::string &extension) : shader("shaders/skybox.vert.glsl", "shaders/skybox.frag.glsl") {
    cubemapTexture = loadCubemap(directory, extension);
    setupMesh();
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &cubemapTexture);
}

Skybox::Skybox(Skybox &&other) noexcept
    : VAO(other.VAO), VBO(other.VBO), cubemapTexture(other.cubemapTexture),
      shader(std::move(other.shader)) {
    other.VAO = 0;
    other.VBO = 0;
    other.cubemapTexture = 0;
}

Skybox &Skybox::operator=(Skybox &&other) noexcept {
    if (this != &other) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteTextures(1, &cubemapTexture);
        VAO = other.VAO;
        VBO = other.VBO;
        cubemapTexture = other.cubemapTexture;
        shader = std::move(other.shader);
        other.VAO = 0;
        other.VBO = 0;
        other.cubemapTexture = 0;
    }
    return *this;
}

void Skybox::Draw(const glm::mat4 &view, const glm::mat4 &projection) {
    // Render skybox last optimization
    glDepthFunc(GL_LEQUAL);

    shader.use();
    glm::mat4 rotView = glm::mat4(glm::mat3(view));

    shader.setMat4("view", rotView);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void Skybox::setupMesh() {
    // clang-format off
    static constexpr float vertices[] = {
        // back face
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        // left face
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        // right face
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        // front face
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        // top face
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        // bottom face
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
    };
    // clang-format on

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindVertexArray(0);
}

unsigned int Skybox::loadCubemap(const std::string &directory, const std::string &extension) {

    // Generate cube map texture handle
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Generate texture face paths
    std::vector<std::string> textures_faces;
    for (const std::string face : Skybox::FACES) {
        textures_faces.push_back(directory + face + "." + extension);
    }

    // Load in textures via stbi
    stbi_set_flip_vertically_on_load(false);
    int width, height, nrChannels;
    unsigned char *data;
    for (unsigned int i = 0; i < textures_faces.size(); i++) {
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);
    return textureID;
}
