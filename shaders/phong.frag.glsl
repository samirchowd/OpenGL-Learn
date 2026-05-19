#version 330 core

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
    vec3 spotDir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float phi;
    float phiOuter;
    bool enabled;
};
uniform SpotLight spotLight;

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform samplerCube skybox;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSkyboxLight(vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcSkyboxLight(norm, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float theta = dot(viewDir, normalize(-light.spotDir));

    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);

    if (theta > light.phiOuter) {
        float diff = max(dot(normal, viewDir), 0.0);
        vec3 reflectDir = reflect(-viewDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
        diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
        specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
        float epsilon   = light.phi - light.phiOuter;
        float intensity = clamp((theta - light.phiOuter) / epsilon, 0.0, 1.0);
        ambient  *= intensity;
        diffuse  *= intensity;
        specular *= intensity;
    }

    return (ambient + diffuse + specular);
}

vec3 CalcSkyboxLight(vec3 normal, vec3 viewDir) {
    vec3 R = reflect(-viewDir, normalize(normal));
    return vec3(texture(skybox, R).rgb);
}
