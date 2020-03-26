#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourCol;
uniform float x,y;

void main(){
    gl_Position = vec4(aPos.x+x, aPos.y+y, aPos.z, 1.0);
    ourCol = aColor;
}
