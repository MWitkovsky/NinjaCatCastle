#ifndef FOOTEST_H
#define FOOTEST_H

class FooTest : public Test
{
public:
	//class member variables
	b2Body* body;

	FooTest() {
	
		//body definition
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;

		//shape definition
		b2PolygonShape polygonShape;
		float32 width = 7.0;
		float32 height = 10;
		float32 edgeWidth = 1.00; 
		float32 edgeHeight = 1.00;
		b2Vec2 vertices[8];
		vertices[0].Set(-width + edgeWidth, -height);		// bottom
		vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
		vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
		vertices[3].Set(width, height - edgeHeight);		// top-right edge start
		vertices[4].Set(width - edgeWidth, height);			// top-right edge end
		vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
		vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
		vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
		polygonShape.Set(vertices, 8);
		//polygonShape.SetAsBox(7, 10); //a 2x2 rectangle

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;


		//create dynamic body
		myBodyDef.position.Set(0, 10);
		body = m_world->CreateBody(&myBodyDef);
		body->CreateFixture(&myFixtureDef);
		//body->SetGravityScale(0);

		//a static body
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(0, 0);
		b2Body* staticBody = m_world->CreateBody(&myBodyDef);

		//add four walls to the static body
		polygonShape.SetAsBox(20, 1, b2Vec2(0, 0), 0);//ground
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(20, 1, b2Vec2(0, 40), 0);//ceiling
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(1, 20, b2Vec2(-20, 20), 0);//left wall
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(1, 20, b2Vec2(20, 20), 0);//right wall
		staticBody->CreateFixture(&myFixtureDef);
	} //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Now we have a foo testttttt");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new FooTest;
	}
};

#endif