#ifndef Mesh_hpp
#define Mesh_hpp

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.hpp"

#include<string>
#include<vector>
using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct ModelTexture {
    GLuint id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh data
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<ModelTexture> textures;
    
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<ModelTexture> textures);
    void Draw(Shader &shader);
    //  render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
#endif
