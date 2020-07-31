#include "stdafx.h"
#include "VertexTextured.h"

std::vector<float> operator+(const std::vector<float>& v1, const std::vector<float>& v2)
{
    std::vector<float> Result;

    for (auto i : v1)
        Result.emplace_back(i);

    for (auto i : v2)
        Result.emplace_back(i);

    return Result;
}

Ermine::VertexTextured::VertexTextured()
    :
    VertexBase()
{}
Ermine::VertexTextured::VertexTextured(glm::vec3 PositionData)
    :
    VertexBase(PositionData)
{}
Ermine::VertexTextured::VertexTextured(glm::vec3 PositionData, glm::vec3 VertexColor)
    :
    VertexBase(PositionData,VertexColor)
{}
Ermine::VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates)
    :
    VertexBase()
{
    this->VertexUVCoordinates = VertexUVCoordinates;
}
Ermine::VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates, float TextureNumber)
{
    this->VertexUVCoordinates = VertexUVCoordinates;
    this->TextureNumber = TextureNumber;
}


std::vector<float> Ermine::VertexTextured::GetVertexData() const
{
    std::vector<float> VertexData;

    //Start Load The Position In
    VertexData.emplace_back(VertexBase::Position.x);
    VertexData.emplace_back(VertexBase::Position.y);
    VertexData.emplace_back(VertexBase::Position.z);
    //Ended Load The Position In

    //Start Load The VertexColor
    VertexData.emplace_back(VertexBase::VertexColor.r);
    VertexData.emplace_back(VertexBase::VertexColor.g);
    VertexData.emplace_back(VertexBase::VertexColor.b);
    //Ended Load The Vertex Color

    //Start Load The Vertex UV Coordinates
    VertexData.emplace_back(this->VertexUVCoordinates.x);
    VertexData.emplace_back(this->VertexUVCoordinates.y);
    //Ended Load The Vertex UV Coordinates

    //Start Load The Texture Number 
    VertexData.emplace_back(TextureNumber);
    //Ended Load The Texture Number

    return VertexData;
}


void Ermine::VertexTextured::SetVertexUV(glm::vec2 VertexUVCoordinates)
{
    this->VertexUVCoordinates = VertexUVCoordinates;
}
glm::vec2 Ermine::VertexTextured::GetVertexUVCoordinates()
{
    return VertexUVCoordinates;
}

void Ermine::VertexTextured::SetTextureNumber(float TextureNumber)
{
    this->TextureNumber = TextureNumber;
}
float Ermine::VertexTextured::GetTextureNumber()
{
    return TextureNumber;
}


int Ermine::VertexTextured::GetNumberOfElementsInVertex()
{
    return 9;
}
int Ermine::VertexTextured::GetVertexSize()
{
    return 9 * sizeof(float);
}


void Ermine::VertexTextured::SetVertexAttribArray(VertexArray& Vao)
{
    static std::vector<VertexAttribPointerSpecification> Spec = {
                {3,GL_FLOAT,false},
                {3,GL_FLOAT,false},
                {2,GL_FLOAT,false},
                {1,GL_FLOAT,false}
    };

    Vao.SetVertexAttribArray(Spec);
}