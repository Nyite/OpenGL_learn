#version 330
in vec3 fragPos;
in vec3 normalIterp;
in vec3 lightPosInterp;
in vec2 textureInterp;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
    vec3 ambient = light.ambient * texture(material.diffuse, textureInterp).rgb;

    vec3 norm = normalize(normalIterp);
    vec3 lightDir = normalize(lightPosInterp - fragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(material.diffuse, textureInterp).rgb;

    vec3 eyeVec = normalize(-fragPos);
    vec3 reflection = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(reflection, eyeVec), 0.0), 32);
    vec3 specular = spec * light.specular * texture(material.specular, textureInterp).rgb;

    // Phong lighting
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
