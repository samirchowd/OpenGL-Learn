#include "Camera.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

// Simple JSON parser for materials
std::map<std::string, Material> loadMaterials(const std::string &filename);
void setMaterial(Shader &shader, const Material &material);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Shader &shader, Camera &camera,
                  float deltaTime);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Global blend factor
float blendFactor = 0.2f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Materials
std::map<std::string, Material> materials;
std::vector<std::string> materialNames;
int currentMaterialIndex = 0;

// Light animation toggle
bool animateLight = false;

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  // Load materials from JSON
  materials = loadMaterials("materials.json");
  for (const auto &pair : materials) {
    materialNames.push_back(pair.first);
  }
  if (materialNames.empty()) {
    std::cout << "No materials loaded, using default gold material" << std::endl;
  } else {
    std::cout << "Loaded " << materialNames.size() << " materials" << std::endl;
  }

  float vertices[] = {
      // positions          // normals           // texture coords
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

  Shader objectShader("shaders/vertex.glsl", "shaders/object.fragment.glsl");
  Shader lightShader("shaders/vertex.glsl", "shaders/light.fragment.glsl");

  // Create vertex array object and buffers
  unsigned int lightVao, objectVao;
  glGenVertexArrays(1, &lightVao);
  glGenVertexArrays(1, &objectVao);

  glBindVertexArray(lightVao);
  VertexBuffer vbo(vertices, sizeof(vertices));

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(objectVao);
  vbo.bind();

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texcoord attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  objectShader.use();
  objectShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  // Set initial material
  if (!materialNames.empty()) {
    setMaterial(objectShader, materials[materialNames[currentMaterialIndex]]);
    std::cout << "Starting with material: " << materialNames[currentMaterialIndex] << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  M/N - Cycle through materials" << std::endl;
    std::cout << "  L   - Toggle animated light colors" << std::endl;

    // Print current material properties for debugging
    auto &mat = materials[materialNames[currentMaterialIndex]];
    std::cout << "Ambient: " << mat.ambient.x << ", " << mat.ambient.y << ", " << mat.ambient.z << std::endl;
    std::cout << "Diffuse: " << mat.diffuse.x << ", " << mat.diffuse.y << ", " << mat.diffuse.z << std::endl;
  } else {
    std::cout << "No materials loaded, using fallback gold" << std::endl;
    Material gold = {{0.24725f, 0.1995f, 0.0745f}, {0.75164f, 0.60648f, 0.22648f}, {0.628281f, 0.555802f, 0.366065f}, 0.4f};
    setMaterial(objectShader, gold);
  }

  // Set all light intensities to full white for accurate material representation
  objectShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
  objectShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
  objectShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  glm::vec3 lightStart(1.2f, 1.0f, 2.0f);

  GLuint objectModelLoc = glGetUniformLocation(objectShader.getID(), "model");
  GLuint objectViewLoc = glGetUniformLocation(objectShader.getID(), "view");
  GLuint objectProjLoc = glGetUniformLocation(objectShader.getID(), "projection");

  GLuint lightModelLoc = glGetUniformLocation(lightShader.getID(), "model");
  GLuint lightViewLoc = glGetUniformLocation(lightShader.getID(), "view");
  GLuint lightProjLoc = glGetUniformLocation(lightShader.getID(), "projection");

  // Set light Position

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, objectShader, camera, deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = (float)glfwGetTime();
    glm::vec3 lightPos;
    if (animateLight) {
      // Spinning light position
      float radius = sqrt(lightStart.x * lightStart.x + lightStart.z * lightStart.z);
      lightPos = glm::vec3(radius * cos(time), lightStart.y, radius * sin(time));
    } else {
      // Static light position
      lightPos = lightStart;
    }

    // 1. Render the object (orange cube)
    objectShader.use();
    glBindVertexArray(objectVao);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(objectModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(objectViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(objectProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    objectShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    objectShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 2. Render the light source (white cube at lightPos)
    lightShader.use();
    glBindVertexArray(lightVao);

    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it smaller

    glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(lightProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window, Shader &shader, Camera &camera,
                  float deltaTime) {
  static bool mKeyPressed = false;
  static bool nKeyPressed = false;
  static bool lKeyPressed = false;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // Camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);

  // Material cycling with M/N keys
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !mKeyPressed && !materialNames.empty()) {
    mKeyPressed = true;
    currentMaterialIndex = (currentMaterialIndex + 1) % materialNames.size();
    setMaterial(shader, materials[materialNames[currentMaterialIndex]]);
    std::cout << "Material: " << materialNames[currentMaterialIndex] << std::endl;
  }
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
    mKeyPressed = false;
  }

  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !nKeyPressed && !materialNames.empty()) {
    nKeyPressed = true;
    currentMaterialIndex = (currentMaterialIndex - 1 + materialNames.size()) % materialNames.size();
    setMaterial(shader, materials[materialNames[currentMaterialIndex]]);
    std::cout << "Material: " << materialNames[currentMaterialIndex] << std::endl;
  }
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) {
    nKeyPressed = false;
  }

  // Light animation toggle with L key
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !lKeyPressed) {
    lKeyPressed = true;
    animateLight = !animateLight;
    std::cout << "Light animation: " << (animateLight ? "ON" : "OFF") << std::endl;
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
    lKeyPressed = false;
  }

  // UP key - increase blend factor
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    blendFactor = std::min(1.0f, blendFactor + 0.005f);
    shader.setFloat("blendFactor", blendFactor);
  }

  // DOWN key - decrease blend factor
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    blendFactor = std::max(0.0f, blendFactor - 0.005f);
    shader.setFloat("blendFactor", blendFactor);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}
// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// Simple JSON parser for materials
std::map<std::string, Material> loadMaterials(const std::string &filename) {
  std::map<std::string, Material> loadedMaterials;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open materials file: " << filename << std::endl;
    return loadedMaterials;
  }

  std::string line;
  std::string currentMaterial;
  Material material;

  while (std::getline(file, line)) {
    // Simple parsing - look for material names (strings ending with ": {")
    if (line.find("\": {") != std::string::npos) {
      size_t start = line.find("\"") + 1;
      size_t end = line.find("\"", start);
      currentMaterial = line.substr(start, end - start);
    }
    // Parse ambient values
    else if (line.find("\"ambient\":") != std::string::npos) {
      size_t start = line.find("[") + 1;
      size_t end = line.find("]");
      std::string values = line.substr(start, end - start);
      std::stringstream ss(values);
      std::string value;
      int i = 0;
      while (std::getline(ss, value, ',') && i < 3) {
        material.ambient[i] = std::stof(value);
        i++;
      }
    }
    // Parse diffuse values
    else if (line.find("\"diffuse\":") != std::string::npos) {
      size_t start = line.find("[") + 1;
      size_t end = line.find("]");
      std::string values = line.substr(start, end - start);
      std::stringstream ss(values);
      std::string value;
      int i = 0;
      while (std::getline(ss, value, ',') && i < 3) {
        material.diffuse[i] = std::stof(value);
        i++;
      }
    }
    // Parse specular values
    else if (line.find("\"specular\":") != std::string::npos) {
      size_t start = line.find("[") + 1;
      size_t end = line.find("]");
      std::string values = line.substr(start, end - start);
      std::stringstream ss(values);
      std::string value;
      int i = 0;
      while (std::getline(ss, value, ',') && i < 3) {
        material.specular[i] = std::stof(value);
        i++;
      }
    }
    // Parse shininess
    else if (line.find("\"shininess\":") != std::string::npos) {
      size_t start = line.find(":") + 1;
      std::string value = line.substr(start);
      // Remove whitespace and trailing characters
      value.erase(0, value.find_first_not_of(" \t"));
      value.erase(value.find_last_not_of(" \t\n\r,") + 1);
      material.shininess = std::stof(value);

      // Material is complete, add it to the map
      if (!currentMaterial.empty()) {
        loadedMaterials[currentMaterial] = material;
      }
    }
  }

  return loadedMaterials;
}

void setMaterial(Shader &shader, const Material &material) {
  shader.use();
  shader.setVec3("material.ambient", material.ambient);
  shader.setVec3("material.diffuse", material.diffuse);
  shader.setVec3("material.specular", material.specular);
  shader.setFloat("material.shininess", material.shininess * 128.0f);
  std::cout << "Ambient: " << material.ambient.x << ", " << material.ambient.y << ", " << material.ambient.z << std::endl;
  std::cout << "Diffuse: " << material.diffuse.x << ", " << material.diffuse.y << ", " << material.diffuse.z << std::endl;
  std::cout << "Specular: " << material.specular.x << ", " << material.specular.y << ", " << material.specular.z << std::endl;
  std::cout << "Shininess: " << material.shininess << ", " << material.shininess * 128.0f << std::endl;
}
