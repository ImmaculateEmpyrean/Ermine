#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aVertexColors;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTextureNumber;

out float TextureNumber;
out vec2 TexCoord;
out vec4 VertexColors;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;

void main()
{
	TextureNumber = aTextureNumber;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    VertexColors = aVertexColors;

	gl_Position =ProjectionViewMatrix * ModelMatrix * vec4(aPos, 1.0);
}