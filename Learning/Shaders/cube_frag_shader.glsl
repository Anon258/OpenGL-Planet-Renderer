#version 330 core

in vec3 ourCol;
out vec4 color;
void main(){
    color = vec4(ourCol,1.0f);
}
