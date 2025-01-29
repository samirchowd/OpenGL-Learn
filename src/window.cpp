#ifndef GLAD_LOADER_INCLUDED
#define GLAD_LOADER_INCLUDED
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  // Initalize the glfw context and set OpenGL version / profile
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ // Required on macOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create a an 800x600 window with title "LearnOpenGL"
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Making the window (an OpenGL context) the main context of the current
  // thread Any thread can only have one OpenGL context active at a time
  glfwMakeContextCurrent(window);

  // This binds a callback to GLAD as to where to load OpenGL functions from
  // GLFW provides getProcAddress as an platform independent way for telling
  // GLAD where to load from
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Tell OpenGL to specify the drawable area
  glViewport(0, 0, 800, 800);

  // Set a call back to update the drawable area in case the user changes the
  // window size
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // While the window is still open, swap the front and back buffer and poll for
  // any callback events
  while (!glfwWindowShouldClose(window)) {
    processInput(window); // Check if the user hit escape

    // Setting Color
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Terminate the context and return
  glfwTerminate();
  return 0;
}
