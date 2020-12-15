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
                    b2MassData BodyDefMassData;

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

                        if (BodyParamenters.key() == "massData-I")
                            BodyDefMassData.I = std::stof(BodyParamenters.value().dump());

                        if (BodyParamenters.key() == "massData-center")
                            BodyDefMassData.center = GetVec2FromJson(BodyParamenters.value());

                        if (BodyParamenters.key() == "massData-mass")
                            BodyDefMassData.mass = std::stof(BodyParamenters.value().dump());

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
                    //Start Create Body Here As All Parameters Are Accounted For By Now
                    auto Component = Ermine::PhysicsComponent2D::Generate(BodyDef, Fixtures);
                    Component->GetBox2DBody()->SetMassData(&BodyDefMassData);
                    Package.Components.emplace_back(Component);
                    //Ended Create Body Here As All Parameters Are Accounted For By Now
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
                    std::filesystem::path ImagePath;

                    int BodyIndex = -1; //-1 is Most Definitely Wrong Maybe Useful For Debugging.. Maybe
                    int RenderOrder = 0;
                    
                    for (auto ImageParameter : Image.value().items())
                    {
                        if (ImageParameter.key() == "file")
                        {
                            ImagePathStr = Image.value()["file"].dump();
                            ImagePathStr.erase(std::remove(ImagePathStr.begin(), ImagePathStr.end(), '\"'), ImagePathStr.end());
                            ImagePath = GetImagePathRelativeToExecutable(std::filesystem::path(ImagePathStr));
                        }

                        if(ImageParameter.key() == "body")
                            BodyIndex = std::stoi(ImageParameter.value().dump());

                        if (ImageParameter.key() == "renderOrder")
                            RenderOrder = std::stoi(ImageParameter.value().dump());
                    }
                    if (BodyIndex != -1)
                    {
                        Package.Sprites[BodyIndex] = Ermine::Sprite::GenerateSprite(ImagePath);
                        Package.RenderOrder.insert(std::pair<unsigned int, unsigned int>(RenderOrder, BodyIndex));
                    }     
                }

              
            }

            if (i.key() == "joint")
            {
                for (auto Joint : i.value().items())
                {
                    RubeJointDefinition JointDef = RubeJointDefinition(Joint.value(), Package);

                    if (JointDef.JointType == "revolute")
                        ConstructRevoluteJoint(JointDef);
                    
                    if (JointDef.JointType == "distance")
                        ConstructDistanceJoint(JointDef);

                    if (JointDef.JointType == "rope")
                        ConstructRopeJoint(JointDef);

                    if (JointDef.JointType == "weld")
                        ConstructWeldJoint(JointDef);

                    if (JointDef.JointType == "wheel")
                        ConstructWheelJoint(JointDef);

                    if (JointDef.JointType == "motor")
                        ConstructMotorJoint(JointDef);

                    if (JointDef.JointType == "prismatic")
                        ConstructPrismaticJoint(JointDef);

                    if (JointDef.JointType == "friction")
                        ConstructFrictionJoint(JointDef);
                }
            }

        }

        return Package;
    }

#pragma region ConstructJoints

    void RubeLoader::ConstructRevoluteJoint(RubeJointDefinition& JointDef)
    {
        b2RevoluteJointDef RevDef;

        RevDef.localAnchorA = JointDef.AnchorA;
        RevDef.localAnchorB = JointDef.AnchorB;

        RevDef.bodyA = JointDef.BodyA->GetBox2DBody();
        RevDef.bodyB = JointDef.BodyB->GetBox2DBody();

        RevDef.collideConnected = JointDef.CollideConnected;

        RevDef.enableLimit = JointDef.EnableLimit;
        RevDef.upperAngle = JointDef.UpperLimit;
        RevDef.lowerAngle = JointDef.LowerLimit;

        RevDef.enableMotor = JointDef.EnableMotor;
        RevDef.motorSpeed = JointDef.MotorSpeed;

        RevDef.referenceAngle = JointDef.ReferenceAngle;

        b2Joint* JointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);

        JointDef.BodyA->CreateRevoluteJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
    }

    void RubeLoader::ConstructDistanceJoint(RubeJointDefinition& JointDef)
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

        JointDef.BodyA->CreateDistanceJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
    }

    void RubeLoader::ConstructRopeJoint(RubeJointDefinition& JointDef)
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

    void RubeLoader::ConstructWeldJoint(RubeJointDefinition& JointDef)
    {
        //Start Creating The Weld Joint Which Is To Be Constructed.. //
        b2WeldJointDef WeldDef;

        WeldDef.localAnchorA = JointDef.AnchorA;
        WeldDef.localAnchorB = JointDef.AnchorB;

        WeldDef.bodyA = JointDef.BodyA->GetBox2DBody();
        WeldDef.bodyB = JointDef.BodyB->GetBox2DBody();

        WeldDef.collideConnected = JointDef.CollideConnected;

        WeldDef.dampingRatio = JointDef.Damping;
        WeldDef.frequencyHz = JointDef.Frequency;
        WeldDef.referenceAngle = JointDef.ReferenceAngle;
        b2Joint* JointHandle = (b2WeldJoint*)Ermine::Universum->CreateJoint(&WeldDef);
        //Ended Creating The Weld Joint Which Is To Be Constructed.. //

        //Start Emplace Joint Into Physics Component..//
        JointDef.BodyA->CreateWeldJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
        //Ended Emplace Joint Into Physics Component..//
    }

    void RubeLoader::ConstructWheelJoint(RubeJointDefinition& JointDef)
    {
        //Start Creating The Wheel Joint Which Is To Be Constructed.. //
        b2WheelJointDef WheelDef;

        WheelDef.localAnchorA = JointDef.AnchorA;
        WheelDef.localAnchorB = JointDef.AnchorB;
        WheelDef.localAxisA = JointDef.LocalAxis;

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
        JointDef.BodyA->CreateWheelJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
        //Ended Emplace Joint Into Physics Component..//
    }

    void RubeLoader::ConstructMotorJoint(RubeJointDefinition& JointDef)
    {
        //Start Constructing The Motor Joint To Be Constrycted..
        b2MotorJointDef MotorDef;

        MotorDef.bodyA = JointDef.BodyA->GetBox2DBody();
        MotorDef.bodyB = JointDef.BodyB->GetBox2DBody();

        MotorDef.collideConnected = JointDef.CollideConnected;

        MotorDef.linearOffset = JointDef.LinearOffset;

        MotorDef.angularOffset = JointDef.ReferenceAngle;

        MotorDef.maxForce = JointDef.MaxMotorForce;
        MotorDef.maxTorque = JointDef.MaxMotorTorque; //This Might Not Be Wrong So Not Criticize At First Shot.. :X

        MotorDef.correctionFactor = JointDef.CorrectionFactor;

        b2Joint* JointHandle = (b2MotorJoint*)Ermine::Universum->CreateJoint(&MotorDef);
        //Ended Creating The Wheel Joint Which Is To Be Constructed.. //

        //Start Emplace Joint Into Physics Component..//
        JointDef.BodyA->CreateMotorJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
        //Ended Emplace Joint Into Physics Component..//
    }

    void RubeLoader::ConstructPrismaticJoint(RubeJointDefinition& JointDef)
    {
        //Start Constructing The Prismatic Joint
        b2PrismaticJointDef PrismaticDef;

        PrismaticDef.localAnchorA = JointDef.AnchorA;
        PrismaticDef.localAnchorB = JointDef.AnchorB;

        PrismaticDef.bodyA = JointDef.BodyA->GetBox2DBody();
        PrismaticDef.bodyB = JointDef.BodyB->GetBox2DBody();

        PrismaticDef.collideConnected = JointDef.CollideConnected;

        PrismaticDef.referenceAngle = JointDef.ReferenceAngle;
        
        PrismaticDef.enableLimit = JointDef.EnableLimit;
        PrismaticDef.lowerTranslation = JointDef.LowerLimit;
        PrismaticDef.upperTranslation = JointDef.UpperLimit;

        PrismaticDef.localAxisA = JointDef.LocalAxis;

        PrismaticDef.enableMotor = JointDef.EnableMotor;
        PrismaticDef.maxMotorForce = JointDef.MaxMotorForce;
        PrismaticDef.motorSpeed = JointDef.MotorSpeed;

        b2Joint* JointHandle = (b2PrismaticJoint*)Ermine::Universum->CreateJoint(&PrismaticDef);
        //Ended Creating The Prismatic Joint Which Is To Be Constructed.. //

        //Start Emplace Joint Into Physics Component..//
        JointDef.BodyA->CreatePrismaticJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
        //Ended Emplace Joint Into Physics Component..//
    }

    void RubeLoader::ConstructFrictionJoint(RubeJointDefinition& JointDef)
    {
        //Start Constructing The 
        b2FrictionJointDef FDef;
        
        FDef.bodyA = JointDef.BodyA->GetBox2DBody();
        FDef.bodyB = JointDef.BodyB->GetBox2DBody();

        FDef.collideConnected = JointDef.CollideConnected;

        FDef.localAnchorA = JointDef.AnchorA;
        FDef.localAnchorB = JointDef.AnchorB;

        FDef.maxForce = JointDef.MaxMotorForce;
        FDef.maxTorque = JointDef.MaxMotorTorque;
        
        b2Joint* JointHandle = (b2FrictionJoint*)Ermine::Universum->CreateJoint(&FDef);
        //Ended Creating The Prismatic Joint Which Is To Be Constructed.. //

        //Start Emplace Joint Into Physics Component..//
        JointDef.BodyA->CreateFrictionJoint(JointHandle, JointDef.JointName, JointDef.BodyB);
        //Ended Emplace Joint Into Physics Component..//

    }
#pragma endregion

}

