#include "stdafx.h"
#include "WeldJoint.h"

namespace Ermine
{
	WeldJoint::WeldJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool CollideConnected)
		:
		JointBase(JointName,BodyA,BodyB)
	{
		b2Vec2 AnchorA = Ermine::GLMToB2Vec2(vertexErmineToWorld(AnchorAWithRespectToBoxCentre));
		b2Vec2 AnchorB = Ermine::GLMToB2Vec2(vertexErmineToWorld(AnchorBWithRespectToBoxCentre));

		b2WeldJointDef WeldDef;

		WeldDef.bodyA = BodyA;
		WeldDef.bodyB = BodyB;
		WeldDef.collideConnected = CollideConnected;

		WeldDef.localAnchorA = AnchorA;
		WeldDef.localAnchorB = AnchorB;

		JointHandle = (b2WeldJoint*)Ermine::Universum->CreateJoint(&WeldDef);
	}

	WeldJoint::WeldJoint(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
		:
		JointBase(JointName,BodyA,BodyB)
	{
		//The Pointer Being Constructed Outside Makes This Method Dangerous..
		JointHandle = Pointer;
	}

	WeldJoint::~WeldJoint()
	{}

	WeldJoint::WeldJoint(WeldJoint&& rhs)
		:
		JointBase(std::move(rhs))
	{}

	Ermine::WeldJoint& WeldJoint::operator=(WeldJoint&& rhs)
	{
		JointBase::operator=(std::move(rhs));
		return *this;
	}

	std::shared_ptr<Ermine::WeldJoint> WeldJoint::Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	{
		return std::shared_ptr<Ermine::WeldJoint>(new Ermine::WeldJoint(JointName, BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre, ShouldBodiesAttachedByTheJointCollide));
	}


	std::shared_ptr<Ermine::WeldJoint> WeldJoint::Generate(b2Joint* Joint, std::string JointName, b2Body* BodyA, b2Body* BodyB)
	{
		return std::shared_ptr<Ermine::WeldJoint>(new Ermine::WeldJoint(Joint,JointName, BodyA, BodyB));
	}

	glm::vec2 WeldJoint::GetBodyALocalAnchorLocation()
	{
		if(GetHealth() == Ermine::JointHealthEnum::StatusOk)
		{
			b2Vec2 LocalAnchorLocation = ((b2WeldJoint*)(JointHandle))->GetLocalAnchorA();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Get BodyALocalAnchor Location Of The Weld Joint As The Health Is Not Okay");
			return glm::vec2(-9999.0f, -9999.0f);
		}
	}
	glm::vec2 WeldJoint::GetBodyBLocalAnchorLocation()
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		{
			b2Vec2 LocalAnchorLocation = ((b2WeldJoint*)(JointHandle))->GetLocalAnchorB();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Get BodyBLocalAnchor Location Of The Weld Joint As The Health Is Not Okay");
			return glm::vec2(-9999.0f, -9999.0f);
		}
	}
}
