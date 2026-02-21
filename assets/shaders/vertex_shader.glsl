#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 vColor;

uniform vec3 offset;

out vec4 fColor;

void main()
{
    gl_Position = vec4(Pos + offset, 1.0);
    fColor = gl_Position;
}
