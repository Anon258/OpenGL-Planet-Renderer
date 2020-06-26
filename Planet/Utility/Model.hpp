#ifndef Model_hpp
#define Model_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Textures.hpp"
#include "Mesh.hpp"
using namespace std;

class Model{
public:
    vector<ModelTexture> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma){
        loadModel(path);
    }
    void Draw(Shader &shader){
        for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
    }
private:
    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<ModelTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
#endif
