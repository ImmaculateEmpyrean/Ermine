#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
    vec4 Color = texture(texture1, TexCoord);
    FragColor = Color;
   // FragColor = vec4(1.0,1.0,1.0,Color.a);
	//FragColor = vec4(Color.r,Color.g,Color.b,1.0);
}