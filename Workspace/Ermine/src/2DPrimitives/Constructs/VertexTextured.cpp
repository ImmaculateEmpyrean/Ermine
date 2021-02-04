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
    VertexTextured::VertexTextured(glm::vec2 VertexUVCoordinates, int TextureNumber)
    {
        UV = VertexUVCoordinates;
        Tex = TextureNumber;
    }
    VertexTextured::VertexTextured(glm::vec3 PositionData, glm::vec4 VertexColor, glm::vec2 VertexUVCoordinates, int TextureNumber)
        :
        VertexBase(PositionData, VertexColor)
    {
        UV = VertexUVCoordinates;
        Tex = TextureNumber;
    }

    std::vector<std::vector<VertexDataObject>>&& Ermine::VertexTextured::GetVertexBufferSeperated(std::vector<VertexDataObject>& LargeContainerHavingMyltipleVertexBuffers)
    {
        std::vector<std::vector<VertexDataObject>> Container;
        int c = 0;
        for (VertexDataObject i : LargeContainerHavingMyltipleVertexBuffers)
        {
            if (c == VertexTextured::GetNumberOfElementsInVertex())
            {
                c = 0;
                Container.emplace_back(std::vector<VertexDataObject>());
            }

            Container[Container.size() - 1].emplace_back(i);
            c++;
        }
        return std::move(Container);
    }


    std::vector<VertexDataObject> Ermine::VertexTextured::GetVertexData() const
    {
        std::vector<VertexDataObject> VertexData = VertexBase::GetVertexData();
 
        VertexData.emplace_back(UV.x);
        VertexData.emplace_back(UV.y);
       
        VertexData.emplace_back(Tex);
        
        return VertexData;
    }


    VertexLayout Ermine::VertexTextured::GetVertexBufferLayout()
    {
        auto Lay = VertexBase::GetVertexBufferLayout();
        Lay.AddSpecification(2, GL_FLOAT);
        Lay.AddSpecification(1, GL_INT);

        return Lay;
    }


    VertexTextured::operator std::vector<Ermine::VertexDataObject>() const
    {
        std::vector<VertexDataObject> Obj = VertexBase::operator std::vector<Ermine::VertexDataObject>();
        
        Obj.emplace_back(UV.x);
        Obj.emplace_back(UV.y);

        Obj.emplace_back(Tex);
    }
}