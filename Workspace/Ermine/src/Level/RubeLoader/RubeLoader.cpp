#include "stdafx.h"
#include "RubeLoader.h"

#include "EngineResourceHandlers/Manifest.h"

#include "RubeJsonHelpers.h"
#include "RubeLoaderPackage.h"
#include "RubeJointDefinition.h"

namespace Ermine
{
    Ermine::RubeLoaderPackage Ermine::RubeLoader::ReadFile(std::filesystem::path RubeJsonFilePath)
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
                    RubeJointDefinition JointDef = RubeJointDefinition(Joint.value(), Package);

                    if (JointDef.JointType == "revolute")
                    {
                        //Start Constructing The Revolute Joint..//
                        b2RevoluteJointDef RevDef;

                        RevDef.localAnchorA = JointDef.AnchorA;
                        RevDef.localAnchorB = JointDef.AnchorB;

                        RevDef.bodyA = JointDef.BodyA->GetBox2DBody();
                        RevDef.bodyB = JointDef.BodyB->GetBox2DBody();

                        RevDef.collideConnected = JointDef.CollideConnected;

                        RevDef.enableLimit = JointDef.EnableLimit;
                        RevDef.upperAngle  = JointDef.UpperLimit;
                        RevDef.lowerAngle  = JointDef.LowerLimit;

                        RevDef.enableMotor = JointDef.EnableMotor;
                        RevDef.motorSpeed  = JointDef.MotorSpeed ;

                        RevDef.referenceAngle = JointDef.ReferenceAngle;

                        b2Joint* JointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);
                        //Ended Constructing The Revolute Joint..//

                        //Start Emplace Joint Into The Physics Component..//
                        JointDef.BodyA->CreateRevoluteJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
                        //Ended Emplace Joint Into The Physics Component..//
                    }
                    
                    if (JointDef.JointType == "distance")
                    {
                        //Start Creating The Distance Joint Which Is To Be Constructed.. //
                        b2DistanceJointDef DisDef;

                        DisDef.localAnchorA = JointDef.AnchorA;
                        DisDef.localAnchorB = JointDef.AnchorB;

                        DisDef.bodyA = JointDef.BodyA->GetBox2DBody();
                        DisDef.bodyB = JointDef.BodyB->GetBox2DBody();

                        DisDef.collideConnected = JointDef.CollideConnected;

                        DisDef.dampingRatio = JointDef.Damping;
                        DisDef.frequencyHz = JointDef.Frequency;

                        DisDef.length = JointDef.Length;
                        b2Joint* JointHandle = (b2DistanceJoint*)Ermine::Universum->CreateJoint(&DisDef);
                        //Ended Creating The Distance Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        JointDef.BodyA->CreateDistanceJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
                        //Ended Emplace Joint INto Physics Component..//
                    }

                    if (JointDef.JointType == "rope")
                    {
                        //Start Creating The Rope Joint Which Is To Be Constructed.. //
                        b2RopeJointDef RopeDef;
                        
                        RopeDef.localAnchorA = JointDef.AnchorA;
                        RopeDef.localAnchorB = JointDef.AnchorB;

                        RopeDef.bodyA = JointDef.BodyA->GetBox2DBody();
                        RopeDef.bodyB = JointDef.BodyB->GetBox2DBody();

                        RopeDef.collideConnected = JointDef.CollideConnected;

                        RopeDef.maxLength = JointDef.MaxLength;
                        b2Joint* JointHandle = (b2RopeJoint*)Ermine::Universum->CreateJoint(&RopeDef);
                        //Ended Creating The Rope Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        JointDef.BodyA->CreateRopeJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
                        //Ended Emplace Joint Into Physics Component..//
                    }

                    if (JointDef.JointType == "weld")
                    {
                        //Start Creating The Weld Joint Which Is To Be Constructed.. //
                        b2WeldJointDef WeldDef;

                        WeldDef.localAnchorA = JointDef.AnchorA;
                        WeldDef.localAnchorB = JointDef.AnchorB;

                        WeldDef.bodyA = JointDef.BodyA->GetBox2DBody();
                        WeldDef.bodyB = JointDef.BodyB->GetBox2DBody();

                        WeldDef.collideConnected = JointDef.CollideConnected;

                        WeldDef.dampingRatio   = JointDef.Damping;
                        WeldDef.frequencyHz    = JointDef.Frequency;
                        WeldDef.referenceAngle = JointDef.ReferenceAngle;
                        b2Joint* JointHandle = (b2WeldJoint*)Ermine::Universum->CreateJoint(&WeldDef);
                        //Ended Creating The Weld Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        JointDef.BodyA->CreateWeldJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
                        //Ended Emplace Joint Into Physics Component..//
                    }

                    if (JointDef.JointType == "wheel")
                    {
                        //Start Creating The Wheel Joint Which Is To Be Constructed.. //
                        b2WheelJointDef WheelDef;

                        WheelDef.localAnchorA = JointDef.AnchorA;
                        WheelDef.localAnchorB = JointDef.AnchorB;
                        WheelDef.localAxisA   = JointDef.LocalAxis;

                        WheelDef.bodyA = JointDef.BodyA->GetBox2DBody();
                        WheelDef.bodyB = JointDef.BodyB->GetBox2DBody();

                        WheelDef.collideConnected = JointDef.CollideConnected;

                        WheelDef.damping = JointDef.Damping;
                        WheelDef.stiffness = JointDef.Frequency;
                        //WheelDef.stiffness = JointDef.Frequency;

                        WheelDef.enableLimit = JointDef.EnableLimit;
                        WheelDef.enableMotor = JointDef.EnableMotor;
                        
                        WheelDef.motorSpeed = JointDef.MotorSpeed;

                        b2Joint* JointHandle = (b2WheelJoint*)Ermine::Universum->CreateJoint(&WheelDef);
                        //Ended Creating The Wheel Joint Which Is To Be Constructed.. //

                        //Start Emplace Joint Into Physics Component..//
                        JointDef.BodyA->CreateWheelJoint(JointHandle,JointDef.JointName, JointDef.BodyB);
                        //Ended Emplace Joint Into Physics Component..//
                    }
                }
            }

        }

        return Package;
    }
}

