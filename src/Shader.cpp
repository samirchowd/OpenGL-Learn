#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readShaderFile(const char *filePath) {
  std::string shaderCode;
  std::ifstream shaderFile;

  // Ensure ifstream objects can throw exceptions:
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // Open files
    shaderFile.open(filePath);
    std::stringstream shaderStream;

    // Read file's buffer contents into stream
    shaderStream << shaderFile.rdbuf();

    // Close file handlers
    shaderFile.close();

    // Convert stream into string
    shaderCode = shaderStream.str();
  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
  }
  return shaderCode;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexSource = readShaderFile(vertexPath);
  std::string fragmentSource = readShaderFile(fragmentPath);

  std::cout << vertexSource << std::endl;

  const char *vShaderCode = vertexSource.c_str();
  const char *fShaderCode = fragmentSource.c_str();

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertex);
  glAttachShader(this->ID, fragment);
  glLinkProgram(this->ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(this->ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}
