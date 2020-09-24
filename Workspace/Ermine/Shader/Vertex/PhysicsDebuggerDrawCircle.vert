#version 330 core

layout(location=0)in vec3  aVertex;
layout(location=1)in float aRadius;
layout(location=2)in vec2  aScreenResolution;
layout(location=3)in vec2  aCircleCentre;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

out float Radius;
out vec2  ScreenResolution;
out vec2  CircleCentre;

void main() 
{
    Radius = aRadius;
    ScreenResolution = aScreenResolution;
    CircleCentre = aCircleCentre;

    gl_Position =  ProjectionViewMatrix * ModelMatrix * vec4(aVertex,1.0);
}
