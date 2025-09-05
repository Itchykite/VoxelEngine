#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 vertexColor;  

uniform vec3 viewPos;
uniform vec3 sunDirection;
uniform vec3 sunColor;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * sunColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-sunDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * sunColor;
    
    vec3 result = (ambient + diffuse) * vertexColor; 
    FragColor = vec4(result, 1.0);
}
