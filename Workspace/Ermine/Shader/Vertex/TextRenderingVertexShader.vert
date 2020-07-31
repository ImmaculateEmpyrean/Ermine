#version 330 core
layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aVertexColors; 
layout(location = 2) in vec2 aTexCoord; 
layout(location = 3) in float aTextureNumber;

out vec3 TexColor;
out vec2 TexCoord; 
out float TextureNumber;

uniform mat4 ModelMatrix; 
uniform mat4 ProjectionViewMatrix; 

void main() 
{ 
    TexColor = aVertexColors;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    TextureNumber = aTextureNumber;
    gl_Position = ProjectionViewMatrix*ModelMatrix*vec4(aPos, 1.0); 
} 