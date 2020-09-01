#include "stdafx.h"
#include "WheelJoint.h"

Ermine::WheelJoint::WheelJoint(b2Body* BodyA, b2Body* BodyB, bool collideConnected)
	:
	JointBase(BodyA,BodyB)
{
	b2WheelJointDef WDef;

	WDef.bodyA = BodyA;
	WDef.bodyB = BodyB;

	WDef.collideConnected = collideConnected;

	WDef.localAnchorA = b2Vec2(Ermine::scalarPixelsToWorld(100.0f), Ermine::scalarPixelsToWorld(-150.0f));
	WDef.localAnchorB = b2Vec2(0.0f, 0.0f);

	//WDef.damping = 100.0f;
	//WDef.stiffness = 20.0f;
	WDef.motorSpeed = glm::radians<float>(10);
	WDef.maxMotorTorque = 100.0f;
	WDef.enableMotor = true;

	WDef.enableLimit = true;
	WDef.upperTranslation = 1.0f;
	WDef.lowerTranslation = 1.0f;
	//WDef.enableLimit = true;

	WDef.localAxisA = b2Vec2(1.0f, 0.0f);

	WheelJointHandle = (b2WheelJoint*)Universum->CreateJoint(&WDef);
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
