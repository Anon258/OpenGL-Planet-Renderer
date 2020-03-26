#version 330 core

in vec3 ourCol;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D text1;
uniform sampler2D text2;

void main(){
    color = texture(text1, TexCoord);
}
