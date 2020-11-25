#version 330 core
out vec4 FragColor;

in vec3 TexColor;
in vec2 TexCoord;
in float TextureNumber;

uniform sampler2D[16] Sampler2DArray; 

void main()
{ 
    int TextureNumberInt = int(TextureNumber);
    FragColor = vec4(TexColor.r,TexColor.g,TexColor.b,texture(Sampler2DArray[TextureNumberInt],TexCoord).r);
    //vec4(1.0f,1.0f,1.0f,1.0f);
}