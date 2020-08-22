#version 330 core

layout(location = 0) in vec2 aPos;

uniform mat4 ProjectionViewMatrix;

void main()
{
	gl_Position = ProjectionViewMatrix * vec4(aPos, 1.0,1.0);
}