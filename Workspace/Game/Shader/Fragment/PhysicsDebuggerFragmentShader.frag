#version 330 core

out vec4 Frag_Color;

in vec4 Color;

void main()
{
    Frag_Color = Color;
}