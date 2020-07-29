#version 330 core \n
out vec4 FragColor;

in vec2 TexCoord;
in float TextureNumber;

uniform sampler2D[16] Sampler2DArray; 

void main()
{ 
    int TextureNumberInt = int(TextureNumber);
    FragColor = texture(Sampler2DArray[TextureNumberInt],TexCoord);
}