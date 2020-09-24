#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

uniform vec4 Body_Color;

out vec4 Color;

void main()
{
	Color = Body_Color;
	gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(aPos, 1.0);
}