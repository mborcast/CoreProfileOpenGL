#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal_cameraspace;
out vec3 eyeDirection;
out vec3 lightDirection;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 spotlightPosition;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    Normal_cameraspace = view * model * vec4(normal, 0);
    lightDirection = vec3(spotlightPosition - gl_Position);
    eyeDirection = vec3(-gl_Position);
} 