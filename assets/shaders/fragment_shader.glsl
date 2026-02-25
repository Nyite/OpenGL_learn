#version 330
in vec3 fragPos;
in vec3 normalIterp;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 eyePos;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normalIterp);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseSthrength = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diffuseSthrength * lightColor;

    vec3 eyeVec = normalize(eyePos - fragPos);
    vec3 reflection = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(reflection, eyeVec), 0.0), 32);
    float specularStrength = 0.5;
    vec3 specular = spec * specularStrength * lightColor;

    // Phong lighting
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
