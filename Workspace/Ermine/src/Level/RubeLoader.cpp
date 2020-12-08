#include "stdafx.h"
#include "RubeLoader.h"

namespace Ermine
{
    static bool CheckIfStringIsTrueLowerCase(std::string String)
    {
        if (String == "true")
            return true;
        else return false;
    }

    static b2Vec2 GetVec2FromJson(nlohmann::json Vec2Json)
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

    static std::vector<b2Vec2> GetVerticesFromRubeJson(nlohmann::json VelocityBlock)
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

    RubeLoaderPackage Ermine::RubeLoader::ReadFile(std::filesystem::path RubeJsonFilePath)
    {
        RubeLoaderPackage Package;
       
        nlohmann::json File;
        std::ifstream RawFile(RubeJsonFilePath);

        File << RawFile;

        for (auto i : File.items())
        {
            if (i.key() == "body")
            {
                for (auto body : i.value().items())
                {
                    b2BodyDef BodyDef;
                    std::vector<b2FixtureDef> Fixtures;
                    std::string Name;
                    
                    for (auto BodyParamenters : body.value().items())
                    {
                        if (BodyParamenters.key() == "type")
                            BodyDef.type = b2BodyType(std::stoi(BodyParamenters.value().dump()));

                        if (BodyParamenters.key() == "angle")
                            BodyDef.angle = std::stof(BodyParamenters.value().dump());

                        if (BodyParamenters.key() == "angularVelocity")
                            BodyDef.angularVelocity = std::stof(BodyParamenters.value().dump());

                        if (BodyParamenters.key() == "awake")
                            BodyDef.awake = CheckIfStringIsTrueLowerCase(BodyParamenters.value().dump());

                        if (BodyParamenters.key() == "linearVelocity")
                            BodyDef.linearVelocity = GetVec2FromJson(BodyParamenters.value());

                        if (BodyParamenters.key() == "position")
                            BodyDef.position = GetVec2FromJson(BodyParamenters.value());

                        if (BodyParamenters.key() == "name")
                            Name = BodyParamenters.value();

                        if (BodyParamenters.key() == "fixture")
                        {
                            for (auto Fixture : BodyParamenters.value().items())
                            {
                                b2FixtureDef FixtureDef;
                                std::string Name; //Not USed As Of Now

                                for (auto FixtureParameters : Fixture.value().items())
                                {
                                    if (FixtureParameters.key() == "density")
                                        FixtureDef.density = std::stof(FixtureParameters.value().dump());

                                    if (FixtureParameters.key() == "friction")
                                        FixtureDef.density = std::stof(FixtureParameters.value().dump());

                                    if (FixtureParameters.key() == "name")
                                        Name = (FixtureParameters.value().dump());

                                    if (FixtureParameters.key() == "polygon")
                                    {
                                        std::vector<b2Vec2> Vertices = GetVerticesFromRubeJson(FixtureParameters.value()["vertices"]);
                                        
                                        b2PolygonShape* PolyShape = new b2PolygonShape();
                                        PolyShape->Set(Vertices.data(), Vertices.size());

                                        FixtureDef.shape = PolyShape;
                                    }

                                    if (FixtureParameters.key() == "circle")
                                    {
                                        b2CircleShape* CircleShape = new b2CircleShape();
                                        CircleShape->m_radius = std::stof(FixtureParameters.value()["radius"].dump());
                                        CircleShape->m_p      = GetVec2FromJson(FixtureParameters.value()["center"]);

                                        FixtureDef.shape = CircleShape;
                                    }
                                }

                                //Use These Fixtures When Creating The Body Buffer.
                                Fixtures.emplace_back(FixtureDef);
                            }
                        }
                    }
                    //Create Body Here As All Parameters Are Accounted For By Now
                    Package.Components.emplace_back(Ermine::PhysicsComponent2D::Generate(BodyDef, Fixtures));
                }
            }

            if (i.key() == "gravity")
            {
                Package.Gravity.x = std::stof(i.value()["x"].dump());
                Package.Gravity.y = std::stof(i.value()["y"].dump());
            }
        }

        return Package;
    }
}
