#include "stdafx.h"
#include "VertexTextured.h"

Ermine::VertexTextured::VertexTextured()
    :
    VertexBase()
{}
Ermine::VertexTextured::VertexTextured(glm::vec3 PositionData)
{
}
Ermine::VertexTextured::VertexTextured(glm::vec3 PositionData, glm::vec3 VertexColor)
{
}
Ermine::VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates)
{
}
Ermine::VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates, float TextureNumber)
{
}


std::vector<float> Ermine::VertexTextured::GetVertexData()
{
    return std::vector<float>();
}

void Ermine::VertexTextured::SetVertexUV(glm::vec2 VertexUVCoordinates)
{
}

glm::vec2 Ermine::VertexTextured::GetVertexUVCoordinates()
{
    return glm::vec2();
}

void Ermine::VertexTextured::SetPositonCoordinates(float Position)
{
}

float Ermine::VertexTextured::GetPositionCoordinates()
{
    return 0.0f;
}

int Ermine::VertexTextured::GetNumberOfElementsInVertex()
{
    return 0;
}

int Ermine::VertexTextured::GetVertexSize()
{
    return 0;
}

void Ermine::VertexTextured::SetVertexAttribArray()
{
}

std::vector<float> Ermine::VertexTextured::operator+(const VertexTextured& rhs)
{
    return std::vector<float>();
}
