#version 410 core

struct Material {
    sampler2D diffuseMap;
    sampler2D lightsMap;
    sampler2D specularMap;
};

struct DirLight {
    vec3 direction;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosition;
in vec3 Normal;

uniform vec3 cameraPos;
uniform Material material;
uniform DirLight dirLight;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    // get original color
    vec3 ambient = texture(material.lightsMap, TexCoords).rgb;
    
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuseMap, TexCoords));
    
    //specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec * texture(material.specularMap, TexCoords).r;
    
    return (ambient+diffuse+specular);
}

void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPosition);
    vec3 light = calcDirLight(dirLight, norm, viewDir);
    
    FragColor = vec4(light, 1.0);
}
