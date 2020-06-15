#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPosition;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D dispMap;

void main(){
    vec3 Pos = aPos + aNormal * 0.1 * texture(dispMap, aTexCoords).r;
    gl_Position = projection * view * model * vec4(Pos,1.0);
    TexCoords = aTexCoords;
    FragPosition = vec3(model*vec4(Pos,1.0));
    Normal = aNormal;
}
