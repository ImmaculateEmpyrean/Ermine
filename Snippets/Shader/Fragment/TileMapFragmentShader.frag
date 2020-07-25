#version 330 core
out vec4 FragColor;
in float TextureNumber;
in vec2 TexCoord;
uniform sampler2D[16] Sampler2DArray;
void main()
{
int TextureNumberInt = int(TextureNumber);
FragColor = texture(Sampler2DArray[TextureNumberInt], TexCoord);
}