#version 330 core

layout(location=0)in vec3 vertex;

uniform mat4 ProjectionViewMatrix;

void main() 
{
    gl_Position =  ProjectionViewMatrix * vec4(vertex,1.0);
}
