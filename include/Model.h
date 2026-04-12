#pragma once

#include <memory>
#include <string>
#include <vector>

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Model {
public:
  Model(const char *path) {
    loadModel(path);
  }
  void Draw(Shader &shader);

private:
  std::vector<Mesh> meshes;
  std::vector<std::shared_ptr<Texture>> textures_loaded;
  std::string directory;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                             std::string typeName);
};
