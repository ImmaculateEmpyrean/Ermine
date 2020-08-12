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
		FixturesAssociatedWithTheBody.emplace_back(PhysicsComponentShape());

		//For The Created DataStructure Set The Default Values..
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Shape.SetAsBox(10.0f, 5.0f);
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Fixture.shape = &FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Shape;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Fixture.density = 1.0f;
		FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Fixture.friction = 0.3f;

		//Attact the Fixture To The Body..
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Fixture);

	}

	PhysicsComponent2D::PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition)
		:
		BodyDefinitionOfTheComponent(Definition)
	{
		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		FixturesAssociatedWithTheBody.emplace_back(PhysicsComponentShape());
		BodyManagedByTheComponent->CreateFixture(&FixturesAssociatedWithTheBody[FixturesAssociatedWithTheBody.size() - 1].Fixture);
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
	PhysicsComponent2D::PhysicsComponent2D(const PhysicsComponent2D& rhs)
	{
		HelperCopyFunction(rhs);
	}

	PhysicsComponent2D PhysicsComponent2D::operator=(const PhysicsComponent2D& rhs)
	{
		HelperCopyFunction(rhs);
		return *this;
	}

	PhysicsComponent2D PhysicsComponent2D::operator=(PhysicsComponent2D&& rhs)
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
	void PhysicsComponent2D::HelperCopyFunction(const PhysicsComponent2D& rhs)
	{
		//Copy The Body Definition From The Right Handside to create the exact body..
		BodyDefinitionOfTheComponent = rhs.BodyDefinitionOfTheComponent;

		//Try To Copy The entire Lot Of Fixtures Definition From The Right Handside To This object 
		FixturesAssociatedWithTheBody = rhs.FixturesAssociatedWithTheBody;

		//Associate Shapes With Fixtures Properly..
		for (auto& i : FixturesAssociatedWithTheBody)
			i.Fixture.shape = &i.Shape;

		//The Body Can Now Be created as the definition is successfully recieved from the rhs
		BodyManagedByTheComponent = Universum->CreateBody(&BodyDefinitionOfTheComponent);

		//Associate All The Fixtures To The Body Which Has Been Created Previously
		for (auto Structure : FixturesAssociatedWithTheBody)
			BodyManagedByTheComponent->CreateFixture(&Structure.Fixture);

		//done
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
	}
#pragma endregion HelperMoveAndCopy
}


