#pragma region PhysicsComponentTest

	//Start Creating Box Physics Component//
	glm::vec2 LocPixelCoordinates = { 500.0f,0.0f };//{ Ermine::GetScreenWidth()/2,Ermine::GetScreenHeight()/2};
	glm::vec2 Loc = Ermine::coordPixelsToWorld(LocPixelCoordinates);

	b2BodyDef Def;
	Def.position.Set(Loc.x,Loc.y);
	Def.type = b2_dynamicBody;

	b2FixtureDef FDef;
	FDef.density = 1.0f;
	FDef.friction = 0.3f;

	static b2CircleShape CircleShape;
	CircleShape.m_radius = 0.5f;
	static b2Shape* Shp = &CircleShape;

	FDef.shape = Shp;

	//Start Creating Fixtures For The Complex Body..
	std::vector<b2FixtureDef> Fixtures;
	
	//1
	b2FixtureDef CentralBoxFixture;
	CentralBoxFixture.density = 1.0f;
	CentralBoxFixture.friction = 0.3f;

	glm::vec2 BoxShapeWidthAndHeight = Ermine::vectorPixelsToWorld(glm::vec2(100.0f, 100.0f));
	b2PolygonShape BoxShape;
	BoxShape.SetAsBox(BoxShapeWidthAndHeight.x/2, BoxShapeWidthAndHeight.y/2);

	CentralBoxFixture.shape = &BoxShape;

	Fixtures.emplace_back(CentralBoxFixture);

	//2
	b2FixtureDef BottomCircleFixture;
	BottomCircleFixture.density = 1.0f;
	BottomCircleFixture.friction = 0.3f;
	
	b2CircleShape CircleShapeBottom;
	CircleShapeBottom.m_radius = 0.5f;
	CircleShapeBottom.m_p = b2Vec2(0, -1 * Ermine::vectorPixelsToWorld(glm::vec2(100.0f, 100.0f)).y);

	BottomCircleFixture.shape = &CircleShapeBottom;

	Fixtures.emplace_back(BottomCircleFixture);

	//3
	b2FixtureDef RightCircleFixture;
	RightCircleFixture.density = 1.0f;
	RightCircleFixture.friction = 0.3f;

	b2CircleShape CircleShapeRight;
	CircleShapeRight.m_radius = 0.5f;
	CircleShapeRight.m_p = b2Vec2(Ermine::vectorPixelsToWorld(glm::vec2(100.0f, 100.0f)).y, 0.0f);

	RightCircleFixture.shape = &CircleShapeRight;

	Fixtures.emplace_back(RightCircleFixture);
	//Ended Creating Fixtures For The Complex Bodies..*/

	static PhysicsComponent2D Obj(Def,Fixtures);

	//Start Create Chain Shape IN Box2D//

	/*glm::vec2 ChainLocPixelCoordinates = { 0.0f,500.0f };//{ Ermine::GetScreenWidth()/2,Ermine::GetScreenHeight()/2};
	glm::vec2 ChainLoc = Ermine::coordPixelsToWorld(ChainLocPixelCoordinates);

	b2BodyDef ChainBodyDef;
	ChainBodyDef.position.Set(ChainLoc.x, ChainLoc.y);
	ChainBodyDef.type = b2_staticBody;

	std::vector<b2Vec2> ChainVertexes;

	glm::vec2 P1 = Ermine::vertexPixelsToWorld(glm::vec2(0.0f,0.0f));
	ChainVertexes.emplace_back(b2Vec2(P1.x,P1.y));

	glm::vec2 P2 = Ermine::vertexPixelsToWorld(glm::vec2(500.0f,100.0f));
	ChainVertexes.emplace_back(b2Vec2(P2.x,P2.y));

	glm::vec2 P3 = Ermine::vertexPixelsToWorld(glm::vec2(1000.0f,200.0f));
	ChainVertexes.emplace_back(b2Vec2(P3.x,P3.y));

	b2ChainShape* ChShp = new b2ChainShape();

	ChShp->CreateChain(&ChainVertexes[0], ChainVertexes.size());

	b2FixtureDef ChainShapeFixtureDef;
	ChainShapeFixtureDef.density = 1.0f;
	ChainShapeFixtureDef.friction = 0.3f;

	ChainShapeFixtureDef.shape = ChShp;

	static PhysicsComponent2D ChainShapeObj(ChainBodyDef, ChainShapeFixtureDef);
	
	delete ChShp;*/

	//Ended Create Chain Shape In Box2D//

	//Start Create Edge Shape//

	glm::vec2 ChainLocPixelCoordinates = { 100.0f,500.0f };//{ Ermine::GetScreenWidth()/2,Ermine::GetScreenHeight()/2};
	glm::vec2 ChainLoc = Ermine::coordPixelsToWorld(ChainLocPixelCoordinates);

	b2BodyDef EdgeBodyDef;
	EdgeBodyDef.position.Set(ChainLoc.x, ChainLoc.y);
	EdgeBodyDef.type = b2_staticBody;

	std::vector<b2Vec2> EdgeVertexes;

	glm::vec2 P1 = Ermine::vertexPixelsToWorld(glm::vec2(0.0f,0.0f));
	EdgeVertexes.emplace_back(b2Vec2(P1.x,P1.y));

	glm::vec2 P2 = Ermine::vertexPixelsToWorld(glm::vec2(500.0f,100.0f));
	EdgeVertexes.emplace_back(b2Vec2(P2.x,P2.y));

	b2EdgeShape* EdShp = new b2EdgeShape();

	EdShp->Set(EdgeVertexes[0], EdgeVertexes[1]);

	b2FixtureDef EdgeShapeFixtureDef;
	EdgeShapeFixtureDef.density = 1.0f;
	EdgeShapeFixtureDef.friction = 0.3f;

	EdgeShapeFixtureDef.shape = EdShp;

	static PhysicsComponent2D EdgeShapeObj(EdgeBodyDef, EdgeShapeFixtureDef);

	delete EdShp;

	//Ended Create Edge Shape//

	//static PhysicsComponent2D Obj = PhysicsComponent2D(Def, FDef);
	//static PhysicsComponent2D Obj(Def, FDef, glm::vec2(100.0f, 100.0f));
	//Ended Creating Box Physics Component//

	//Start Create Ground Physics Component..//

	glm::vec2 LocPixelCoordinatesGrd = { 500.0f,1000.0f };//{ Ermine::GetScreenWidth()/2,Ermine::GetScreenHeight()/2};
	glm::vec2 LocGrd = Ermine::coordPixelsToWorld(LocPixelCoordinatesGrd);

	b2BodyDef GroundBodyDefinition;
	GroundBodyDefinition.position.Set(LocGrd.x,LocGrd.y);
	GroundBodyDefinition.type = b2_staticBody;

	//static auto GroundShape = b2PolygonShape();
	//GroundShape.SetAsBox(100.0f, 0.01f);

	b2FixtureDef GroundFixture;
	GroundFixture.density = 1.0f;
	//GroundFixture.shape = &GroundShape;
	GroundFixture.friction = 0.3f;

	static PhysicsComponent2D GroundBody(GroundBodyDefinition,GroundFixture,glm::vec2(1000.0f,50.0f));

	//Ended Create Ground Physics Component..//

	//b2Vec2 Position = Obj.operator b2Body* ()->GetPosition();
	//float angle = Obj.operator b2Body * ()->GetAngle();

	//std::cout << "Position : [" << Position.x << "," << Position.y << "]" << " Angle : " << angle << std::endl;
	//std::cout << Obj.operator b2Body * ()->GetPosition().y<<std::endl;

	//Ended Physics Component Test//

#pragma endregion PhysicsComponentTest