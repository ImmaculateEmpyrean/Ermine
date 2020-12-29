#include "stdafx.h"
#include "VertexTextured.h"

//TODO - Get Rid Of These Two Headers As They Have No Business Sitting Here..
#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace Ermine
{
    VertexTextured::VertexTextured()
        :
        VertexBase()
    {}
    VertexTextured::VertexTextured(glm::vec3 PositionData)
        :
        VertexBase(PositionData)
    {}
    VertexTextured::VertexTextured(glm::vec3 PositionData, glm::vec4 VertexColor)
        :
        VertexBase(PositionData, VertexColor)
    {}
    VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates)
        :
        VertexBase()
    {
        UV = VertexUVCoordinates;
    }
    VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates, float TextureNumber)
    {
        UV = VertexUVCoordinates;
        Tex = TextureNumber;
    }
    VertexTextured::VertexTextured(glm::vec3 PositionData, glm::vec4 VertexColor, glm::vec2 VertexUVCoordinates, float TextureNumber)
        :
        VertexBase(PositionData, VertexColor)
    {
        UV = VertexUVCoordinates;
        Tex = TextureNumber;
    }
    VertexTextured::VertexTextured(std::vector<float> Vertex)
        :
        VertexBase(Vertex)
    {
        UV.x = Vertex[7];
        UV.y = Vertex[8];

        Tex = Vertex[9];
    }


    std::vector<std::vector<float>>&& Ermine::VertexTextured::GetVertexBufferSeperated(std::vector<float>& LargeContainerHavingMyltipleVertexBuffers)
    {
        std::vector<std::vector<float>> Container;
        int c = 0;
        for (float i : LargeContainerHavingMyltipleVertexBuffers)
        {
            if (c == VertexTextured::GetNumberOfElementsInVertex())
            {
                c = 0;
                Container.emplace_back(std::vector<float>());
            }

            Container[Container.size() - 1].emplace_back(i);
            c++;
        }
        return std::move(Container);
    }


    std::vector<float> Ermine::VertexTextured::GetVertexData() const
    {
        std::vector<float> VertexData = VertexBase::GetVertexData();
 
        VertexData.emplace_back(UV.x);
        VertexData.emplace_back(UV.y);
       
        VertexData.emplace_back(Tex);
        
        return VertexData;
    }


    VertexLayout Ermine::VertexTextured::GetVertexBufferLayout()
    {
        auto Lay = VertexBase::GetVertexBufferLayout();
        Lay.AddSpecification(2, GL_FLOAT);
        Lay.AddSpecification(1, GL_FLOAT);

        return Lay;
    }
}