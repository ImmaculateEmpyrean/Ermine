#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in float UseVertexColor;
layout(location = 2) in vec4 VertexColor;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

uniform vec4 Body_Color_Uniform;

out vec4 Color;

void main()
{
	
	if(UseVertexColor > 0.0)
		Color = VertexColor;
	else Color = Body_Color_Uniform;

	gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(aPos, 1.0);
}