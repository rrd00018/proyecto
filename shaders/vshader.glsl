#version 330

in vec4 vPosition;
uniform mat4 M;
void main()
{
    gl_Position = M*vec4(vPosition);
}
