#version 330
in vec2 texPosInterp;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0);
}
