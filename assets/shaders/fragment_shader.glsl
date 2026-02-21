#version 330

uniform sampler2D 2dText;

out vec4 FragColor;

void main()
{
    FragColor = textture(2dText);
}
