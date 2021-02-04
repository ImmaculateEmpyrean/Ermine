#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in int TextureNumber;

uniform sampler2D[16] Sampler2DArray;

void main()
{ 
    //int TextureNumberInt = int(ceil(TextureNumber));
    FragColor = texture(Sampler2DArray[TextureNumber],TexCoord);
}