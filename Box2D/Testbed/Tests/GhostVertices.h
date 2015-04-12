#ifndef GHOSTVERTICES_H
#define GHOSTVERTICES_H

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

//demonstrates the use of ghost vertices of edge shapes

class GhostVertices : public Test
{
public:
	//for keeping track of input state
	enum moveState {
		MS_STOP,
		MS_LEFT,
		MS_RIGHT,
	};

	float getHeight(float x) {
		return 0;//1 - cosf( x * 0.1f );
	}

	GhostVertices() {

		//body def
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;

		//fixture def
		b2FixtureDef myFixtureDef;
		myFixtureDef.density = 1;
		myFixtureDef.friction = 0;
		b2PolygonShape polygonShape;
		myFixtureDef.shape = &polygonShape;
		polygonShape.SetAsBox(1, 1);

		//a dynamic body to act as the main body of a player character
		myBodyDef.position.Set(0, 5);
		m_playerBody = m_world->CreateBody(&myBodyDef);
		m_playerBody->CreateFixture(&myFixtureDef);

		//another dynamic body to weigh down the main body and make the problem occur more regularly
		myBodyDef.fixedRotation = true;
		myBodyDef.position.Set(0, 6);
		myFixtureDef.density = 50;
		m_weightBody = m_world->CreateBody(&myBodyDef);
		m_weightBody->CreateFixture(&myFixtureDef);

		//joint the two bodies together
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = m_playerBody;
		jointDef.bodyB = m_weightBody;
		jointDef.localAnchorA.Set(0, 0);
		jointDef.localAnchorB.Set(0, -1);
		m_world->CreateJoint(&jointDef);

		//body to support the 'ground' fixtures
		myBodyDef.position.Set(0, 0);
		b2Body* fakeGroundBody = m_world->CreateBody(&myBodyDef);

		//ground made of boxes
		for (int i = 0; i < 21; i++) {
			polygonShape.SetAsBox(1, 2, b2Vec2(-20 + i * 2, 2), 0);
			fakeGroundBody->CreateFixture(&myFixtureDef);
		}

		//ground made of edges
		b2EdgeShape edgeShape;
		myFixtureDef.shape = &edgeShape;
		for (int i = 0; i < 21; i++) {
			float left = -20 + i * 2 - 1;
			float right = -20 + i * 2 + 1;
			edgeShape.Set(b2Vec2(left, 10 + getHeight(left)), b2Vec2(right, 10 + getHeight(right)));
			fakeGroundBody->CreateFixture(&myFixtureDef);
		}

		//ground made of edges with ghost vertices set
		myFixtureDef.shape = &edgeShape;
		for (int i = 0; i < 21; i++) {
			float left = -20 + i * 2 - 1;
			float right = -20 + i * 2 + 1;
			edgeShape.Set(b2Vec2(left, 20 + getHeight(left)), b2Vec2(right, 20 + getHeight(right)));
			edgeShape.m_vertex0.Set(left - 1, 20 + getHeight(left));
			edgeShape.m_vertex3.Set(right + 1, 20 + getHeight(right));
			edgeShape.m_hasVertex0 = true;
			edgeShape.m_hasVertex3 = true;
			fakeGroundBody->CreateFixture(&myFixtureDef);
		}

		//ground body
		b2BodyDef groundBodyDef;
		b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

		edgeShape.Set(b2Vec2(-20, 0), b2Vec2(20, 0));
		groundBody->CreateFixture(&myFixtureDef);
		edgeShape.Set(b2Vec2(-21.1, 0), b2Vec2(-21.1, 2));
		groundBody->CreateFixture(&myFixtureDef);
		edgeShape.Set(b2Vec2(21.1, 0), b2Vec2(21.1, 2));
		groundBody->CreateFixture(&myFixtureDef);

		m_moveState = MS_STOP;
	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'q': //move left
			m_moveState = MS_LEFT;
			break;
		case 'w': //stop
			m_moveState = MS_STOP;
			break;
		case 'e': //move right
			m_moveState = MS_RIGHT;
			break;

		default:
			Test::Keyboard(key);
		}
	}

	void Step(Settings* settings)
	{
		b2Vec2 vel = m_weightBody->GetLinearVelocity();
		float force = 0;
		switch (m_moveState)
		{
		case MS_LEFT:  if (vel.x > -5) force = -1000;  break;
		case MS_STOP:  force = vel.x * -100; break;
		case MS_RIGHT: if (vel.x <  5) force = 1000; break;
		}
		m_weightBody->ApplyForce(b2Vec2(force, -10), m_weightBody->GetWorldCenter(), true);

		//tap the lower body downwards just as it nears a tile boundary
		b2Vec2 pos = m_playerBody->GetPosition();
		if (fabsf(pos.x - (int)pos.x) < 0.02f)
			m_playerBody->ApplyLinearImpulse(b2Vec2(0, -20), m_weightBody->GetWorldCenter(), true);

		//run the default physics and rendering
		Test::Step(settings);

		m_debugDraw.DrawString(5, m_textLine, "Press q/w/e to move the player");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new GhostVertices;
	}

	b2Body* m_playerBody;
	b2Body* m_weightBody;
	moveState m_moveState;

};

#endif