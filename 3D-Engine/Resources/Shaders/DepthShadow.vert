#version 410 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;

out vec4 v_color;

void main()
{
        gl_Position = lightSpaceMatrix * vec4(position, 1.0);
}
