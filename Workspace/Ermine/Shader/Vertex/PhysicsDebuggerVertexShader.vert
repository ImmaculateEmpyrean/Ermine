#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

out vec4 Color;

void main()
{
	Color = vec4(255.0,255.0,255.0,0);
	gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(aPos, 1.0);
}