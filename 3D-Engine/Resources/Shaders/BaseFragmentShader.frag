#version 410 core

in vec3 normal;
in vec2 tex;

uniform sampler2D diffuse;

out vec4 color;

void main()
{
    vec3 tmp = normalize(normal) * 0.5 + 0.5;
    //tmp = tmp * texture(diffuse, tex).rgb;

    color = vec4(vec3(tex, 1.0), 1.0);
}
