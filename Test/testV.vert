#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;

out vec3 fragColor;

uniform mat4 modelViewProjectionMatrix;

void main() {
    fragColor = color;
    gl_Position  = modelViewProjectionMatrix * vec4(vertex, 1.0);
}