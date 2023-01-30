#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    TexCoord = aTex;
    Normal = mat3(transpose(inverse(model))) * aNor;  
    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * vec4(FragPos, 1.0);
}