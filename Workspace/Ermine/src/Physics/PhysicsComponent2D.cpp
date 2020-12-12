#include "stdafx.h"
#include "PhysicsComponent2D.h"

namespace Ermine
{
#pragma region Constructors
	
	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, std::vector<b2FixtureDef> FixtureDefinitions)
		:
		BodyDefinitionOfTheComponent(Definition),
		FixturesAssociatedWithTheBody(FixtureDefinitions)
	{
		//First Create The Body In The Box2D World As Intended..
		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		for(b2FixtureDef& FixtureDefinition : FixturesAssociatedWithTheBody)
			BodyManagedByTheComponent->CreateFixture(&FixtureDefinition);
	}

	PhysicsComponent2D::~PhysicsComponent2D()
	{
		//Donot Bother Deleting a Nullptr Right..
		if (BodyManagedByTheComponent != nullptr)
		{
			for (auto i = BodyManagedByTheComponent->GetFixtureList(); i!= nullptr;i++)
			{
				//Start Destroy All Shapes..
				if(i->GetShape() != nullptr)
					delete i->GetShape();
				//Ended Destroy All Shapes..

				if(i->GetUserData() != nullptr)
					delete i->GetUserData();
			}
			for (auto& i : JointsBuffer)
				i->DestroyJoint();

			Universum->DestroyBody(BodyManagedByTheComponent);
		}
	}
#pragma endregion

#pragma region CopyAndMoveConstruction
	PhysicsComponent2D& PhysicsComponent2D::operator=(PhysicsComponent2D&& rhs)
	{
		HelperMoveFunction(std::move(rhs));
		return *this;
	}

	std::shared_ptr<Ermine::PhysicsComponent2D> PhysicsComponent2D::Generate(b2BodyDef BodyDefinition, std::vector<b2FixtureDef> FixturesAssociatedWithBody)
	{
		std::shared_ptr<Ermine::PhysicsComponent2D> PhyComponent(new Ermine::PhysicsComponent2D(std::move(BodyDefinition), std::move(FixturesAssociatedWithBody)));
		return PhyComponent;
	}

	PhysicsComponent2D::PhysicsComponent2D(PhysicsComponent2D&& rhs)
	{
		HelperMoveFunction(std::move(rhs));
	}
#pragma endregion CopyAndMoveConstruction

#pragma region HelperFunctions
	void PhysicsComponent2D::HelperMoveFunction(PhysicsComponent2D&& rhs)
	{
		//Try Moving The Body Definition Into The New Object
		BodyDefinitionOfTheComponent = std::move(rhs.BodyDefinitionOfTheComponent);

		//The Entire Lot Of The Fixtures Is To be Tried And moved Into The New Object
		FixturesAssociatedWithTheBody = std::move(rhs.FixturesAssociatedWithTheBody);

		//Must Set All The Shapes To Null Right Or Else They Will Probably Be Wiped Out by The Destructor..
		rhs.FixturesAssociatedWithTheBody.clear();

		//Donot Create A New Body instead point to the body pointed by the rhs previously
		//the rhs must loose ownership over its body for its destructor not to delete it
		BodyManagedByTheComponent = rhs.BodyManagedByTheComponent;
		rhs.BodyManagedByTheComponent = nullptr;
	}

	glm::vec2 PhysicsComponent2D::HelperGetWidthAndHeightOfTheBoundingBox()
	{
		b2AABB aabb;
		aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX); 
		aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
		b2Transform t;
		t.SetIdentity();
		b2Fixture* fixture = BodyManagedByTheComponent->GetFixtureList();
		while (fixture != NULL) {
			const b2Shape* shape = fixture->GetShape();
			const int childCount = shape->GetChildCount();
			for (int child = 0; child < childCount; ++child) {
				b2AABB shapeAABB;
				shape->ComputeAABB(&shapeAABB, t, child);
				shapeAABB.lowerBound = shapeAABB.lowerBound;
				shapeAABB.upperBound = shapeAABB.upperBound;
				aabb.Combine(shapeAABB);
			}
			fixture = fixture->GetNext();
		}

		b2Vec2 lowerVertex = aabb.lowerBound;
		b2Vec2 heigherVertex = aabb.upperBound;
		float bodyWidth = heigherVertex.x - lowerVertex.x;
		float bodyHeight = heigherVertex.y - lowerVertex.y;

		return Ermine::vectorWorldToErmine(bodyWidth, bodyHeight);
	}

#pragma endregion HelperFunctions

	void PhysicsComponent2D::AddForce(glm::vec2 Force, glm::vec2 WorldPositionInPixelCoordinatesWhereTheForceWasApplied)
	{
		//Convert The Coordinates Of Body Point At Which Force Is Applied From Poxel Space To Box2D Space..
		WorldPositionInPixelCoordinatesWhereTheForceWasApplied = Ermine::coordErmineToWorld(WorldPositionInPixelCoordinatesWhereTheForceWasApplied);

		//Ask Box2D To Apply The Force On The Object In Question..
		BodyManagedByTheComponent->ApplyForce(b2Vec2(Force.x, Force.y), b2Vec2(WorldPositionInPixelCoordinatesWhereTheForceWasApplied.x, WorldPositionInPixelCoordinatesWhereTheForceWasApplied.y), true);
	}


	void PhysicsComponent2D::SetPosition(glm::vec2 Position)
	{
		glm::vec2 WorldCoordinates = Ermine::vectorErmineToWorld(Position);
		BodyManagedByTheComponent->SetTransform(Ermine::GLMToB2Vec2(WorldCoordinates), BodyManagedByTheComponent->GetAngle());
	}

	void PhysicsComponent2D::SetVelocity(glm::vec2 Velocity)
	{
		glm::vec2 VelocityInBox2DLand = Ermine::vectorErmineToWorld(Velocity);
		BodyManagedByTheComponent->SetLinearVelocity(Ermine::GLMToB2Vec2(VelocityInBox2DLand));
	}

	void PhysicsComponent2D::SetAngularVelocity(float AngularVelocity)
	{
		float AngularVelocityInBox2DLand = Ermine::scalarErmineToWorld(AngularVelocity);
		BodyManagedByTheComponent->SetAngularVelocity(AngularVelocityInBox2DLand);
	}


	glm::vec2 PhysicsComponent2D::GetVelocityOfTheBody()
	{
		return Ermine::vectorWorldToErmine(Ermine::B2Vec2ToGLM(BodyManagedByTheComponent->GetLinearVelocity()));
	}

	float PhysicsComponent2D::GetAngularVelocityOfTheBody()
	{
		return glm::degrees<float>(BodyManagedByTheComponent->GetAngularVelocity());
	}

	void PhysicsComponent2D::AddForceToCentre(glm::vec2 Force)
	{
		//In Pixel Space Y Points Down While In Box2D It Points Up..
		Force.y = -1.0f * Force.y;

		//Ask Body2D To Apply The Force To The Body Centre..
		BodyManagedByTheComponent->ApplyForceToCenter(b2Vec2(Force.x,Force.y), true);
	}

	std::shared_ptr<JointBase> PhysicsComponent2D::GetJoint(std::string JointIdentificationName)
	{
		for (auto& i : JointsBuffer)
		{
			if (i->GetName() == JointIdentificationName)
				return i;
		}
	}
	void PhysicsComponent2D::DestroyJoint(std::string Identifier)
	{
		for (auto& i : JointsBuffer)
		{
			if (i->GetName() == Identifier)
			{
				i->DestroyJoint();
				break;
			}
		}
	}


	//Create Distance Joint Functions
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateDistanceJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, bool CollideCollision)
	{
		return CreateDistanceJoint(JointName, BodyB, glm::vec2(0.0f,0.0f), glm::vec2(0.0f, 0.0f), CollideCollision);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateDistanceJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision)
	{
		return CreateDistanceJoint(JointName, BodyB, LocalAnchorPointA, glm::vec2(0.0f, 0.0f), CollideCollision);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateDistanceJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision)
	{
		std::shared_ptr<Ermine::DistanceJoint> DisJoint = DistanceJoint::Generate(JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA,LocalAnchorPointB, CollideCollision);

		JointsBuffer.emplace_back(DisJoint);
		BodyB->JointsBuffer.emplace_back(DisJoint);

		return DisJoint;
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateDistanceJoint(b2Joint* Pointer, std::string JointName, std::shared_ptr<Ermine::PhysicsComponent2D> BodyB)
	{
		std::shared_ptr<Ermine::DistanceJoint> DisJoint = Ermine::DistanceJoint::Generate(Pointer, JointName, this->BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent);

		JointsBuffer.emplace_back(DisJoint);
		BodyB->JointsBuffer.emplace_back(DisJoint);

		return DisJoint;
	}
	
	//Create Revolute Joint Functions
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRevoluteJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, bool CollideCollision)
	{
		return CreateRevoluteJoint(JointName, BodyB, glm::vec2(0.0f,0.0f), glm::vec2(0.0f, 0.0f), 0.0f, CollideCollision);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRevoluteJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision)
	{
		return CreateRevoluteJoint(JointName, BodyB, LocalAnchorPointA, glm::vec2(0.0f,0.0f), 0.0f, CollideCollision);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRevoluteJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision)
	{
		return CreateRevoluteJoint(JointName, BodyB, LocalAnchorPointA, LocalAnchorPointB, 0.0f,CollideCollision);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRevoluteJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInDegrees, bool CollideCollision)
	{
		std::shared_ptr<Ermine::RevoluteJoint> RevJoint = Ermine::RevoluteJoint::Generate(JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, LocalAnchorPointB, ReferenceAngleInDegrees, CollideCollision);

		JointsBuffer.emplace_back(RevJoint);
		BodyB->JointsBuffer.emplace_back(RevJoint);

		return RevJoint;
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRevoluteJoint(b2Joint* Pointer, std::string JointName, std::shared_ptr<Ermine::PhysicsComponent2D> BodyB)
	{
		std::shared_ptr<Ermine::RevoluteJoint> RevJoint = Ermine::RevoluteJoint::Generate(Pointer,JointName,this->BodyManagedByTheComponent,BodyB->BodyManagedByTheComponent);

		JointsBuffer.emplace_back(RevJoint);
		BodyB->JointsBuffer.emplace_back(RevJoint);

		return RevJoint;
	}


	//Create Prismatic Joints
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreatePrismaticJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, bool CollideConnected)
	{
		return CreatePrismaticJoint(JointName, BodyB, glm::vec2(0.0f,0.0f), glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 0.0f), CollideConnected);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreatePrismaticJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision)
	{
		return CreatePrismaticJoint(JointName, BodyB, LocalAnchorPointA, glm::vec2(0.0f,0.0f), 0.0f, glm::vec2(1.0f, 0.0f), CollideCollision);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreatePrismaticJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision)
	{
		return CreatePrismaticJoint(JointName, BodyB, LocalAnchorPointA, LocalAnchorPointB, 0.0f, glm::vec2(1.0f, 0.0f), CollideCollision);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreatePrismaticJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInDegrees, bool CollideCollision)
	{
		return CreatePrismaticJoint(JointName, BodyB, LocalAnchorPointA, LocalAnchorPointB, ReferenceAngleInDegrees, glm::vec2(1.0f, 0.0f), CollideCollision);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreatePrismaticJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInDegrees, glm::vec2 SlidingAxis, bool CollideCollision)
	{
		std::shared_ptr<Ermine::PrismaticJoint> PJ = Ermine::PrismaticJoint::Generate(JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, LocalAnchorPointB, ReferenceAngleInDegrees, SlidingAxis, CollideCollision);

		JointsBuffer.emplace_back(PJ);
		BodyB->JointsBuffer.emplace_back(PJ);

		return PJ;
	}

	//Create Wheel Joint
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWheelJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, b2WheelJointDef Def)
	{
		std::shared_ptr<Ermine::WheelJoint> WheelJoint = Ermine::WheelJoint::Generate(JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, Def);

		JointsBuffer.emplace_back(WheelJoint);
		BodyB->JointsBuffer.emplace_back(WheelJoint);

		return WheelJoint;
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWheelJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, bool CollideConnected)
	{
		return CreateWheelJoint(JointName, BodyB, glm::vec2(0.0f,0.0f), glm::vec2(0.0f,0.0f), glm::vec2(1.0f, 0.0f), CollideConnected);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWheelJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, bool CollideConnected)
	{
		return CreateWheelJoint(JointName, BodyB, LocalAnchorA, LocalAnchorB, glm::vec2(1.0f, 0.0f), CollideConnected);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWheelJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, glm::vec2 LocalTranslationalAxisInBodyA, bool CollideConnected)
	{
		std::shared_ptr<Ermine::WheelJoint> WheelJoint = Ermine::WheelJoint::Generate(JointName,BodyManagedByTheComponent,BodyB->BodyManagedByTheComponent,LocalAnchorA,LocalAnchorB,LocalTranslationalAxisInBodyA,CollideConnected);

		JointsBuffer.emplace_back(WheelJoint);
		BodyB->JointsBuffer.emplace_back(WheelJoint);

		return WheelJoint;
	}

	//Create Rope Joint..
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRopeJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, bool CollideConnected)
	{
		return CreateRopeJoint(JointName, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 10.0f, CollideConnected);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRopeJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, float RopeLength, bool CollideConnected)
	{
		return CreateRopeJoint(JointName, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), RopeLength, CollideConnected);
	}
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRopeJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, float RopeLength, bool CollideConnected)
	{
		std::shared_ptr<Ermine::RopeJoint> RJ = Ermine::RopeJoint::Generate(JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorA, LocalAnchorB, RopeLength, CollideConnected);
		JointsBuffer.emplace_back(RJ);
		BodyB->JointsBuffer.emplace_back(RJ);

		return RJ;
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateRopeJoint(b2Joint* Pointer, std::string JointName, std::shared_ptr<Ermine::PhysicsComponent2D> BodyB)
	{
		std::shared_ptr<Ermine::RopeJoint> RJ = Ermine::RopeJoint::Generate(Pointer,JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent);
		JointsBuffer.emplace_back(RJ);
		BodyB->JointsBuffer.emplace_back(RJ);

		return RJ;
	}
	
	//Create Weld Joint..
	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWeldJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, bool CollideConnected)
	{
		return CreateWeldJoint(JointName, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), CollideConnected);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWeldJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorA, bool CollideConnected)
	{
		return CreateWeldJoint(JointName, BodyB, LocalAnchorA, glm::vec2(0.0f, 0.0f), CollideConnected);
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWeldJoint(std::string JointName, std::shared_ptr<PhysicsComponent2D> BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, bool CollideConnected)
	{
		std::shared_ptr<Ermine::WeldJoint> WeldJoint = Ermine::WeldJoint::Generate(JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorA, LocalAnchorB, CollideConnected);

		JointsBuffer.emplace_back(WeldJoint);
		BodyB->JointsBuffer.emplace_back(WeldJoint);

		return WeldJoint;
	}

	std::shared_ptr<Ermine::JointBase> PhysicsComponent2D::CreateWeldJoint(b2Joint* Pointer, std::string JointName, std::shared_ptr<Ermine::PhysicsComponent2D> BodyB)
	{
		std::shared_ptr<Ermine::WeldJoint> WeldJoint = Ermine::WeldJoint::Generate(Pointer,JointName, BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent);

		JointsBuffer.emplace_back(WeldJoint);
		BodyB->JointsBuffer.emplace_back(WeldJoint);

		return WeldJoint;
	}


#pragma region GetBodyTransform

	glm::vec2 PhysicsComponent2D::GetBodyLocationBox2DSpace()
	{
		//This Is To Be Returned Back to The User..
		glm::vec2 Location;

		//Ask Box2D Where The Body Is Currently..
		auto Loc = ((b2Body*)BodyManagedByTheComponent)->GetPosition();
		
		//Convert From That Propriety Type Into Type used By The Engine As Staple
		Location.x = Loc.x;
		Location.y = Loc.y;

		//Return The Result To The User..
		return Location;
	}

	glm::vec2 PhysicsComponent2D::GetBodyLocationPixelSpace()
	{
		//First Find Out The Position Of The Body in The Box2D World..
		glm::vec2 LocationRudimentary = GetBodyLocationBox2DSpace();

		//Now Convert From Box2DCoordinates Into Ermine Coordinates..
		glm::vec2 Location = coordWorldToErmine(LocationRudimentary);
		
		//Send The Result Back To The User..
		return Location;
	}

	float PhysicsComponent2D::GetAngleOfTheBodyRadians()
	{
		return BodyManagedByTheComponent->GetAngle() * -1.0f;
	}

	float PhysicsComponent2D::GetAnfleOfTheBodyDegrees()
	{
		return glm::degrees<float>((BodyManagedByTheComponent->GetAngle() * -1.0f));
	}

	glm::vec2 PhysicsComponent2D::GetBoundingBoxDiamensions()
	{
		return HelperGetWidthAndHeightOfTheBoundingBox();
	}

	float PhysicsComponent2D::GetBoundingBoxWidth()
	{
		return HelperGetWidthAndHeightOfTheBoundingBox().x;
	}

	float PhysicsComponent2D::GetBoundingBoxHeight()
	{
		return HelperGetWidthAndHeightOfTheBoundingBox().y;
	}
	


	glm::mat4 PhysicsComponent2D::GetModelMatrix()
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(GetBodyLocationPixelSpace(), 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, GetAngleOfTheBodyRadians(), glm::vec3(0.0f, 0.0f, 1.0f));
		//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f,1.0f, 1.0f)); //Since Box2D Only Works With Rigid Bodies.. Elasticity Cannot Be Calculated Using Ermine Ever..

		return ModelMatrix;
	}

	glm::mat4 PhysicsComponent2D::GetTranslationMatrix()
	{
		//Create An Empty Matrix.. 
		glm::mat4 TranslationMatrix = glm::mat4(1.0f);

		//Ask Box2D For Location Of The Body..And Also Convert It into ermine space..
		glm::vec2 BodyLocation = GetBodyLocationPixelSpace();

		//glm::translate requires glm::vec3 to be inputted hence convert vec2 into vec3
		glm::vec3 BodyLocation3 = glm::vec3(BodyLocation, 1.0f);

		//Ask GLM To calculate the Translation Matrix..
		TranslationMatrix = glm::translate(TranslationMatrix, BodyLocation3);

		//Return Translated Matrix Back To The User..
		return TranslationMatrix;
	}

	glm::mat4 PhysicsComponent2D::GetRotationMatrix()
	{
		//Create An Empty Matrix.. 
		glm::mat4 RotationMatrix = glm::mat4(1.0f);

		//Ask Box2D For The Rotation Data..
		float RotationAngle = GetAngleOfTheBodyRadians();

		//The Negative Is Already Done In The GetAngleOfTheBody..
		//RotationAngle = RotationAngle * -1.0f;

		//Ask GLm To rotate using glm::rotate function..
		RotationMatrix = glm::rotate(RotationMatrix, RotationAngle, glm::vec3(0.0, 0.0, 1.0));

		//Return The Rotation MAtrix To The User..
		return RotationMatrix;
	}

#pragma endregion GetBodyTransform
}


