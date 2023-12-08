#version 330
in vec4 vPosition;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main()
{
    gl_Position = P*V*M*vec4(vPosition);
}
