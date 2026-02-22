#version 330
in vec2 texPosInterp;
uniform sampler2D tex1;
uniform sampler2D tex2;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(tex1, texPosInterp), texture(tex2, texPosInterp), 0.2);
}
