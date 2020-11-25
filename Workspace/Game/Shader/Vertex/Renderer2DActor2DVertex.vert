#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aVertexColors;
layout(location = 2) in vec2 aTexCoord;
out vec3 aVertexColor;
out vec2 TexCoord;
uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;
void main()
{
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	aVertexColor=aVertexColors;
	gl_Position =ProjectionViewMatrix*ModelMatrix*vec4(aPos, 1.0);
}