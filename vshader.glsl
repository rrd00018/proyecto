#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 vNormal;


out vec3 Normal;
out vec3 FragPos;
out vec2 TextCoords;
out vec3 cameraPositionWorld;
out vec3 lightPositionWorld;
out vec3 viewDir;
out vec3 lightDir;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 cameraPosition;
uniform vec3 lightPos;



void main() {
    gl_Position = P * V * M * vPosition;

    //Convierte la posicion del vertice a coordenadas del mundo
    FragPos = vec3(V * M * vPosition);

    TextCoords = textureCoordinates;

    //Transform the camera positon to world coordinates
    cameraPositionWorld = vec3(M * vec4(cameraPosition,1));

    //Transform the light position to world coordinates
    lightPositionWorld = vec3(M * vec4(lightPos,1));

    //Vector that goes from the vertex to the camera in camera coordinates
    viewDir = cameraPosition - vec3(V * vec4(FragPos,1));

    //Vector that goes from the vertex to the light source in camera coordinates
    lightDir = vec3(V * vec4(lightPositionWorld,1.0)) + viewDir;

    //Tranform the normal to camera coordinates
    Normal = vec3(V * M * vec4(vNormal,0));
}
