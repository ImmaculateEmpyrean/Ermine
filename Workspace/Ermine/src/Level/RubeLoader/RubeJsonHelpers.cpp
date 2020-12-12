#include "stdafx.h"

#include "RubeJsonHelpers.h"

namespace Ermine
{
    bool CheckIfStringIsTrueLowerCase(std::string String)
    {
        //Erase The Usual " If They Are There.. Especially If You Dump Json Into This..
        String.erase(std::remove(String.begin(), String.end(), '\"'), String.end());

        if (String == "true")
            return true;
        else return false;
    }

    b2Vec2 GetVec2FromJson(nlohmann::json Vec2Json)
    {
        b2Vec2 Vec = { 0.0f,0.0f };

        for (auto i : Vec2Json.items())
        {
            if (i.key() == "x")
                Vec.x = std::stof(i.value().dump());
            if (i.key() == "y")
                Vec.y = std::stof(i.value().dump());
        }

        return Vec;
    }

    std::vector<b2Vec2> GetVerticesFromRubeJson(nlohmann::json VelocityBlock)
    {
        std::vector<b2Vec2> Vertices;

        std::vector<float> XValues;
        std::vector<float> YValues;

        for (auto i : VelocityBlock.items())
        {
            if (i.key() == "x")
            {
                for (auto j : i.value().items())
                    XValues.emplace_back(std::stof(j.value().dump()));
            }
            if (i.key() == "y")
            {
                for (auto j : i.value().items())
                    YValues.emplace_back(std::stof(j.value().dump()));
            }
        }

        if (XValues.size() != YValues.size())
            STDOUTDefaultLog_Critical("Error In Rube File.. Cannot Read Vertex Values As Number Of X Values Are Not Equal To Y Values.. Program Will Probably Behave In A Funny Way Or Crash");

        for (int i = 0; i < XValues.size(); i++)
            Vertices.emplace_back(b2Vec2(XValues[i], YValues[i]));

        return Vertices;
    }

    void removeSubstrs(std::string& string, const std::string& SubstringToRemove)
    {
        std::string::size_type n = SubstringToRemove.length();

        for (std::string::size_type i = string.find(SubstringToRemove);
            i != std::string::npos;
            i = string.find(SubstringToRemove))
            string.erase(i, n);
    }

    std::filesystem::path GetImagePathRelativeToExecutable(std::filesystem::path PathToConsider)
    {
        std::string PathStr = PathToConsider.u8string();

        auto FilePath = Ermine::Manifest::GetString("Texture");

        if (!FilePath.has_value())
        {
            STDOUTDefaultLog_Error("Manifest File May Probably Be Corrupted As We Cannot Query Key : Texture From It.. Falling Back To Default Location Hardcoded");
        }

        std::string TexturePath = FilePath.value_or("Texture");
        std::size_t found = PathStr.find(TexturePath);

        if (found != std::string::npos)
            PathStr.erase(PathStr.begin(), PathStr.begin() + found);
        else
            STDOUTDefaultLog_Error("Image Set In Rube Editor Might Not Exist In A Subfolder From Exe.. Please Check All The Paths Of The Rube Json File.. Errored Path : %s", PathToConsider.u8string().c_str());

        return std::filesystem::path(PathStr);
    }
}