#include "stdafx.h"
#include "BodyPart.h"

Ermine::BodyPart::BodyPart(b2FixtureDef FixtureDefinition, b2Shape* Shape, glm::vec2 PartSize, 
						   glm::vec2 OffsetFromTheCentre, bool InBox2DSpace)
	:
	FixtureDefinition(std::move(FixtureDefinition)),
	Shape(std::move(Shape)),
	OffsetFromTheCentre(OffsetFromTheCentre),
	PartSize(PartSize),
	InBox2DSpace(InBox2DSpace)
{
	if (Shape->m_type == b2Shape::Type::e_circle)
	{
		b2CircleShape* CircleShape = (b2CircleShape*)this->Shape;
		CircleShape->m_p.Set(b2Vec2(OffsetFromTheCentre.x, OffsetFromTheCentre.y).x, b2Vec2(OffsetFromTheCentre.x, OffsetFromTheCentre.y).y);
	}
}

void Ermine::BodyPart::ConvertOffsetFromCentreAndPartSizeToBox2DSpace()
{
	if (InBox2DSpace == false)
	{
		//Convert Offset From Centre Into Box2D Space
		OffsetFromTheCentre = Ermine::coordErmineToWorld(OffsetFromTheCentre);

		//Convert Part Size Into Box2D Space.. Part Size Is Not A POINT On The Coordinate Space..
		PartSize = Ermine::vectorErmineToWorld(PartSize);
		
		//Set The Flag To True If It Was False In The First Place..
		InBox2DSpace = true;
	}
}
