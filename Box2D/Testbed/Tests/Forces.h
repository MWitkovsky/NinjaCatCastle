#ifndef FORCES_H
#define FORCES_H

class Forces : public Test
{
public:
	//class member variable to keep track of three bodies
	b2Body* bodies[3];

	Forces() {
		//body definition
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;

		//shape definition
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(1, 1); //a 2x2 rectangle

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;

		//create identical bodies in different positions
		for (int i = 0; i < 3; i++) {
			myBodyDef.position.Set(-10 + i * 10, 20);
			bodies[i] = m_world->CreateBody(&myBodyDef);
			bodies[i]->CreateFixture(&myFixtureDef);
		}

		//a static floor to drop things on
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(0, 0);
		//polygonShape.SetAsEdge(b2Vec2(-15, 0), b2Vec2(15, 0));
		m_world->CreateBody(&myBodyDef)->CreateFixture(&myFixtureDef);
	} //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Forces Test");
		m_textLine += 15;




	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'q':
			//apply gradual force upwards
			bodies[0]->ApplyForce(b2Vec2(0, 50), bodies[0]->GetWorldCenter(), true);
			break;
		case 'w':
			//apply immediate force upwards
			bodies[1]->ApplyLinearImpulse(b2Vec2(0, 50), bodies[1]->GetWorldCenter(), true);
			break;
		case 'e':
			//teleport or 'warp' to new location
			bodies[2]->SetTransform(b2Vec2(10, 20), 0);
			break;
		default:
			//run default behaviour
			Test::Keyboard(key);
		}
	}

	static Test* Create()
	{
		return new Forces;
	}
};

#endif