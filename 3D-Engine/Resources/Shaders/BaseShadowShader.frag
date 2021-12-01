#version 410 core
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

float computeShadow(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 color = normal * 0.5 + 0.5;
    vec3 lightColor = vec3(0.3);

    // Ambient light
    vec3 ambient = 0.3 * lightColor;

    // Diffuse light
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 diffuse = diff * lightColor;

    float shadow = computeShadow(FragPosLightSpace);
    vec3 light = (ambient + (1.0 - shadow) * diffuse) * color;

    FragColor = vec4(light, 1.0);
}
