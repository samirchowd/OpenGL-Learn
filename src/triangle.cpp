#ifndef GLAD_LOADER_INCLUDED
#define GLAD_LOADER_INCLUDED
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSourceOrange =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShaderSourceYellow =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);\n"
    "}\n\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  // ==========================================================================
  //                            Window Setup
  // ==========================================================================
  // Window Setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ // Required on macOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 800);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  // ==========================================================================
  //                            Shader Program
  // ==========================================================================
  // Vertex Shader
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Orange Fragment Shader
  unsigned int fragmentShaderOrange;
  fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);
  glCompileShader(fragmentShaderOrange);

  // Yellow Fragment Shader
  unsigned int fragmentShaderYellow;
  fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
  glCompileShader(fragmentShaderYellow);

  // Create orange shader program
  unsigned int shaderProgramOrange;
  shaderProgramOrange = glCreateProgram();

  // Attached compiled shaders to the program and link them
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShaderOrange);
  glLinkProgram(shaderProgramOrange);

  // Create yellow shader program
  unsigned int shaderProgramYellow;
  shaderProgramYellow = glCreateProgram();

  glAttachShader(shaderProgramYellow, vertexShader);
  glAttachShader(shaderProgramYellow, fragmentShaderYellow);
  glLinkProgram(shaderProgramYellow);

  // Delete shader programs after they've been linked
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOrange);
  glDeleteShader(fragmentShaderYellow);

  // ==========================================================================
  //                            Vertex
  // ==========================================================================
  // Vertex Data
  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  // Generate vertex buffer and VAO (configures how shader reads buffer)
  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind VAO first
  glBindVertexArray(VAO);

  // Bind buffer and then send vertex data to the buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind buffer and send element data to the buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Tell OpenGL how to read buffer data, VAO stores this information
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(VAO);

  // ==========================================================================
  //                            Render
  // ==========================================================================
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramOrange);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

    GLvoid *offset = (GLvoid *)(3 * sizeof(unsigned int));

    glUseProgram(shaderProgramYellow);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, offset);

    // Handle events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // Terminate the context and return
  glfwTerminate();
  return 0;
}
