#include "stdafx.h"
#include "WeldJoint.h"

namespace Ermine
{
	WeldJoint::WeldJoint(b2Body* BodyA, b2Body* BodyB, bool CollideConnected)
		:
		JointBase(BodyA,BodyB)
	{
		HelperConstructWeldJoint(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), CollideConnected);
	}
	WeldJoint::WeldJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool CollideConnected)
		:
		JointBase(BodyA,BodyB)
	{
		HelperConstructWeldJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, glm::vec2(0.0f, 0.0f), CollideConnected);
	}
	WeldJoint::WeldJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool CollideConnected)
		:
		JointBase(BodyA,BodyB)
	{
		HelperConstructWeldJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre, CollideConnected);
	}

	WeldJoint::~WeldJoint()
	{
		if (WeldJointHandle == nullptr)
		{
			Ermine::Universum->DestroyJoint(WeldJointHandle);
			WeldJointHandle = nullptr;
		}
	}


	glm::vec2 WeldJoint::GetBodyALocalAnchorLocation()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = WeldJointHandle->GetLocalAnchorA();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}
	glm::vec2 WeldJoint::GetBodyBLocalAnchorLocation()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = WeldJointHandle->GetLocalAnchorB();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}

	glm::vec2 WeldJoint::GetBodyAWorldAnchorLocationPixels()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = WeldJointHandle->GetAnchorA();
			glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}
	glm::vec2 WeldJoint::GetBodyBWorldAnchorLocationPixels()
	{
		if (ValidFlag == true)
		{
			b2Vec2 LocalAnchorLocation = WeldJointHandle->GetAnchorB();
			glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else return glm::vec2(-9999.0f, -9999.0f);
	}


	void WeldJoint::HelperConstructWeldJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool CollideConnected)
	{
		glm::vec2 AnchorAWithRespectToBoxCentreWorldSpace = vertexPixelsToWorld(AnchorAWithRespectToBoxCentre);
		glm::vec2 AnchorBWithRespectToBoxCentreWorldSpace = vertexPixelsToWorld(AnchorBWithRespectToBoxCentre);

		b2WeldJointDef WeldDef;

		WeldDef.bodyA = BodyA;
		WeldDef.bodyB = BodyB;
		WeldDef.collideConnected = CollideConnected;

		WeldDef.localAnchorA = Ermine::GLMToB2Vec2(AnchorAWithRespectToBoxCentreWorldSpace);
		WeldDef.localAnchorB = Ermine::GLMToB2Vec2(AnchorBWithRespectToBoxCentreWorldSpace);

		WeldJointHandle = (b2WeldJoint*)Ermine::Universum->CreateJoint(&WeldDef);

		JointBase::JointHandle = WeldJointHandle;
	}
}
