#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoord;

void main()
{
    TexCoord = aTex;

    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}