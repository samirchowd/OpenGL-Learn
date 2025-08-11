#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) : ID(0) {
  std::string vertexSource = readFile(vertexPath);
  std::string fragmentSource = readFile(fragmentPath);

  const char *vShaderCode = vertexSource.c_str();
  const char *fShaderCode = fragmentSource.c_str();

  // Compile vertex shader
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  // Compile fragment shader
  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  // Link program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  // Clean up shaders
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  if (ID != 0) {
    glDeleteProgram(ID);
  }
}

Shader::Shader(Shader &&other) noexcept
    : ID(other.ID), uniformLocations(std::move(other.uniformLocations)) {
  other.ID = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    if (ID != 0) {
      glDeleteProgram(ID);
    }
    ID = other.ID;
    uniformLocations = std::move(other.uniformLocations);
    other.ID = 0;
  }
  return *this;
}

void Shader::use() const { glUseProgram(ID); }

std::string Shader::readFile(const char *filePath) const {
  std::ifstream shaderFile;
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    shaderFile.open(filePath);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_READ: " << filePath << " - "
              << e.what() << std::endl;
    return "";
  }
}

void Shader::checkCompileErrors(unsigned int shader,
                                const std::string &type) const {
  int success;
  char infoLog[1024];

  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << std::endl;
    }
  }
}

GLint Shader::getUniformLocation(const std::string &name) const {
  auto it = uniformLocations.find(name);
  if (it != uniformLocations.end()) {
    return it->second;
  }

  GLint location = glGetUniformLocation(ID, name.c_str());
  uniformLocations[name] = location;
  return location;
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVec4(const std::string &name, float x, float y, float z,
                     float w) const {
  glUniform4f(getUniformLocation(name), x, y, z, w);
}
