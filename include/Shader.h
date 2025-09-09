#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
private:
    unsigned int ID;
    mutable std::unordered_map<std::string, GLint> uniformLocations;
    
    std::string readFile(const char* filePath) const;
    void checkCompileErrors(unsigned int shader, const std::string& type) const;
    GLint getUniformLocation(const std::string& name) const;

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    
    // Rule of 5 - prevent copying, allow moving
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void use() const;
    unsigned int getID() const { return ID; }

    // Uniform setters with caching
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    
    // Convenience methods
    void setCameraUniforms(const std::string& viewName, const std::string& projectionName, 
                          const class Camera& camera, float aspectRatio) const;
};
