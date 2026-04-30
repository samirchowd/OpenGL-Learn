#include "Light.h"
#include <format>

void setPointLight(Shader &shader, int index, const PointLight &light) {
    shader.setVec3(std::format("pointLights[{}].position", index), light.position);
    shader.setFloat(std::format("pointLights[{}].constant", index), light.constant);
    shader.setFloat(std::format("pointLights[{}].linear", index), light.linear);
    shader.setFloat(std::format("pointLights[{}].quadratic", index), light.quadratic);
    shader.setVec3(std::format("pointLights[{}].ambient", index), light.ambient);
    shader.setVec3(std::format("pointLights[{}].diffuse", index), light.diffuse);
    shader.setVec3(std::format("pointLights[{}].specular", index), light.specular);
}

void setDirLight(Shader &shader, const DirLight &light) {
    shader.setVec3("dirLight.direction", light.direction);
    shader.setVec3("dirLight.ambient", light.ambient);
    shader.setVec3("dirLight.diffuse", light.diffuse);
    shader.setVec3("dirLight.specular", light.specular);
}

void setSpotLight(Shader &shader, const SpotLight &light) {
    shader.setVec3("spotLight.position", light.position);
    shader.setVec3("spotLight.spotDir", light.direction);
    shader.setVec3("spotLight.ambient", light.ambient);
    shader.setVec3("spotLight.diffuse", light.diffuse);
    shader.setVec3("spotLight.specular", light.specular);
    shader.setFloat("spotLight.phi", light.phi);
    shader.setFloat("spotLight.phiOuter", light.phiOuter);
    shader.setBool("spotLight.enabled", light.enabled);
}
