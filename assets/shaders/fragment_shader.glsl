#version 330
in vec3 fragPos;
in vec3 normalIterp;
in vec3 lightPosInterp;

uniform vec3 objectColor;
uniform vec3 eyePos;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    uint shine;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(normalIterp);
    vec3 lightDir = normalize(lightPosInterp - fragPos);
    float diffuseSthrength = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diffuseSthrength * light.diffuse * material.diffuse;

    vec3 eyeVec = normalize(-fragPos);
    vec3 reflection = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(reflection, eyeVec), 0.0), 32);
    vec3 specular = spec * light.specular * material.specular;

    // Phong lighting
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
