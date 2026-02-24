#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fragPos;
out vec3 normalIterp;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);

    fragPos = vec3(model * vec4(Pos, 1.0));
    normalIterp = Normal;
}
