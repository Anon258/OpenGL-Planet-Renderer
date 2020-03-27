#version 330 core

out vec4 color;

in vec3 ourCol;
in vec2 TexCoord;

uniform sampler2D text1;
uniform sampler2D text2;

void main(){
    color = mix(texture(text1, TexCoord), texture(text2, TexCoord), 0.0);
}
