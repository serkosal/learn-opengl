#version 330

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Light{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//uniform Light light;

uniform sampler2D diffuse_text;
uniform sampler2D specular_text;

uniform vec3 viewPos;


out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(1.0);
    vec3 lightColor = vec3(1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 result = (ambient + diffuse) * vec3(texture(diffuse_text, TexCoord));
    result += spec * vec3(texture(specular_text, TexCoord));


    FragColor = vec4(result, 1.0);
}