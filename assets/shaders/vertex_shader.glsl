#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 texPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texPosInterp;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);
    texPosInterp = texPos;
}
