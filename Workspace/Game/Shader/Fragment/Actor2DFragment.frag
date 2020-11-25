#version 330 core

out vec4 FragColor;

in float TextureNumber;
in vec2 TexCoord;
in vec4 VertexColors;

uniform sampler2D[16] Sampler2DArray;

void main()
{
    int TextureNumberInt = int(TextureNumber);

    if(TextureNumberInt>=16)
    {
        FragColor = VertexColors;
    }
    else
    {
        FragColor = texture(Sampler2DArray[TextureNumberInt], TexCoord);
    }

}