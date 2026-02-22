#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 texPos;

out vec2 texPosInterp;

void main()
{
    gl_Position = vec4(Pos, 1.0);
    texPosInterp = texPos;
}
