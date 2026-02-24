#version 330
in vec3 fragPos;
in vec3 normalIterp;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normalIterp);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseSthrength = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diffuseSthrength * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
