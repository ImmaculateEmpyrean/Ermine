#version 330 core

layout(location=0)in vec3  aVertex;
layout(location=1)in float aRadius;
layout(location=2)in vec2  aScreenResolution;
layout(location=3)in vec2  aCircleCentre;
layout(location=4)in float aUseVertexColor;
layout(location=5)in vec4  aVertexColor;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;
uniform vec4 Uniform_CircleColor;

out float Radius;
out vec2  ScreenResolution;
out vec2  CircleCentre;
out vec4  Color;

void main() 
{
    Radius = aRadius;
    ScreenResolution = aScreenResolution;
    CircleCentre = aCircleCentre;

    if(aUseVertexColor > 0.0f)
        Color = aVertexColor;            
    else Color = Uniform_CircleColor;

    gl_Position =  ProjectionViewMatrix * ModelMatrix * vec4(aVertex,1.0);
}
