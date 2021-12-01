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

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 color = vec3(1.0);
    vec3 lightColor = vec3(0.3);

    // Ambient light
    vec3 ambient = 0.3 * lightColor;

    // Diffuse light
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    float shadow = computeShadow(FragPosLightSpace);
    vec3 light = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(light, 1.0);
}
