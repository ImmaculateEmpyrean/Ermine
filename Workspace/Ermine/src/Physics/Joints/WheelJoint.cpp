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
	WheelJointHandle = (b2WheelJoint*)Universum->CreateJoint(&jd);

	//WheelJointHandle = (b2WheelJoint*)Universum->CreateJoint(&WDef);
	JointBase::JointHandle = WheelJointHandle;
}


glm::vec2 Ermine::WheelJoint::GetBodyALocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = WheelJointHandle->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
glm::vec2 Ermine::WheelJoint::GetBodyBLocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = WheelJointHandle->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}

glm::vec2 Ermine::WheelJoint::GetBodyAWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = WheelJointHandle->GetAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
glm::vec2 Ermine::WheelJoint::GetBodyBWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = WheelJointHandle->GetAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
