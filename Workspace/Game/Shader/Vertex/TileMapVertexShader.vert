#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTextureNumber;
out float TextureNumber;
out vec2 TexCoord;
uniform mat4 ProjectionViewMatrix;
void main()
{
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	TextureNumber = aTextureNumber;
	gl_Position =ProjectionViewMatrix * vec4(aPos, 1.0);
}