#include "stdafx.h"
#include "DistanceJoint.h"

namespace Ermine
{
	Ermine::DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB)
	{
		ConstructDistanceJointHelper(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), ShouldBodiesAttachedByTheJointCollide);
	}
	DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB)
	{
		ConstructDistanceJointHelper(BodyA, BodyB, AnchorAWithRespectToBoxCentre, glm::vec2(0.0f, 0.0f), ShouldBodiesAttachedByTheJointCollide);
	}
	DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB)
	{
		ConstructDistanceJointHelper(BodyA, BodyB, AnchorAWithRespectToBoxCentre ,AnchorBWithRespectToBoxCentre, ShouldBodiesAttachedByTheJointCollide);
	}

	DistanceJoint::~DistanceJoint()
	{
		if (ValidFlag == true)
		{
			Universum->DestroyJoint(DistanceJointHandle);
			DistanceJointHandle = nullptr;
		}
	}


	glm::vec2 DistanceJoint::GetBodyALocalAnchorLocation()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = DistanceJointHandle->GetLocalAnchorA();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}
	glm::vec2 DistanceJoint::GetBodyBLocalAnchorLocation()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = DistanceJointHandle->GetLocalAnchorB();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}

	glm::vec2 DistanceJoint::GetBodyAWorldAnchorLocationPixels()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = DistanceJointHandle->GetAnchorA();
			glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}
	glm::vec2 DistanceJoint::GetBodyBWorldAnchorLocationPixels()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = DistanceJointHandle->GetAnchorB();
			glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}


	void DistanceJoint::ConstructDistanceJointHelper(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	{
		glm::vec2 LocalAnchorAWorldSpace = Ermine::vertexPixelsToWorld(AnchorAWithRespectToBoxCentre);
		glm::vec2 LocalAnchorBWorldSpace = Ermine::vertexPixelsToWorld(AnchorBWithRespectToBoxCentre);

		b2DistanceJointDef DisDef;

		DisDef.bodyA = BodyA;
		DisDef.bodyB = BodyB;
		DisDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

		DisDef.localAnchorA = b2Vec2(LocalAnchorAWorldSpace.x,LocalAnchorAWorldSpace.y);
		DisDef.localAnchorB = b2Vec2(LocalAnchorBWorldSpace.x,LocalAnchorBWorldSpace.y);

		DistanceJointHandle = (b2DistanceJoint*)Ermine::Universum->CreateJoint(&DisDef);

		JointBase::JointHandle = DistanceJointHandle;
	}
}
