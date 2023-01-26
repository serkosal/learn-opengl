#version 330

in vec2 TexCoord;

uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
    FragColor = texture(Texture, TexCoord);
}