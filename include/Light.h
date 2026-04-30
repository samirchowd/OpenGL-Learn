#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float phi;
    float phiOuter;
    bool enabled;
};

void setPointLight(Shader &shader, int index, const PointLight &light);
void setDirLight(Shader &shader, const DirLight &light);
void setSpotLight(Shader &shader, const SpotLight &light);
