#include "stdafx.h"
#include "RevoluteJoint.h"

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructRevoluteJoint(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
	HelperConstructRevoluteJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, glm::vec2(0.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
	HelperConstructRevoluteJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre, 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
	float ReferenceAngle = glm::radians<float>(ReferenceAngleDegrees);
	HelperConstructRevoluteJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre, ReferenceAngle, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::~RevoluteJoint()
{
	if (ValidFlag == true)
	{
		Ermine::Universum->DestroyJoint(RevoluteJointHandle);
		RevoluteJointHandle = nullptr;
	}
}


void Ermine::RevoluteJoint::SetRotationLimits(float LowerLimitDegrees, float UpperLimitDegrees)
{
	float LowerLimitRadians = glm::radians<float>(LowerLimitDegrees);
	float UpperLimitRadians = glm::radians<float>(UpperLimitDegrees);

	RevoluteJointHandle->EnableLimit(true);
	RevoluteJointHandle->SetLimits(LowerLimitRadians, UpperLimitRadians);
}
void Ermine::RevoluteJoint::SetRotationLimits(float LimitDegrees, bool IsUpperLimit)
{
	float LimitRadians = glm::radians<float>(LimitDegrees);
	
	RevoluteJointHandle->EnableLimit(true);

	if(IsUpperLimit == true)
		RevoluteJointHandle->SetLimits(-99999.0f, LimitRadians);
	else 
		RevoluteJointHandle->SetLimits(LimitRadians, 99999.0f);
}

void Ermine::RevoluteJoint::ClearLimits()
{
	RevoluteJointHandle->EnableLimit(false);
}

glm::vec2 Ermine::RevoluteJoint::GetBodyALocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RevoluteJointHandle->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}

glm::vec2 Ermine::RevoluteJoint::GetBodyBLocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RevoluteJointHandle->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}

glm::vec2 Ermine::RevoluteJoint::GetBodyAWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RevoluteJointHandle->GetAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}

glm::vec2 Ermine::RevoluteJoint::GetBodyBWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RevoluteJointHandle->GetAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}


#pragma region MotorFunctions

void Ermine::RevoluteJoint::EnableMotor(bool enabled)
{
	if(ValidFlag == true)
		RevoluteJointHandle->EnableMotor(enabled);
}
void Ermine::RevoluteJoint::SetMotorSpeed(float speed)
{
	if (ValidFlag == true)
		RevoluteJointHandle->SetMotorSpeed(speed);
}
void Ermine::RevoluteJoint::SetMaxMotorTorque(float torque)
{
	if (ValidFlag == true)
		RevoluteJointHandle->SetMaxMotorTorque(torque);
}

bool Ermine::RevoluteJoint::IsMotorEnabled()
{
	if (ValidFlag == true)
		return RevoluteJointHandle->IsMotorEnabled();
	else return false;
}
float Ermine::RevoluteJoint::GetMotorSpeed()
{
	if (ValidFlag == true)
		return RevoluteJointHandle->GetMotorSpeed();
	else return -9999.0f;
}
float Ermine::RevoluteJoint::GetMotorTorque()
{
	if (ValidFlag == true)
		return RevoluteJointHandle->GetMotorTorque(1.0f / PhysicsWorldTimestep);
	else return -9999.0f;
}

#pragma endregion MotorFunctions


float Ermine::RevoluteJoint::GetReferenceAngleInDegrees()
{
	if (ValidFlag == true)
		return glm::degrees<float>(GetReferenceAngleInRadians());
	else return -9999.0f;
}
float Ermine::RevoluteJoint::GetReferenceAngleInRadians()
{
	if (ValidFlag == true)
		return RevoluteJointHandle->GetReferenceAngle();
	else return -9999.0f;
}


void Ermine::RevoluteJoint::HelperConstructRevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngle, bool ShouldBodiesAttachedByTheJointCollide)
{
	glm::vec2 LocalAnchorAWorldSpace = Ermine::vertexPixelsToWorld(AnchorAWithRespectToBoxCentre);
	glm::vec2 LocalAnchorBWorldSpace = Ermine::vertexPixelsToWorld(AnchorBWithRespectToBoxCentre);

	b2RevoluteJointDef RevDef;

	RevDef.bodyA = BodyA;
	RevDef.bodyB = BodyB;
	RevDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	RevDef.localAnchorA = b2Vec2(LocalAnchorAWorldSpace.x, LocalAnchorAWorldSpace.y);
	RevDef.localAnchorB = b2Vec2(LocalAnchorBWorldSpace.x, LocalAnchorBWorldSpace.y);

	RevDef.referenceAngle = ReferenceAngle;

	RevoluteJointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);

	JointBase::JointHandle = RevoluteJointHandle;
}
