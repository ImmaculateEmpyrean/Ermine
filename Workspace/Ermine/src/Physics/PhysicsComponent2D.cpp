#include "stdafx.h"
#include "PhysicsComponent2D.h"

namespace Ermine
{

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
		Shape.SetAsBox(10.0f, 5.0f);

		//For The Created DataStructure Set The Default Values..
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].shape = &Shape;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].density = 1.0f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].friction = 0.3f;

		//Attact the Fixture To The Body..
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1]);

	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition)
		:
		BodyDefinitionOfTheComponent(Definition)
	{
		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		FixturesAssociatedWithTheBody.emplace_back(FixtureDefinition);

		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1]);
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
	/*PhysicsComponent2D::PhysicsComponent2D(const PhysicsComponent2D& rhs)
	{
		HelperCopyFunction(rhs);
	}

	PhysicsComponent2D PhysicsComponent2D::operator=(const PhysicsComponent2D& rhs)
	{
		HelperCopyFunction(rhs);
		return *this;
	}*/

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

#pragma region HelperMoveAndCopy
	/*void PhysicsComponent2D::HelperCopyFunction(const PhysicsComponent2D& rhs)
	{
		//Copy The Body Definition From The Right Handside to create the exact body..
		BodyDefinitionOfTheComponent = rhs.BodyDefinitionOfTheComponent;

		//Try To Copy The entire Lot Of Fixtures Definition From The Right Handside To This object 
		FixturesAssociatedWithTheBody = rhs.FixturesAssociatedWithTheBody;

		//The Body Can Now Be created as the definition is successfully recieved from the rhs
		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		//Associate All The Fixtures To The Body Which Has Been Created Previously
		for (auto& AssociatedFixtures : FixturesAssociatedWithTheBody)
			BodyManagedByTheComponent->CreateFixture(&AssociatedFixtures);

		//done
	}*/

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
	}
#pragma endregion HelperMoveAndCopy

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


