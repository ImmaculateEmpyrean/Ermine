#include "stdafx.h"
#include "RubeLoader.h"

#include "EngineResourceHandlers/Manifest.h"

namespace Ermine
{
    static bool CheckIfStringIsTrueLowerCase(std::string String)
    {
        //Erase The Usual " If They Are There.. Especially If You Dump Json Into This..
        String.erase(std::remove(String.begin(),String.end(),'\"'), String.end());

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

    static void removeSubstrs(std::string& string,
                              const std::string& SubstringToRemove) 
    {
        std::string::size_type n = SubstringToRemove.length();

        for (std::string::size_type i = string.find(SubstringToRemove);
            i != std::string::npos;
            i = string.find(SubstringToRemove))
            string.erase(i, n);
    }

    static std::filesystem::path GetImagePathRelativeToExecutable(std::filesystem::path PathToConsider)
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
            STDOUTDefaultLog_Error("Image Set In Rube Editor Might Not Exist In A Subfolder From Exe.. Please Check All The Paths Of The Rube Json File.. Errored Path : %s",PathToConsider.u8string().c_str());

        return std::filesystem::path(PathStr);
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

            if (i.key() == "image")
            {
                for (auto Image : i.value().items())
                {
                    std::string ImagePathStr;
                    int BodyIndex = -1; //-1 is Most Definitely Wrong Maybe Useful For Debugging.. Maybe

                    ImagePathStr = Image.value()["file"].dump();
                    ImagePathStr.erase(std::remove(ImagePathStr.begin(), ImagePathStr.end(), '\"'), ImagePathStr.end());
                    auto ImagePath = GetImagePathRelativeToExecutable(std::filesystem::path(ImagePathStr));

                    for (auto ImageProperties : Image.value().items())
                    {
                        if (ImageProperties.key() == "body")
                        {
                            BodyIndex = std::stoi(ImageProperties.value().dump());
                            break;
                        }
                    }

                    if (BodyIndex != -1)
                        Package.Sprites[BodyIndex] = Ermine::Sprite::GenerateSprite(ImagePath);
                }
            }

            if (i.key() == "joint")
            {
                for (auto Joint : i.value().items())
                {
                    if (Joint.value()["type"] == "revolute")
                    {
                        //Start Getting All The Information About The joint In Question From Rube//
                        //All Coordinates Are In Box2D Coordinates And All Angles Are In Radians When Extracted Out From The Json..

                        b2Vec2 AnchorA, AnchorB;
                        std::shared_ptr<Ermine::PhysicsComponent2D> BodyA, BodyB;
                        bool CollideConnected = false, EnableLimit = false, EnableMotor = false;
                        float upperLimit = 0.0f, lowerLimit = 0.0f, JointSpeed = 0.0f, maxMotorTorque = 0.0f, MotorSpeed = 0.0f, ReferenceAngle = 0.0f;
                        std::string JointName = "None";


                        for (auto RevoluteParameters : Joint.value().items())
                        {
                            if(RevoluteParameters.key() == "anchorA")
                                AnchorA = GetVec2FromJson(Joint.value()["anchorA"]);
                            if (RevoluteParameters.key() == "anchorB")
                                AnchorB = GetVec2FromJson(Joint.value()["anchorB"]);

                            if (RevoluteParameters.key() == "bodyA")
                                BodyA = Package.Components[std::stoi(Joint.value()["bodyA"].dump())];
                            if (RevoluteParameters.key() == "bodyB")
                                BodyB = Package.Components[std::stoi(Joint.value()["bodyB"].dump())];

                            if (RevoluteParameters.key() == "collideConnected")
                                CollideConnected = CheckIfStringIsTrueLowerCase(Joint.value()["collideConnected"].dump());

                            if (RevoluteParameters.key() == "enableLimit")
                                EnableLimit = CheckIfStringIsTrueLowerCase(Joint.value()["enableLimit"].dump());
                            if (RevoluteParameters.key() == "upperLimit")
                                upperLimit = std::stof(Joint.value()["upperLimit"].dump());
                            if (RevoluteParameters.key() == "lowerLimit")
                                lowerLimit = std::stof(Joint.value()["lowerLimit"].dump());

                            if (RevoluteParameters.key() == "enableMotor")
                                EnableMotor = CheckIfStringIsTrueLowerCase(Joint.value()["enableMotor"].dump());
                            if (RevoluteParameters.key() == "jointSpeed")
                                JointSpeed = std::stof(Joint.value()["jointSpeed"].dump());
                            if (RevoluteParameters.key() == "maxMotorTorque")
                                maxMotorTorque = std::stof(Joint.value()["maxMotorTorque"].dump());
                            if (RevoluteParameters.key() == "motorSpeed")
                                MotorSpeed = std::stof(Joint.value()["motorSpeed"].dump());

                            if (RevoluteParameters.key() == "name")
                            {
                                JointName = Joint.value()["name"];
                                JointName.erase(std::remove(JointName.begin(), JointName.end(), '\"'), JointName.end());
                            }

                            if (RevoluteParameters.key() == "refAngle")
                                ReferenceAngle = std::stof(Joint.value()["refAngle"].dump());
                        }
                        //Ended Getting All The Information About The joint In Question From Rube//

                        //Start Creating The Revolute Joint Which Is To Be Constructed.. //
                        b2RevoluteJointDef RevDef;

                        RevDef.localAnchorA = AnchorA;
                        RevDef.localAnchorB = AnchorB;

                        RevDef.bodyA = BodyA->GetBox2DBody();
                        RevDef.bodyB = BodyB->GetBox2DBody();

                        RevDef.collideConnected = CollideConnected;

                        RevDef.enableLimit = EnableLimit;
                        RevDef.upperAngle = upperLimit;
                        RevDef.lowerAngle = lowerLimit;

                        RevDef.enableMotor = EnableMotor;
                        RevDef.motorSpeed = JointSpeed; //This IS Probably Right Donot Doubt At First Shot..

                        RevDef.referenceAngle = ReferenceAngle;

                        b2Joint* JointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);
                        //Ended Constructing The Revolute Joint Which Was To Be Constructed..//

                        //Start Emplace Joint Into The Physics Component..//
                        BodyA->CreateRevoluteJoint(JointHandle, JointName, BodyB);
                        //Ended Emplace Joint Into The Physics Component..//
                    }

                    if (Joint.value()["type"] == "distance")
                    {
                        //Start Getting All The Information About The joint In Question From Rube//
                        //All Coordinates Are In Box2D Coordinates And All Angles Are In Radians When Extracted Out From The Json..
                        
                        b2Vec2 AnchorA = { 0.0f,0.0f }, AnchorB = {0.0f,0.0f};
                        std::shared_ptr<Ermine::PhysicsComponent2D> BodyA, BodyB;
                        float DampingRatio = 0.0f, frequency = 0.0f, length = 0.0f;
                        bool CollideConnected = false;
                        std::string JointName = "None";
                        
                        for (auto DistanceParameters : Joint.value().items())
                        {
                            if (DistanceParameters.key() == "anchorA")
                                AnchorA = GetVec2FromJson(Joint.value()["anchorA"]);
                            if (DistanceParameters.key() == "anchorB")
                                AnchorB = GetVec2FromJson(Joint.value()["anchorB"]);

                            if (DistanceParameters.key() == "bodyA")
                                BodyA = Package.Components[std::stoi(Joint.value()["bodyA"].dump())];
                            if (DistanceParameters.key() == "bodyB")
                                BodyB = Package.Components[std::stoi(Joint.value()["bodyB"].dump())];

                            if (DistanceParameters.key() == "collideConnected")
                                CollideConnected = CheckIfStringIsTrueLowerCase(Joint.value()["collideConnected"].dump());

                            if (DistanceParameters.key() == "dampingRatio")
                                DampingRatio = std::stof(Joint.value()["dampingRatio"].dump());
                            if (DistanceParameters.key() == "frequency")
                                frequency = std::stof(Joint.value()["frequency"].dump());
                            if (DistanceParameters.key() == "length")
                                length = std::stof(Joint.value()["length"].dump());

                            if (DistanceParameters.key() == "name")
                            {
                                JointName = Joint.value()["name"];
                                JointName.erase(std::remove(JointName.begin(), JointName.end(), '\"'), JointName.end());
                            }
                        }
                        //Ended Getting All The Information About The joint In Question From Rube//

                        //Start Creating The Distance Joint Which Is To Be Constructed.. //
                        b2DistanceJointDef DisDef;

                        DisDef.localAnchorA = AnchorA;
                        DisDef.localAnchorB = AnchorB;

                        DisDef.bodyA = BodyA->GetBox2DBody();
                        DisDef.bodyB = BodyB->GetBox2DBody();

                        DisDef.collideConnected = CollideConnected;

                        DisDef.dampingRatio = DampingRatio;
                        DisDef.frequencyHz = frequency;

                        DisDef.length = length;
                        b2Joint* JointHandle = (b2DistanceJoint*)Ermine::Universum->CreateJoint(&DisDef);
                        //Ended Creating The Distance Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        BodyA->CreateDistanceJoint(JointHandle, JointName, BodyB);
                        //Ended Emplace Joint INto Physics Component..//
                    }

                    if (Joint.value()["type"] == "rope")
                    {
                        //Start Getting All The Information About The joint In Question From Rube//
                        //All Coordinates Are In Box2D Coordinates And All Angles Are In Radians When Extracted Out From The Json..

                        b2Vec2 AnchorA = { 0.0f,0.0f }, AnchorB = { 0.0f,0.0f };
                        std::shared_ptr<Ermine::PhysicsComponent2D> BodyA, BodyB;
                        float MaxLength = 0.0f;
                        bool CollideConnected = false;
                        std::string JointName = "None";

                        for (auto RopeParameters : Joint.value().items())
                        {
                            if(RopeParameters.key() == "anchorA")
                                AnchorA = GetVec2FromJson(Joint.value()["anchorA"]);
                            if (RopeParameters.key() == "anchorB")
                                AnchorB = GetVec2FromJson(Joint.value()["anchorB"]);

                            if (RopeParameters.key() == "bodyA")
                                BodyA = Package.Components[std::stoi(Joint.value()["bodyA"].dump())];
                            if (RopeParameters.key() == "bodyB")
                                BodyB = Package.Components[std::stoi(Joint.value()["bodyB"].dump())];

                            if (RopeParameters.key() == "collideConnected")
                                CollideConnected = CheckIfStringIsTrueLowerCase(Joint.value()["collideConnected"].dump());

                            if (RopeParameters.key() == "maxLength")
                                MaxLength = std::stof(Joint.value()["maxLength"].dump());

                            if (RopeParameters.key() == "name")
                            {
                                JointName = Joint.value()["name"];
                                JointName.erase(std::remove(JointName.begin(), JointName.end(), '\"'), JointName.end());
                            }
                        }
                        //Ended Getting All The Information About The joint In Question From Rube//

                        //Start Creating The Rope Joint Which Is To Be Constructed.. //
                        b2RopeJointDef RopeDef;
                        
                        RopeDef.localAnchorA = AnchorA;
                        RopeDef.localAnchorB = AnchorB;

                        RopeDef.bodyA = BodyA->GetBox2DBody();
                        RopeDef.bodyB = BodyB->GetBox2DBody();

                        RopeDef.collideConnected = CollideConnected;

                        RopeDef.maxLength = MaxLength;
                        b2Joint* JointHandle = (b2RopeJoint*)Ermine::Universum->CreateJoint(&RopeDef);
                        //Ended Creating The Rope Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        BodyA->CreateRopeJoint(JointHandle, JointName, BodyB);
                        //Ended Emplace Joint Into Physics Component..//
                    }

                    if (Joint.value()["type"] == "weld")
                    {
                        //Start Getting All The Information About The joint In Question From Rube//
                        //All Coordinates Are In Box2D Coordinates And All Angles Are In Radians When Extracted Out From The Json..

                        b2Vec2 AnchorA = { 0.0f,0.0f }, AnchorB = { 0.0f,0.0f };
                        std::shared_ptr<Ermine::PhysicsComponent2D> BodyA, BodyB;
                        float DampingRatio = 0.0f, frequency = 0.0f, refAngle = 0.0f;
                        bool CollideConnected = false;
                        std::string JointName = "None";

                        for (auto WeldParameters : Joint.value().items())
                        {
                            if (WeldParameters.key() == "anchorA")
                                AnchorA = GetVec2FromJson(Joint.value()["anchorA"]);
                            if (WeldParameters.key() == "anchorB")
                                AnchorB = GetVec2FromJson(Joint.value()["anchorB"]);

                            if (WeldParameters.key() == "bodyA")
                                BodyA = Package.Components[std::stoi(Joint.value()["bodyA"].dump())];
                            if (WeldParameters.key() == "bodyB")
                                BodyB = Package.Components[std::stoi(Joint.value()["bodyB"].dump())];
                            
                            if (WeldParameters.key() == "collideConnected")
                                CollideConnected = CheckIfStringIsTrueLowerCase(Joint.value()["collideConnected"].dump());

                            if (WeldParameters.key() == "dampingRatio")
                                DampingRatio = std::stof(Joint.value()["dampingRatio"].dump());
                            if (WeldParameters.key() == "frequency")
                                frequency = std::stof(Joint.value()["frequency"].dump());
                            if (WeldParameters.key() == "refAngle")
                                refAngle = std::stof(Joint.value()["refAngle"].dump());

                            if (WeldParameters.key() == "name")
                            {
                                JointName = Joint.value()["name"];
                                JointName.erase(std::remove(JointName.begin(), JointName.end(), '\"'), JointName.end());
                            }                
                        }
                        //Ended Getting All The Information About The Joint In Question From Rube//

                        //Start Creating The Weld Joint Which Is To Be Constructed.. //
                        b2WeldJointDef WeldDef;

                        WeldDef.localAnchorA = AnchorA;
                        WeldDef.localAnchorB = AnchorB;

                        WeldDef.bodyA = BodyA->GetBox2DBody();
                        WeldDef.bodyB = BodyB->GetBox2DBody();

                        WeldDef.collideConnected = CollideConnected;

                        WeldDef.dampingRatio = DampingRatio;
                        WeldDef.frequencyHz = frequency;
                        WeldDef.referenceAngle = refAngle;
                        b2Joint* JointHandle = (b2WeldJoint*)Ermine::Universum->CreateJoint(&WeldDef);
                        //Ended Creating The Weld Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        BodyA->CreateWeldJoint(JointHandle, JointName, BodyB);
                        //Ended Emplace Joint Into Physics Component..//
                    }
                }
            }

        }

        return Package;
    }
}