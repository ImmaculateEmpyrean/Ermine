#include "stdafx.h"
#include "DistanceJoint.h"

namespace Ermine
{
	DistanceJoint::DistanceJoint(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(JointName,BodyA,BodyB)
	{
		
		b2Vec2 AnchorA = Ermine::GLMToB2Vec2(Ermine::vertexErmineToWorld(AnchorAWithRespectToBoxCentre));
		b2Vec2 AnchorB = Ermine::GLMToB2Vec2(Ermine::vertexErmineToWorld(AnchorBWithRespectToBoxCentre));

		b2DistanceJointDef DisDef;

		DisDef.bodyA = BodyA;
		DisDef.bodyB = BodyB;
		DisDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

		DisDef.localAnchorA = AnchorA;
		DisDef.localAnchorB = AnchorB;

		JointHandle = (b2DistanceJoint*)Ermine::Universum->CreateJoint(&DisDef);
	}

	DistanceJoint::~DistanceJoint()
	{}

	DistanceJoint::DistanceJoint(DistanceJoint&& rhs)
		:
		JointBase(std::move(rhs))
	{}

	DistanceJoint& DistanceJoint::operator=(DistanceJoint&& rhs)
	{
		JointBase::operator=(std::move(rhs));
		return *this;
	}

	std::shared_ptr<Ermine::DistanceJoint> DistanceJoint::Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	{
		std::shared_ptr<Ermine::DistanceJoint> Joint(new Ermine::DistanceJoint(JointName, BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre,ShouldBodiesAttachedByTheJointCollide),Ermine::JointDeleter<Ermine::DistanceJoint>());
		return Joint;
	}


	glm::vec2 DistanceJoint::GetBodyALocalAnchorLocation()
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		{
			b2Vec2 LocalAnchorLocation = ((b2DistanceJoint*)JointHandle)->GetLocalAnchorA();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Get BodyA LocalAnchor Location Of The Distance Joint IN Question As The Object Health Is Not Okay");
			return glm::vec2(-9999.0f, -9999.0f);
		}
	}
	glm::vec2 DistanceJoint::GetBodyBLocalAnchorLocation()
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		{
			b2Vec2 LocalAnchorLocation = ((b2DistanceJoint*)JointHandle)->GetLocalAnchorB();
			glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
			return LocalAnchorLocPixel;
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Get BodyB LocalAnchor Location Of The Distance Joint IN Question As The Object Health Is Not Okay");
			return glm::vec2(-9999.0f, -9999.0f);
		}
	}


	float DistanceJoint::GetLength()
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		{
			return Ermine::scalarWorldToErmine(((b2DistanceJoint*)JointHandle)->GetLength());
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Get Length Of The Distance Joint In Question As The Object Health Is Not Okay");
			return -9999.0f;
		}
	}
	void DistanceJoint::SetLength(float Length)
	{
		if(GetHealth() == Ermine::JointHealthEnum::StatusOk)
			((b2DistanceJoint*)JointHandle)->SetLength(Ermine::scalarErmineToWorld(Length));
		else STDOUTDefaultLog_Error("Cannot Set Length Of The Distance Joint In Question As The Object Health Is Not Okay");
	}


	float DistanceJoint::GetFrequency()
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
			return ((b2DistanceJoint*)JointHandle)->GetFrequency(); //Dunno What Exactly This Means.. So Passing It On As Is
		else
		{
			STDOUTDefaultLog_Error("Cannot Get Frequency Of The Distance Joint In Question As The Object Health Is Not Okay");
			return -9999.0f;
		}
	}
	void DistanceJoint::SetFrequency(float Frequency)
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
			((b2DistanceJoint*)JointHandle)->SetFrequency(Frequency); //Dunno What Exactly This Means.. So Passing It On As Is
		else STDOUTDefaultLog_Error("Cannot Set Frequency Of The Distance Joint In Question As The Object Health Is Not Okay");
	}

	float DistanceJoint::GetDampingRatio()
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
			return ((b2DistanceJoint*)JointHandle)->GetDampingRatio();
		else
		{
			STDOUTDefaultLog_Error("Cannot Get Damping Ratio Of The Distance Joint In Question As The Object Health Is Not Okay");
			return -9999.0f;
		}
	}
	void DistanceJoint::SetDampingRatio(float Ratio)
	{
		if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
			((b2DistanceJoint*)JointHandle)->SetDampingRatio(Ratio);
		else STDOUTDefaultLog_Error("Cannot Set Damping Ratio Of The Distance Joint In Question As The Object Health Is Not Okay");
	}
}