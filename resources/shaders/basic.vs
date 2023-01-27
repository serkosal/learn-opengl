#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
    TexCoord = aTex;
    Normal = aNor;

    gl_Position = projection * model * view * vec4(aPos, 1.0);
}