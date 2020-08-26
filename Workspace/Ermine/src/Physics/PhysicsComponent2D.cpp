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
		BodyDefinitionOfTheComponent.position.Set(20.0f,20.0f);
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

		//Attact the Fixture To The Body..
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1]);

	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition,
										   glm::vec2 BoxSizeInPixels)
		:
		BodyDefinitionOfTheComponent(Definition)
	{
		//Calculate Size Of The Body In Box2D Space..
		BodySize = Ermine::vectorPixelsToWorld(BoxSizeInPixels);

		//Create A Shape To Be associated With The Fixture..
		b2PolygonShape Shape = b2PolygonShape();
		Shape.SetAsBox(BodySize.x/2.0f, BodySize.y /2.0f);

		FixturesAssociatedWithTheBody.emplace_back(FixtureDefinition);
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].shape = &Shape;

		HelperConstructorConstructBody();
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
		//Donot Bother Deleting a Nullptr Right..
		if (BodyManagedByTheComponent != nullptr)
		{
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

		for(auto FixtureDefinition: FixturesAssociatedWithTheBody)
		BodyManagedByTheComponent->CreateFixture(&FixtureDefinition);
	}
	
	
	void PhysicsComponent2D::HelperMoveFunction(PhysicsComponent2D&& rhs)
	{
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

	void PhysicsComponent2D::AddForce(glm::vec2 Force, glm::vec2 BodyPointAtWhichForceIsApplied)
	{
		//In Pixel Space Y Points Down While In Box2D It Points Up..
		Force.y = -1.0f * Force.y;

		//Convert The Coordinates Of Body Point At Which Force Is Applied From Poxel Space To Box2D Space..
		BodyPointAtWhichForceIsApplied = Ermine::coordPixelsToWorld(BodyPointAtWhichForceIsApplied);

		//Ask Box2D To Apply The Force On The Object In Question..
		BodyManagedByTheComponent->ApplyForce(b2Vec2(Force.x, Force.y), b2Vec2(BodyPointAtWhichForceIsApplied.x, BodyPointAtWhichForceIsApplied.y), true);
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
			//Submit Yourself To The Renderer..
			FuncDetachBodyFromRenderer2D(this);

			//Dont Try To Remove The Body Which Was Not Even Submitted Right?
			IsDebugTraceEnabled = false;
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
		//This May Have To Be Inverted to Take Y Axis Flip Into Account Just Check Abt It In The Future..
		return BodyManagedByTheComponent->GetAngle();
	}


	glm::mat4 PhysicsComponent2D::GetTranslationMatrix()
	{
		//Create An Empty Matrix.. 
		glm::mat4 TranslationMatrix = glm::mat4(1.0f);

		//Ask Box2D For Location Of The Body..And Also Convert It into ermine space..
		glm::vec2 BodyLocation = GetBodyLocationPixelSpace();

		//glm::translate requires glm::vec3 to be inputted hence convert vec2 into vec3
		glm::vec3 BodyLocation3 = glm::vec3(BodyLocation, 0.0f);

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

		//Ask GLm To rotate using glm::rotate function..
		RotationMatrix = glm::rotate(RotationMatrix, RotationAngle, glm::vec3(0.0, 0.0, 1.0));

		//Return The Rotation MAtrix To The User..
		return RotationMatrix;
	}

#pragma endregion GetBodyTransform
}


