#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //ambient light - this is the default color which will be present even if light is missing
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor; //the ambient light
    
    //next we apply 2nd layer of light - which is the diffuse lighting - properly orientated fragments are more well lit, we will calcluate the angle of light with each fragment
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  //vector from fragment, not towards
    float diff = max(dot(lightDir,norm),0.0); //diffuse constant
    vec3 diffuse = diff * lightColor; //The diffused light
    
    //Finally, we apply last layer of light - specular lighting, the bright spot on shiny objects. This depends on reflected light direction as well as the view direction, so we need a viewPos
    float specularStrength = 0.8; //this gives the intensity value
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  //reflect function expects incident ray from light to fragment, so we negate lightDir
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
