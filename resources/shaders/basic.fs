#version 330

in vec2 TexCoord;

uniform sampler2D diffuse;

out vec4 FragColor;

void main()
{
    FragColor = texture(diffuse, TexCoord);
}