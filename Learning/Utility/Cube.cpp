#include "Cube.hpp"

void Cube::face(int a, int b, int c, int d){
    glPoints[pos_index] = positions[a]; pos_index++;
    glPoints[pos_index] = positions[b]; pos_index++;
    glPoints[pos_index] = positions[c]; pos_index++;
    glPoints[pos_index] = positions[a]; pos_index++;
    glPoints[pos_index] = positions[c]; pos_index++;
    glPoints[pos_index] = positions[d]; pos_index++;
}

void Cube::colorcube(){
    face(0,1,2,3); //front face
    face(0,1,5,4); //bottom face
    face(4,5,6,7); //back face
    face(3,2,6,7); //top face
    face(0,3,7,4); //left face
    face(1,2,6,5); //right face
}

Cube::Cube(GLchar* vertshad, GLchar* fragshad){
    
    positions[0]=glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f); //front bottom left
    positions[1]=glm::vec4(0.5f, -0.5f, 0.5f, 1.0f); //front bottom right
    positions[2]=glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); //front top right
    positions[3]=glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);//front top left
    positions[4]=glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f); //back bottom left
    positions[5]=glm::vec4(0.5f, -0.5f, -0.5f, 1.0f); //back bottom right
    positions[6]=glm::vec4(0.5f, 0.5f, -0.5f, 1.0f); //back top right
    positions[7]=glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f); //back top left
    
    pos_index = 0;

    //Next we create a vertex data to draw
    shader = new Shader(vertshad, fragshad);
    colorcube();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glPoints), glPoints, GL_STATIC_DRAW);

    glVertexAttribPointer(0,4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
