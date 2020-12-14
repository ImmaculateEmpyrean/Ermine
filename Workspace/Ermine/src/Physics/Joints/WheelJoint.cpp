#include "stdafx.h"
#include "WheelJoint.h"

/*Ermine::WheelJoint::WheelJoint(b2Body* BodyA, b2Body* BodyB, bool collideConnected)
	:
	JointBase(BodyA,BodyB)
{
	b2WheelJointDef jd;
	b2Vec2 axis(0.0f, 1.0f);

	float mass1 = BodyB->GetMass();
	
	float hertz = 4.0f;
	float dampingRatio = 0.7f;
	float omega = 2.0f * b2_pi * hertz;

	jd.Initialize(BodyA, BodyB, BodyB->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = true;
	jd.stiffness = mass1 * omega * omega;
	jd.damping = 2.0f * mass1 * dampingRatio * omega;
	jd.lowerTranslation = -0.25f;
	jd.upperTranslation = 0.25f;
	jd.enableLimit = true;

	JointHandle = (b2WheelJoint*)Universum->CreateJoint(&jd);
}*/


Ermine::WheelJoint::WheelJoint(std::string Name, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, glm::vec2 LocalTranslationalAxisInBodyA, bool CollideConnected)
	:
	JointBase(Name,BodyA,BodyB)
{
	
	b2WheelJointDef jd;
	b2Vec2 axis(0.0f, 1.0f);

	float mass1 = BodyB->GetMass();
	float hertz = 4.0f;
	float dampingRatio = 0.7f;
	float omega = 2.0f * b2_pi * hertz;

	jd.Initialize(BodyA, BodyB, BodyB->GetPosition(), axis);
	jd.collideConnected = CollideConnected;
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = true;
	jd.stiffness = mass1 * omega * omega;
	jd.damping = 2.0f * mass1 * dampingRatio * omega;
	jd.lowerTranslation = -0.25f;
	jd.upperTranslation = 0.25f;
	jd.enableLimit = true;

	jd.localAnchorA = Ermine::GLMToB2Vec2(Ermine::coordErmineToWorld(LocalAnchorA));
	jd.localAnchorB = Ermine::GLMToB2Vec2(Ermine::coordErmineToWorld(LocalAnchorB));

	jd.localAxisA = Ermine::GLMToB2Vec2(Ermine::vectorErmineToWorld(LocalTranslationalAxisInBodyA));

	JointHandle = (b2WheelJoint*)Universum->CreateJoint(&jd);
}

Ermine::WheelJoint::WheelJoint(std::string Name, b2Body* BodyA, b2Body* BodyB, b2WheelJointDef Def)
	:
	JointBase(Name,BodyA,BodyB)
{
	//Convert The Ermine Coordinates Into Physics World Coordinates Before Using.. THE AXIS NEED NOT BE CONVERTED RIGHT..
	Def.localAnchorA = Ermine::GLMToB2Vec2(Ermine::coordErmineToWorld(Ermine::B2Vec2ToGLM(Def.localAnchorA)));
	Def.localAnchorB = Ermine::GLMToB2Vec2(Ermine::coordErmineToWorld(Ermine::B2Vec2ToGLM(Def.localAnchorB)));

	JointHandle = (b2WheelJoint*)Universum->CreateJoint(&Def);
}

Ermine::WheelJoint::WheelJoint(b2Joint* Pointer,std::string Name, b2Body* BodyA, b2Body* BodyB)
	:
	JointBase(Name, BodyA, BodyB)
{
	//This Is The Reason Why This Constructor Is Dangerous.. The pointer Is  Essentially Constructed Somewhere else..
	JointHandle = Pointer;
}

Ermine::WheelJoint::~WheelJoint()
{}

Ermine::WheelJoint::WheelJoint(WheelJoint&& rhs)
	:
	JointBase(std::move(rhs))
{}

Ermine::WheelJoint& Ermine::WheelJoint::operator=(WheelJoint&& rhs)
{
	JointBase::operator=(std::move(rhs));
	return *this;
}

std::shared_ptr<Ermine::WheelJoint> Ermine::WheelJoint::Generate(std::string Name, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, glm::vec2 LocalTranslationalAxisInBodyA, bool CollideConnected)
{
	return std::shared_ptr<Ermine::WheelJoint>(new Ermine::WheelJoint(Name, BodyA, BodyB, LocalAnchorA, LocalAnchorB, LocalTranslationalAxisInBodyA, CollideConnected));
}

std::shared_ptr<Ermine::WheelJoint> Ermine::WheelJoint::Generate(std::string Name, b2Body* BodyA, b2Body* BodyB, b2WheelJointDef Def)
{
	return std::shared_ptr<Ermine::WheelJoint>(new Ermine::WheelJoint(Name, BodyA, BodyB, Def));
}

std::shared_ptr<Ermine::WheelJoint> Ermine::WheelJoint::Generate(b2Joint* Joint, std::string Name, b2Body* BodyA, b2Body* BodyB)
{
	return std::shared_ptr<Ermine::WheelJoint>(new Ermine::WheelJoint(Joint,Name, BodyA, BodyB));
}

glm::vec2 Ermine::WheelJoint::GetBodyALocalAnchorLocation()
{
	if(GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2WheelJoint*)JointHandle)->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyAAnchor Location Of The Wheel Joint As The Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}
glm::vec2 Ermine::WheelJoint::GetBodyBLocalAnchorLocation()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2WheelJoint*)JointHandle)->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyBAnchor Location Of The Wheel Joint As The Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}

bool Ermine::WheelJoint::IsMotorEnabled()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2WheelJoint*)JointHandle)->IsMotorEnabled();
	else {
		STDOUTDefaultLog_Error("Cannot Query IsMotorEnabled As Wheel Joint Health Is Not OK");
		return false;
	}
}

void Ermine::WheelJoint::EnableMotor()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2WheelJoint*)JointHandle)->EnableMotor(true);
	else STDOUTDefaultLog_Error("Cannot Enable Motor As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::DisableMotor()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2WheelJoint*)JointHandle)->EnableMotor(false);
	else STDOUTDefaultLog_Error("Cannot Enable Motor As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::ToggleMotor()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Flag = IsMotorEnabled();
		if (Flag == true)
			DisableMotor();
		else
			EnableMotor();
	}
	else STDOUTDefaultLog_Error("Cannot Toggle Motor As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::SetMaxMotorTorque(float Torque)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2WheelJoint*)JointHandle)->SetMaxMotorTorque(Torque);
	else STDOUTDefaultLog_Error("Cannot Set Max Motor Torque As Wheel Joint Health Is Not OK");
}

float Ermine::WheelJoint::GetMotorTorque()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetMotorTorque(0.04f));
	else {
		STDOUTDefaultLog_Error("Cannot Query Motor Torque As Wheel Joint Health Is Not OK");
		return false;
	}
}

void Ermine::WheelJoint::SetMotorSpeedRadians(float Angle)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2WheelJoint*)JointHandle)->SetMotorSpeed(Angle);
	else STDOUTDefaultLog_Error("Cannot Set MotorSpeed As Wheel Joint Health Is Not OK");
}

float Ermine::WheelJoint::GetMotorSpeedRadians()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2WheelJoint*)JointHandle)->GetMotorSpeed();
	else
	{
		STDOUTDefaultLog_Error("Cannot Query MotorSpeed As Wheel Joint Health Is Not OK");
		return -9999.0f;
	}
}

void Ermine::WheelJoint::SetMotorSpeedDegrees(float Angle)
{
	SetMotorSpeedRadians(glm::radians<float>(Angle));
}

float Ermine::WheelJoint::GetMotorSpeedDegrees()
{
	return glm::degrees<float>(GetMotorSpeedRadians());
}

bool Ermine::WheelJoint::IsLimitEnabled()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2WheelJoint*)JointHandle)->IsLimitEnabled();
	else {
		STDOUTDefaultLog_Error("Cannot Query IsLimitEnabled As Wheel Joint Health Is Not OK");
		return false;
	}
}

void Ermine::WheelJoint::EnableLimit()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2WheelJoint*)JointHandle)->EnableLimit(true);
	else STDOUTDefaultLog_Error("Cannot Enable Limit As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::DisableLimit()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2WheelJoint*)JointHandle)->EnableLimit(false);
	else STDOUTDefaultLog_Error("Cannot Disable Motor As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::ToggleLimit()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Flag = IsLimitEnabled();
		if (Flag == true)
			DisableLimit();
		else
			EnableLimit();
	}
	else STDOUTDefaultLog_Error("Cannot Toggle Limit As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::SetLimit(float Lower, float Upper)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float WorldLower = Ermine::scalarErmineToWorld(Lower);
		float WorldUpper = Ermine::scalarErmineToWorld(Upper);

		((b2WheelJoint*)JointHandle)->SetLimits(WorldLower, WorldUpper);
	}
	else STDOUTDefaultLog_Error("Cannot Set Limits As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::SetUpperLimit(float Upper)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float WorldLower = Ermine::scalarErmineToWorld(GetLowerLimit());
		float WorldUpper = Ermine::scalarErmineToWorld(Upper);

		((b2WheelJoint*)JointHandle)->SetLimits(WorldLower, WorldUpper);
	}
	else STDOUTDefaultLog_Error("Cannot Set Upper Limit As Wheel Joint Health Is Not OK");
}

void Ermine::WheelJoint::SetLowerLimit(float Lower)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float WorldLower = Ermine::scalarErmineToWorld(Lower);
		float WorldUpper = Ermine::scalarErmineToWorld(GetUpperLimit());

		((b2WheelJoint*)JointHandle)->SetLimits(WorldLower, WorldUpper);
	}
	else STDOUTDefaultLog_Error("Cannot Set Lower Limit As Wheel Joint Health Is Not OK");
}

glm::vec2 Ermine::WheelJoint::GetLimits()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return glm::vec2(GetLowerLimit(), GetUpperLimit());
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Limits As Wheel Joint Health Is Not OK")
		return glm::vec2(9999.0f, -9999.0f);
	}
}

float Ermine::WheelJoint::GetUpperLimit()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		if (((b2WheelJoint*)JointHandle)->GetUpperLimit() > 9999.0f)
		{
			STDOUTDefaultLog_Warn("Attempting To Get Upper Limit Of A Wheel joint Where None Was Previously Set.. This May LEad To Undefined Behaviour");
			return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetUpperLimit());
		}
		else return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetUpperLimit());
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get UpperLimit Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}

float Ermine::WheelJoint::GetLowerLimit()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		if (((b2WheelJoint*)JointHandle)->GetLowerLimit() < -9999.0f)
		{
			STDOUTDefaultLog_Warn("Attempting To Get LowerLimit Of A Wheel joint Where None Was Previously Set.. This May LEad To Undefined Behaviour");
			return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetLowerLimit());
		}
		else return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetLowerLimit());
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get LowerLimit Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}


float Ermine::WheelJoint::GetJointAngleDegrees()
{
	return glm::degrees<float>(GetJointAngleRadians());
}
float Ermine::WheelJoint::GetJointAngleRadians()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2WheelJoint*)JointHandle)->GetJointAngle();
	else
	{
		STDOUTDefaultLog_Error("Cannot Get JointAngle Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}

float Ermine::WheelJoint::GetJointAngularSpeedDegrees()
{
	return glm::degrees<float>(GetJointAngularSpeedRadians());
}
float Ermine::WheelJoint::GetJointAngularSpeedRadians()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2WheelJoint*)JointHandle)->GetJointAngularSpeed();
	else
	{
		STDOUTDefaultLog_Error("Cannot Get JointAngularSpeed Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}

float Ermine::WheelJoint::GetJointLinearSpeed()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetJointLinearSpeed());
	else
	{
		STDOUTDefaultLog_Error("Cannot Get JointLinearSpeed Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}

float Ermine::WheelJoint::GetJointTranslation()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetJointTranslation());
	else
	{
		STDOUTDefaultLog_Error("Cannot Get JointTranslation Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}

void Ermine::WheelJoint::SetJointStiffness(float Stiffness)
{
	if	 (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		 ((b2WheelJoint*)JointHandle)->SetStiffness(Stiffness);
	else STDOUTDefaultLog_Error("Cannot Get JointStiffness Of The Wheel Joint As Its Health Is Not Okay");
}

float Ermine::WheelJoint::GetJointStiffness()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return Ermine::scalarWorldToErmine(((b2WheelJoint*)JointHandle)->GetStiffness());
	else
	{
		STDOUTDefaultLog_Error("Cannot Get JointStiffness Of The Wheel Joint As Its Health Is Not Okay");
		return 9999.0f;
	}
}