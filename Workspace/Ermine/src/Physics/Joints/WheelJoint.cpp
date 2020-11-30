#include "stdafx.h"
#include "WheelJoint.h"

Ermine::WheelJoint::WheelJoint(b2Body* BodyA, b2Body* BodyB, bool collideConnected)
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
}


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

	jd.localAnchorA = Ermine::GLMToB2Vec2(Ermine::coordPixelsToWorld(LocalAnchorA));
	jd.localAnchorB = Ermine::GLMToB2Vec2(Ermine::coordPixelsToWorld(LocalAnchorB));

	jd.localAxisA = Ermine::GLMToB2Vec2(Ermine::vectorPixelsToWorld(LocalTranslationalAxisInBodyA));

	JointHandle = (b2WheelJoint*)Universum->CreateJoint(&jd);
}

Ermine::WheelJoint::WheelJoint(std::string Name, b2Body* BodyA, b2Body* BodyB, b2WheelJointDef Def)
	:
	JointBase(Name,BodyA,BodyB)
{
	JointHandle = (b2WheelJoint*)Universum->CreateJoint(&Def);
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
	return std::shared_ptr<Ermine::WheelJoint>(new Ermine::WheelJoint(Name, BodyA, BodyB, LocalAnchorA, LocalAnchorB, LocalTranslationalAxisInBodyA, CollideConnected), Ermine::JointDeleter<Ermine::WheelJoint>());
}

std::shared_ptr<Ermine::WheelJoint> Ermine::WheelJoint::Generate(std::string Name, b2Body* BodyA, b2Body* BodyB, b2WheelJointDef Def)
{
	return std::shared_ptr<Ermine::WheelJoint>(new Ermine::WheelJoint(Name, BodyA, BodyB, Def), Ermine::JointDeleter<Ermine::WheelJoint>());
}

glm::vec2 Ermine::WheelJoint::GetBodyALocalAnchorLocation()
{
	if(GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2WheelJoint*)JointHandle)->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
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
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyBAnchor Location Of The Wheel Joint As The Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}

	((b2WheelJoint*)JointHandle)->
}
