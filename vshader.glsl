#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 textureCoordinates;



out vec3 Normal;
out vec3 FragPos;
out vec2 TextCoords;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


#define M_PI 3.1415926535897932384626433832795

void main() {
    gl_Position = P * V * M * vPosition;

    //Convierte la posicion del vertice a coordenadas del mundo
    FragPos =  vec3(M * vPosition);

    float s = acos(vPosition.x/(1/sqrt(3)))/M_PI;
    float t = (atan(vPosition.z/vPosition.y)/M_PI)+0.5;
    TextCoords = vec2(s,t);

    //Transform the camera positon to world coordinates
    Normal = vNormal;

}
