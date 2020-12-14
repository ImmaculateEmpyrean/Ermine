#include "stdafx.h"
#include "RubeJointDefinition.h"

#include "EngineResourceHandlers/Manifest.h"

#include "RubeJsonHelpers.h"

Ermine::RubeJointDefinition::RubeJointDefinition(nlohmann::json Joint,RubeLoaderPackage& Package)
{
    for (auto Parameter : Joint.items())
    {
        //Start Get Anchors//
        if (Parameter.key() == "anchorA")
            AnchorA = GetVec2FromJson(Joint["anchorA"]);
        if (Parameter.key() == "anchorB")
            AnchorB = GetVec2FromJson(Joint["anchorB"]);
        //Ended Get Anchors// Tot -2

        //Start Get Bodies//
        if (Parameter.key() == "bodyA")
            BodyA = Package.Components[std::stoi(Joint["bodyA"].dump())];
        if (Parameter.key() == "bodyB")
            BodyB = Package.Components[std::stoi(Joint["bodyB"].dump())];
        //Ended Get Bodies// Tot -4

        //Start Get Flags//
        if (Parameter.key() == "collideConnected") 
            CollideConnected = CheckIfStringIsTrueLowerCase(Joint["collideConnected"].dump());
        if (Parameter.key() == "enableLimit")
            EnableLimit = CheckIfStringIsTrueLowerCase(Joint["enableLimit"].dump());
        if (Parameter.key() == "enableMotor")
            EnableMotor = CheckIfStringIsTrueLowerCase(Joint["enableMotor"].dump());
        //Ended Get Flags// Tot -7

        //Start Get limits And Lengths//
        if (Parameter.key() == "upperLimit")
            UpperLimit = std::stof(Joint["upperLimit"].dump());
        if (Parameter.key() == "lowerLimit")
            LowerLimit = std::stof(Joint["lowerLimit"].dump());
        if (Parameter.key() == "length")
            Length = std::stof(Joint["length"].dump());
        if (Parameter.key() == "maxLength")
            MaxLength = std::stof(Joint["maxLength"].dump());
        //Ended Get limits And Lengths// Tot -11
        
        //Start Motor Options//
        if (Parameter.key() == "jointSpeed")                        
            MotorSpeed = std::stof(Joint["jointSpeed"].dump());     ///Both These Write To Motor Speed Beware..
        if (Parameter.key() == "motorSpeed")
            MotorSpeed = std::stof(Joint["motorSpeed"].dump());     ///Both Of These Write To Motor Speed Beware..

        if (Parameter.key() == "maxMotorForce")
            MaxMotorForce = std::stof(Joint["maxMotorForce"].dump());     ///This Writes To MaxMotorForce Beware Just Like Atleast One Other Query..

        if (Parameter.key() == "maxMotorTorque")                    ///Both Of These Write To Motor Torque Beware..
            MaxMotorTorque = std::stof(Joint["maxMotorTorque"].dump());
        if (Parameter.key() == "maxTorque")
            MaxMotorTorque = std::stof(Joint["maxTorque"].dump()); ///Both Of These Write To Motor Torque Beware..
        //Ended Motor Options// Tot -14

        //Start Get Name//
        if (Parameter.key() == "name")
        {
            JointName = Joint["name"];
            JointName.erase(std::remove(JointName.begin(), JointName.end(), '\"'), JointName.end());
        }

        if (Parameter.key() == "type")
        {
            JointType = Joint["type"];
            JointType.erase(std::remove(JointType.begin(), JointType.end(), '\"'), JointType.end());
        }
        //Ended Get Name// Tot -16

        //Start Get Misc//
        if (Parameter.key() == "refAngle")
            ReferenceAngle = std::stof(Joint["refAngle"].dump());
        if (Parameter.key() == "springDampingRatio")
            Damping = std::stof(Joint["springDampingRatio"].dump());
        if (Parameter.key() == "springFrequency")
            Frequency = std::stof(Joint["springFrequency"].dump());
        if (Parameter.key() == "localAxisA")
            LocalAxis = GetVec2FromJson(Joint["localAxisA"]);
        //Ended Get Misc// Tot -20

        //Start Motor Joint Specifics..//
        if (Parameter.key() == "correctionFactor")
            CorrectionFactor = std::stof(Joint["correctionFactor"].dump());

        if (Parameter.key() == "maxForce")
            MaxMotorForce = std::stof(Joint["maxForce"].dump()); ///This Writes To MaxMotorForce Beware Just Like Atleast One Other Query..

        if (Parameter.key() == "linearOffset")
            LinearOffset = GetVec2FromJson(Joint["linearOffset"]);
        //Ended Motor Joint Specifics..//
    }
}


