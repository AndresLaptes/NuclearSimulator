#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

out vec3 N;
out vec3 L;
out vec3 V;
out vec3 color2;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightPosition;

void main() {
    N = normalMatrix * normal;
    vec3 P = (modelViewMatrix * vec4(vertex, 1.0)).xyz;

    L = (lightPosition.xyz - P);
    V = -P;
    color2 = color;
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}