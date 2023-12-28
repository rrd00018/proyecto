#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 cameraPosition;

out vec3 Normal;
out vec3 FragPos;
out vec4 CameraPos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
    vec4 viewPosition = V * M * vec4(vPosition, 1.0);
    gl_Position = P * viewPosition;

    // Escribe las variables interpoladas
    Normal = mat3(transpose(inverse(M))) * vNormal;
    FragPos = viewPosition.xyz;
    CameraPos = cameraPosition;
}
