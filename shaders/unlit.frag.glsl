#version 330 core

uniform sampler2D texture_diffuse1;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(texture(texture_diffuse1, TexCoords)) * 0.4, 1.0);
}
