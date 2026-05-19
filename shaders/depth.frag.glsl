#version 330 core

uniform float near;
uniform float far;

out vec4 FragColor;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = clamp(LinearizeDepth(gl_FragCoord.z) / 50.0, 0.0, 1.0);
    FragColor = vec4(vec3(depth), 1.0);
}
