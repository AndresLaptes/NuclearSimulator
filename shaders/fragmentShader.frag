#version 330 core

in vec3 N;
in vec3 L;
in vec3 V;
out vec4 fragColor;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightPosition;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float matShininess;
uniform vec3 color2;


vec4 Phong(vec3 N, vec3 V, vec3 L) {
    V = normalize(V);
    L = normalize(L);
    N = normalize(N);

    float NL = max(0.0, dot(N, L));

    vec3 R = normalize(2.0 * dot(N,L) * N - L);
    float RV = max(0.0, dot(R, V));
    float Idiff = NL;
    float Ispec = 0;

    if (NL > 0) Ispec = pow(RV, matShininess);
    return  matAmbient * lightAmbient + matDiffuse * lightDiffuse * Idiff + matSpecular * lightSpecular * Ispec;
} 

void main()
{
    fragColor = Phong(N, V, L) * vec4(color2, 1.0);
}