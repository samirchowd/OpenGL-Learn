#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <format>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float deltaTime);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool spotlight = false;

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader shader("shaders/model.lit.vertex.glsl", "shaders/model.lit.fragment.glsl");
  Shader lightShader("shaders/vertex.glsl", "shaders/light.fragment.glsl");
  Model model("models/backpack/backpack.obj");

  float cubeVertices[] = {
    -0.5f,-0.5f,-0.5f, 0.5f,-0.5f,-0.5f, 0.5f, 0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,-0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,-0.5f, 0.5f,-0.5f,-0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,-0.5f,-0.5f, 0.5f,-0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f, 0.5f, 0.5f,-0.5f, 0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f, 0.5f,-0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f, 0.5f,-0.5f,-0.5f, 0.5f,-0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,-0.5f,-0.5f, 0.5f,-0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,-0.5f, 0.5f, 0.5f,-0.5f, 0.5f,-0.5f,
  };

  unsigned int lightVAO, lightVBO;
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &lightVBO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  shader.use();

  // material
  shader.setFloat("shininess", 32.0f);

  // directional light
  shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  shader.setVec3("dirLight.ambient",    0.05f, 0.05f, 0.05f);
  shader.setVec3("dirLight.diffuse",    0.4f,  0.4f,  0.4f);
  shader.setVec3("dirLight.specular",   0.5f,  0.5f,  0.5f);

  // point lights
  for (int i = 0; i < 4; i++) {
    shader.setVec3(std::format("pointLights[{}].position",  i), pointLightPositions[i]);
    shader.setVec3(std::format("pointLights[{}].ambient",   i), 0.05f, 0.05f, 0.05f);
    shader.setVec3(std::format("pointLights[{}].diffuse",   i), 0.8f,  0.8f,  0.8f);
    shader.setVec3(std::format("pointLights[{}].specular",  i), 1.0f,  1.0f,  1.0f);
    shader.setFloat(std::format("pointLights[{}].constant",  i), 1.0f);
    shader.setFloat(std::format("pointLights[{}].linear",    i), 0.09f);
    shader.setFloat(std::format("pointLights[{}].quadratic", i), 0.032f);
  }

  // spotlight
  shader.setFloat("spotLight.phi",      glm::cos(glm::radians(12.5f)));
  shader.setFloat("spotLight.phiOuter", glm::cos(glm::radians(25.0f)));
  shader.setVec3("spotLight.ambient",   0.0f, 0.0f, 0.0f);
  shader.setVec3("spotLight.diffuse",   1.0f, 1.0f, 1.0f);
  shader.setVec3("spotLight.specular",  1.0f, 1.0f, 1.0f);
  shader.setBool("spotLight.enabled",   spotlight);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, deltaTime);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    shader.setVec3("viewPos", camera.Position);

    // update spotlight direction and toggle
    shader.setVec3("spotLight.spotDir", camera.Front);
    shader.setBool("spotLight.enabled", spotlight);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                            0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setMat4("model", modelMat);

    model.Draw(shader);

    // render light cubes
    lightShader.use();
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    glBindVertexArray(lightVAO);
    for (unsigned int i = 0; i < 4; i++) {
      glm::mat4 lightMat = glm::mat4(1.0f);
      lightMat = glm::translate(lightMat, pointLightPositions[i]);
      lightMat = glm::scale(lightMat, glm::vec3(0.2f));
      lightShader.setMat4("model", lightMat);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);

  static bool fKeyPressed = false;
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed) {
    fKeyPressed = true;
    spotlight = !spotlight;
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    fKeyPressed = false;
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
