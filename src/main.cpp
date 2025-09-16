#include "Camera.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

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

  // Geometry
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

  Shader objectShader("shaders/vertex.glsl", "shaders/object.fragment.glsl");
  Shader lightShader("shaders/vertex.glsl", "shaders/light.fragment.glsl");

  // Create vertex array object and buffers
  unsigned int lightVao, objectVao;
  glGenVertexArrays(1, &lightVao);
  glGenVertexArrays(1, &objectVao);

  glBindVertexArray(lightVao);
  VertexBuffer vbo(vertices, sizeof(vertices));

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(objectVao);
  vbo.bind();

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  objectShader.use();
  objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  objectShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

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
    float radius = sqrt(lightStart.x * lightStart.x + lightStart.z * lightStart.z);
    glm::vec3 lightPos = glm::vec3(radius * cos(time), lightStart.y, radius * sin(time));

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
