#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sun;

void main(){
    FragColor = vec4(texture(sun,TexCoords).rgb, 1.0);
}
