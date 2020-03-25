#include "Cube.hpp"

void Cube::face(int a, int b, int c, int d){
    glPoints[pos_index] = positions[a]; glColors[pos_index] = colors[a]; pos_index++;
    glPoints[pos_index] = positions[b]; glColors[pos_index] = colors[b]; pos_index++;
    glPoints[pos_index] = positions[c]; glColors[pos_index] = colors[c]; pos_index++;
    glPoints[pos_index] = positions[a]; glColors[pos_index] = colors[a]; pos_index++;
    glPoints[pos_index] = positions[c]; glColors[pos_index] = colors[c]; pos_index++;
    glPoints[pos_index] = positions[d]; glColors[pos_index] = colors[d]; pos_index++;
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
    
    colors[0]=glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //front bottom left
    colors[1]=glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); //front bottom right
    colors[2]=glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //front top right
    colors[3]=glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); //front top left
    colors[4]=glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //back bottom left
    colors[5]=glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); //back bottom right
    colors[6]=glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); //back top right
    colors[7]=glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); //back top left
    
    pos_index = 0;

    //Next we create a vertex data to draw
    shader = new Shader(vertshad, fragshad);
    colorcube();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glPoints) + sizeof(glColors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glPoints), glPoints);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glPoints), sizeof(glColors), glColors);

    glVertexAttribPointer(0,4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(sizeof(glPoints)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
