#include "stdafx.h"
#include "PhysicsComponent2D.h"

namespace Ermine
{
	//Start Definition Of Static Variables..//
	std::function<void(PhysicsComponent2D*)> Ermine::PhysicsComponent2D::FuncSubmitBodyToRenderer2D;
	std::function<void(PhysicsComponent2D*)> Ermine::PhysicsComponent2D::FuncDetachBodyFromRenderer2D;
	//Ended Definition Of Static Variables..//

#pragma region Constructors
	//The Default Constructor.. Not The Best Constructor In The World But it May be Useful For Debugging
	PhysicsComponent2D::PhysicsComponent2D()
	{
		BodyDefinitionOfTheComponent.position.Set(0.0f,0.0f);
		BodyDefinitionOfTheComponent.type = b2_dynamicBody;

		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		//Create A Physics Component Shape DataStructure Inside The Buffer Array
		FixturesAssociatedWithTheBody.emplace_back(b2FixtureDef());

		//Create A Shape To Be associated With The Fixture..
		b2PolygonShape Shape = b2PolygonShape();
		Shape.SetAsBox(BodySize.x,BodySize.y);

		//For The Created DataStructure Set The Default Values..
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].shape = &Shape;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].density = 1.0f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].friction = 0.3f;

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].userData = new FixtureUserDataStruct();
#endif 
		
		//Attact the Fixture To The Body..
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1]);

	}

	PhysicsComponent2D::PhysicsComponent2D(glm::vec2 BodyLocationInPixelSpace,glm::vec2 BodySizeInPixelSpace ,bool StaticBody)
	{
		HelperConstructBox(BodyLocationInPixelSpace, BodySizeInPixelSpace, StaticBody);
	}

	PhysicsComponent2D::PhysicsComponent2D(glm::vec2 BodyLocationInPixelSpace, glm::vec2 BodySizeInPixelSpace, bool StaticBody, glm::vec4 Color)
	{
		HelperConstructBox(BodyLocationInPixelSpace, BodySizeInPixelSpace, StaticBody);
		UseCustomColorsOnDebugTrace = true;
		CustomDebugTraceColor = Color;
	}

	PhysicsComponent2D::PhysicsComponent2D(glm::vec2 BodyLocationInPixelSpace, float BodyRadiusInPixelSpace, bool StaticBody)
	{
		HelperConstructCircle(BodyLocationInPixelSpace, BodyRadiusInPixelSpace, StaticBody);
	}

	PhysicsComponent2D::PhysicsComponent2D(glm::vec2 BodyLocationInPixelSpace, float BodyRadiusInPixelSpace, bool StaticBody, glm::vec4 Color)
	{
		HelperConstructCircle(BodyLocationInPixelSpace, BodyRadiusInPixelSpace, StaticBody);
		UseCustomColorsOnDebugTrace = true;
		CustomDebugTraceColor = Color;
	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition,
										   glm::vec2 BoxSizeInPixels)
		
	{
		HelperConstructBox(Definition, FixtureDefinition, BoxSizeInPixels);
	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition, 
										   glm::vec2 BodySizeInPixelSpace, glm::vec4 Color)
	{
		HelperConstructBox(Definition, FixtureDefinition, BodySizeInPixelSpace); //The Helper Is Trusted To Properly Construct The Object..
		UseCustomColorsOnDebugTrace = true;
		CustomDebugTraceColor = Color;
	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition)
		:
		BodyDefinitionOfTheComponent(Definition)
	{
		//Get The Fixture Associated With The Body And Place It inside The Buffer 
		FixturesAssociatedWithTheBody.emplace_back(FixtureDefinition);

		//Construct The Body Using The Fixtures AlreadySetup
		HelperConstructorConstructBody();

		//Calculate Size Of The Body In Box2D Space..
		BodySize = HelperGetWidthAndHeightOfTheBoundingBox();
	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, std::vector<b2FixtureDef> FixtureDefinitions)
		:
		BodyDefinitionOfTheComponent(Definition),
		FixturesAssociatedWithTheBody(FixtureDefinitions)
	{
		//Construct The Body Using The Fixtures AlreadySetup
		HelperConstructorConstructBody();

		//Calculate Size Of The Body In Box2D Space..
		BodySize = HelperGetWidthAndHeightOfTheBoundingBox();
	}

	PhysicsComponent2D::~PhysicsComponent2D()
	{
		//First Try Deleting Invalid Caskets
		for (auto& casket : JointsBuffer)
		{
			if (casket.second != nullptr)
			{
				if (casket.second->ValidFlag == false)
				{
					delete casket.second;
					casket.second = nullptr;
				}
			}
		} 

		//Set Caskets As Invalid Which Are In Infact Invalid..
		for (auto& casket : JointsBuffer)
		{
			if (casket.second != nullptr)
			{
				casket.second->ValidFlag = false;
			}
		}

		JointsBuffer.clear();

		//Donot Bother Deleting a Nullptr Right..
		if (BodyManagedByTheComponent != nullptr)
		{
			for (auto i = BodyManagedByTheComponent->GetFixtureList(); i!= nullptr;i++)
			{
				if(i->GetUserData() != nullptr)
					delete i->GetUserData();
			}

			Universum->DestroyBody(BodyManagedByTheComponent);
		}
	}
#pragma endregion Constructors

#pragma region CopyAndMoveConstruction
	PhysicsComponent2D& PhysicsComponent2D::operator=(PhysicsComponent2D&& rhs)
	{
		HelperMoveFunction(std::move(rhs));
		return *this;
	}

	PhysicsComponent2D::PhysicsComponent2D(PhysicsComponent2D&& rhs)
	{
		HelperMoveFunction(std::move(rhs));
	}
#pragma endregion CopyAndMoveConstruction

#pragma region HelperFunctions
	
	void PhysicsComponent2D::HelperConstructorConstructBody()
	{
		//First Create The Body In The Box2D World As Intended..
		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		for (auto FixtureDefinition : FixturesAssociatedWithTheBody)
		{
#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
			if (FixtureDefinition.userData == nullptr)
			{
				FixtureDefinition.userData = new FixtureUserDataStruct();
			}
#endif
			BodyManagedByTheComponent->CreateFixture(&FixtureDefinition);
		}
		
	}
	
	
	void PhysicsComponent2D::HelperMoveFunction(PhysicsComponent2D&& rhs)
	{
		if (rhs.IsDebugTraceEnabled)
			rhs.FuncDetachBodyFromRenderer2D(&rhs);

		//Try Moving The Body Definition Into The New Object
		BodyDefinitionOfTheComponent = std::move(rhs.BodyDefinitionOfTheComponent);

		//The Entire Lot Of The Fixtures Is To be Tried And moved Into The New Object
		FixturesAssociatedWithTheBody = std::move(rhs.FixturesAssociatedWithTheBody);

		//Donot Create A New Body instead point to the body pointed by the rhs previously
		//the rhs must loose ownership over its body for its destructor not to delete it
		BodyManagedByTheComponent = rhs.BodyManagedByTheComponent;
		rhs.BodyManagedByTheComponent = nullptr;

		//Get The Size Of The Body from the Right Side
		BodySize = rhs.BodySize;

		//This Handle Is Stored Inside The Physics Component Class So That We Can Submit The Body To The Renderer Easily..
		FuncSubmitBodyToRenderer2D = rhs.FuncSubmitBodyToRenderer2D;

		//This Handle Is Stored Inside The Physics Component Class So That We Can Detach The Body From The Renderer Easily..
		FuncDetachBodyFromRenderer2D = rhs.FuncDetachBodyFromRenderer2D;

		IsDebugTraceEnabled = rhs.IsDebugTraceEnabled;

		if (IsDebugTraceEnabled)
			FuncSubmitBodyToRenderer2D(this); //Submit Yourself To DebugDraw..

		UseCustomColorsOnDebugTrace = rhs.UseCustomColorsOnDebugTrace;
		CustomDebugTraceColor = rhs.CustomDebugTraceColor;
	}

	void PhysicsComponent2D::HelperConstructBox(b2BodyDef Definition, b2FixtureDef FixtureDefinition, glm::vec2 BodySizeInPixelSpace)
	{
		BodyDefinitionOfTheComponent = Definition;
		//Calculate Size Of The Body In Box2D Space..
		BodySize = Ermine::vectorPixelsToWorld(BodySizeInPixelSpace);

		//Create A Shape To Be associated With The Fixture..
		b2PolygonShape Shape = b2PolygonShape();
		Shape.SetAsBox(BodySize.x / 2.0f, BodySize.y / 2.0f);

		FixturesAssociatedWithTheBody.emplace_back(FixtureDefinition);
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].shape = &Shape;

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].userData = new FixtureUserDataStruct(); //Made A Shared Pointer In Hopes That It Will Delete Itself When The Time Comes.. 
#endif

		HelperConstructorConstructBody();
	}

	void PhysicsComponent2D::HelperConstructBox(glm::vec2 BodyLocationInPixelSpace, glm::vec2 BodySizeInPixelSpace, bool StaticBody)
	{
		glm::vec2 LocationInWorldSpace = Ermine::coordPixelsToWorld(BodyLocationInPixelSpace);
		BodyDefinitionOfTheComponent.position.Set(LocationInWorldSpace.x, LocationInWorldSpace.y);

		glm::vec2 BodySizeInWorldSpace = Ermine::vectorPixelsToWorld(BodySizeInPixelSpace);
		BodySize.x = BodySizeInWorldSpace.x;
		BodySize.y = BodySizeInWorldSpace.y;

		if (StaticBody)
			BodyDefinitionOfTheComponent.type = b2_staticBody;
		else
			BodyDefinitionOfTheComponent.type = b2_dynamicBody;

		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		//Create A Physics Component Shape DataStructure Inside The Buffer Array
		FixturesAssociatedWithTheBody.emplace_back(b2FixtureDef());

		//Create A Shape To Be associated With The Fixture..
		b2PolygonShape Shape = b2PolygonShape();
		Shape.SetAsBox(BodySize.x / 2.0f, BodySize.y / 2.0f);

		//For The Created DataStructure Set The Default Values..
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].shape = &Shape;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].density = 1.0f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].friction = 0.3f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].restitution = 0.5f;

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].userData = new FixtureUserDataStruct();
#endif 

		//Attach the Fixture To The Body..
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1]);
	}

	void PhysicsComponent2D::HelperConstructCircle(glm::vec2 BodyLocationInPixelSpace, float BodyRadiusInPixelSpace, bool StaticBody)
	{
		glm::vec2 LocationInWorldSpace = Ermine::coordPixelsToWorld(BodyLocationInPixelSpace);
		BodyDefinitionOfTheComponent.position.Set(LocationInWorldSpace.x, LocationInWorldSpace.y);

		float BodyRadiusInWorldSpace = Ermine::scalarPixelsToWorld(BodyRadiusInPixelSpace);
		BodySize.x = BodyRadiusInWorldSpace * 2.0f;
		BodySize.y = BodyRadiusInWorldSpace * 2.0f;

		if (StaticBody)
			BodyDefinitionOfTheComponent.type = b2_staticBody;
		else
			BodyDefinitionOfTheComponent.type = b2_dynamicBody;

		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		//Create A Physics Component Shape DataStructure Inside The Buffer Array
		FixturesAssociatedWithTheBody.emplace_back(b2FixtureDef());

		//Create A Shape To Be associated With The Fixture..
		b2CircleShape Shape = b2CircleShape();
		Shape.m_radius = BodyRadiusInWorldSpace;

		//For The Created DataStructure Set The Default Values..
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].shape = &Shape;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].density = 1.0f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].friction = 0.3f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].restitution = 0.5f;

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].userData = new FixtureUserDataStruct(); //Made A Shared Pointer In Hopes That It Will Delete Itself When The Time Comes.. 
#endif 

		//Attach the Fixture To The Body..
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1]);
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

		return glm::vec2(bodyWidth, bodyHeight);
	}

#pragma endregion HelperFunctions

	void PhysicsComponent2D::AddForce(glm::vec2 Force, glm::vec2 WorldPositionInPixelCoordinatesWhereTheForceWasApplied)
	{
		//In Pixel Space Y Points Down While In Box2D It Points Up..
		//Force.y = -1.0f * Force.y;

		//Convert The Coordinates Of Body Point At Which Force Is Applied From Poxel Space To Box2D Space..
		WorldPositionInPixelCoordinatesWhereTheForceWasApplied = Ermine::coordPixelsToWorld(WorldPositionInPixelCoordinatesWhereTheForceWasApplied);

		//Ask Box2D To Apply The Force On The Object In Question..
		BodyManagedByTheComponent->ApplyForce(b2Vec2(Force.x, Force.y), b2Vec2(WorldPositionInPixelCoordinatesWhereTheForceWasApplied.x, WorldPositionInPixelCoordinatesWhereTheForceWasApplied.y), true);
	}


	void PhysicsComponent2D::SetPosition(glm::vec2 Position)
	{
		glm::vec2 WorldCoordinates = Ermine::vectorPixelsToWorld(Position);
		BodyManagedByTheComponent->SetTransform(Ermine::GLMToB2Vec2(WorldCoordinates), BodyManagedByTheComponent->GetAngle());
	}

	void PhysicsComponent2D::SetVelocity(glm::vec2 Velocity)
	{
		glm::vec2 VelocityInBox2DLand = Ermine::vectorPixelsToWorld(Velocity);
		BodyManagedByTheComponent->SetLinearVelocity(Ermine::GLMToB2Vec2(VelocityInBox2DLand));
	}

	void PhysicsComponent2D::SetAngularVelocity(float AngularVelocity)
	{
		float AngularVelocityInBox2DLand = Ermine::scalarPixelsToWorld(AngularVelocity);
		BodyManagedByTheComponent->SetAngularVelocity(AngularVelocityInBox2DLand);
	}


	glm::vec2 PhysicsComponent2D::GetVelocityOfTheBody()
	{
		return Ermine::vectorWorldToPixels(Ermine::B2Vec2ToGLM(BodyManagedByTheComponent->GetLinearVelocity()));
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


	void PhysicsComponent2D::StartDebugTrace()
	{
		if (IsDebugTraceEnabled == false)
		{
			//Submit Yourself To The Renderer..
			FuncSubmitBodyToRenderer2D(this);

			//Dont Submit More Than One Copy Of The Body..
			IsDebugTraceEnabled = true;
		}
		
	}

	void Ermine::PhysicsComponent2D::StopDebugTrace()
	{
		if (IsDebugTraceEnabled == true)
		{
			//Detach Yourself From The Renderer..
			FuncDetachBodyFromRenderer2D(this);

			//Dont Try To Remove The Body Which Was Not Even Submitted Right?
			IsDebugTraceEnabled = false;
		}
	}


	JointBase* PhysicsComponent2D::GetJoint(unsigned int Identifier)
	{
		auto& casket = JointsBuffer.find(Identifier);

		if (casket != JointsBuffer.end())
		{
			if (casket->second != nullptr)
			{
				return casket->second;
			}
		}
	}
	void PhysicsComponent2D::DeleteJoint(unsigned int Identifier)
	{
		auto& casket = JointsBuffer.find(Identifier);

		if (casket != JointsBuffer.end())
		{
			if (casket->second != nullptr)
			{
				delete casket->second;
				casket->second = nullptr;
			}
			JointsBuffer.erase(casket->first);
		}
	}

	//Create Distance Joint Functions
	JointBase* PhysicsComponent2D::CreateDistanceJoint(PhysicsComponent2D* BodyB, bool CollideCollision)
	{
		Ermine::DistanceJoint* Joint = new Ermine::DistanceJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(),Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateDistanceJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision)
	{
		Ermine::DistanceJoint* Joint = new Ermine::DistanceJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateDistanceJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision)
	{
		Ermine::DistanceJoint* Joint = new Ermine::DistanceJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, LocalAnchorPointB, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}

	//Create Revolute Joint Functions
	JointBase* PhysicsComponent2D::CreateRevoluteJoint(PhysicsComponent2D* BodyB, bool CollideCollision)
	{
		Ermine::RevoluteJoint* Joint = new Ermine::RevoluteJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent,CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateRevoluteJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision)
	{
		Ermine::RevoluteJoint* Joint = new Ermine::RevoluteJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateRevoluteJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision)
	{
		Ermine::RevoluteJoint* Joint = new Ermine::RevoluteJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA,LocalAnchorPointB, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateRevoluteJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInRadians, bool CollideCollision)
	{
		Ermine::RevoluteJoint* Joint = new Ermine::RevoluteJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, LocalAnchorPointB,ReferenceAngleInRadians, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}

	JointBase* PhysicsComponent2D::CreatePrismaticJoint(PhysicsComponent2D* BodyB, bool CollideCollision)
	{
		Ermine::PrismaticJoint* Joint = new Ermine::PrismaticJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreatePrismaticJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision)
	{
		Ermine::PrismaticJoint* Joint = new Ermine::PrismaticJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent,LocalAnchorPointA, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreatePrismaticJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision)
	{
		Ermine::PrismaticJoint* Joint = new Ermine::PrismaticJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA,LocalAnchorPointB, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreatePrismaticJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInRadians, bool CollideCollision)
	{
		Ermine::PrismaticJoint* Joint = new Ermine::PrismaticJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, LocalAnchorPointB,ReferenceAngleInRadians, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreatePrismaticJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInRadians, glm::vec2 SlidingAxis, bool CollideCollision)
	{
		Ermine::PrismaticJoint* Joint = new Ermine::PrismaticJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorPointA, LocalAnchorPointB, ReferenceAngleInRadians,SlidingAxis, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}

	JointBase* PhysicsComponent2D::CreateWheelJoint(PhysicsComponent2D* BodyB, bool CollideCollision)
	{
		Ermine::WheelJoint* Joint = new Ermine::WheelJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}

	JointBase* PhysicsComponent2D::CreateRopeJoint(PhysicsComponent2D* BodyB, bool CollideCollision)
	{
		Ermine::RopeJoint* Joint = new Ermine::RopeJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, CollideCollision);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateRopeJoint(PhysicsComponent2D* BodyB, float RopeLength, bool CollideConnected)
	{
		Ermine::RopeJoint* Joint = new Ermine::RopeJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent,RopeLength, CollideConnected);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateRopeJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLength, bool CollideConnected)
	{
		Ermine::RopeJoint* Joint = new Ermine::RopeJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent,LocalAnchorAPixelCoordinates,LocalAnchorBPixelCoordinates, RopeLength, CollideConnected);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}

	JointBase* PhysicsComponent2D::CreateWeldJoint(PhysicsComponent2D* BodyB, bool CollideConnected)
	{
		Ermine::WeldJoint* Joint = new Ermine::WeldJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, CollideConnected);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateWeldJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, bool CollideConnected)
	{
		Ermine::WeldJoint* Joint = new Ermine::WeldJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent,LocalAnchorAPixelCoordinates, CollideConnected);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;
	}
	JointBase* PhysicsComponent2D::CreateWeldJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, bool CollideConnected)
	{
		Ermine::WeldJoint* Joint = new Ermine::WeldJoint(BodyManagedByTheComponent, BodyB->BodyManagedByTheComponent, LocalAnchorAPixelCoordinates, LocalAnchorBPixelCoordinates ,CollideConnected);

		BodyB->JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);
		JointsBuffer.emplace(Joint->GetUniqueIdentifier(), Joint);

		return Joint;

	}

	void PhysicsComponent2D::SetDebugColorToBody(glm::vec4 Color)
	{
		UseCustomColorsOnDebugTrace = true;
		CustomDebugTraceColor = Color;
	}

	void PhysicsComponent2D::SetDebugColorToFixture(b2Fixture* Fixture, glm::vec4 Color)
	{
		if (Fixture->GetUserData() != nullptr)
		{
			FixtureUserDataStruct* Struct = (FixtureUserDataStruct*)Fixture->GetUserData();
			Struct->DebugTraceColor.UseColor = true;
			Struct->DebugTraceColor.Color = Color;
		}
		else
		{
			FixtureUserDataStruct *Struct = new FixtureUserDataStruct();
			Struct->DebugTraceColor.UseColor = true;
			Struct->DebugTraceColor.Color = Color;

			Fixture->SetUserData(Struct);
		}
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
		glm::vec2 Location = coordWorldToPixels(LocationRudimentary);
		
		//Send The Result Back To The User..
		return Location;
	}


	float PhysicsComponent2D::GetAngleOfTheBody()
	{
		//Directly Ask Box2D For The Angle to Send Back to the User..
		//This May Have To Be Inverted to Take Y Axis Flip Into Account Just Check Abt It In The Future.. //Done The Inversion.. Not Tested Though.. :<
		return BodyManagedByTheComponent->GetAngle(); //*-1.0f;;
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
		float RotationAngle = GetAngleOfTheBody();

		RotationAngle = RotationAngle * -1.0f;

		//Ask GLm To rotate using glm::rotate function..
		RotationMatrix = glm::rotate(RotationMatrix, RotationAngle, glm::vec3(0.0, 0.0, 1.0));

		//Return The Rotation MAtrix To The User..
		return RotationMatrix;
	}

#pragma endregion GetBodyTransform
}


