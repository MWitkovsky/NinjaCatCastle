#ifndef BODIES_H
#define BODIES_H

class Bodies : public Test
{
public:
	Bodies() {
		// Creating a body
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
		myBodyDef.position.Set(0, 20); //set the starting position
		myBodyDef.angle = 0; //set the starting angle

		b2Body* dynamicBody = m_world->CreateBody(&myBodyDef);

		b2PolygonShape boxShape;
		float32 width = 0.60f;
		float32 height = 0.8f;
		float32 edgeWidth = 0.5f;
		float32 edgeHeight = 0.8f;
		b2Vec2 vertices[8];
		vertices[0].Set(-width + edgeWidth, -height);		// bottom
		vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
		vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
		vertices[3].Set(width, height - edgeHeight);		// top-right edge start
		vertices[4].Set(width - edgeWidth, height);			// top-right edge end
		vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
		vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
		vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
		boxShape.Set(vertices, 8);

		//I like rectangles anyway
		//shape.SetAsBox(0.7, 1);
		b2FixtureDef boxFixtureDef;
		boxFixtureDef.shape = &boxShape;
		boxFixtureDef.density = 1;
		dynamicBody->CreateFixture(&boxFixtureDef);

		dynamicBody->SetTransform(b2Vec2(10, 20), 1);

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

		dynamicBody->SetTransform(b2Vec2(10, 20), 45 * DEGTORAD); //45 degrees counter-clockwise

		dynamicBody->SetLinearVelocity(b2Vec2(-5, 5)); //moving up and left 5 units per second
		dynamicBody->SetAngularVelocity(-90 * DEGTORAD); //90 degrees per second clockwise


		// Static bodies
		myBodyDef.type = b2_staticBody; //this will be a static body
		myBodyDef.position.Set(0, 10); //slightly lower position
		b2Body* staticBody = m_world->CreateBody(&myBodyDef); //add body to world
		staticBody->CreateFixture(&boxFixtureDef); //add fixture to body

		// Kinematic bodies
		myBodyDef.type = b2_kinematicBody; //this will be a kinematic body
		myBodyDef.position.Set(-18, 11); // start from left side, slightly above the static body
		b2Body* kinematicBody = m_world->CreateBody(&myBodyDef); //add body to world
		kinematicBody->CreateFixture(&boxFixtureDef); //add fixture to body

		kinematicBody->SetLinearVelocity(b2Vec2(1, 0)); //move right 1 unit per second
		kinematicBody->SetAngularVelocity(360 * DEGTORAD); //1 turn per second counter-clockwise


		b2Vec2 pos = dynamicBody->GetPosition();
		float angle = dynamicBody->GetAngle();
		b2Vec2 vel = dynamicBody->GetLinearVelocity();
		float angularVel = dynamicBody->GetAngularVelocity();
		m_debugDraw.DrawString(5, m_textLine,
			"Position:%.3f,%.3f Angle:%.3f", pos.x, pos.y, angle * RADTODEG);
		m_textLine += 15;
		m_debugDraw.DrawString(5, m_textLine,
			"Velocity:%.3f,%.3f Angular velocity:%.3f", vel.x, vel.y, angularVel * RADTODEG);
		m_textLine += 15;
	} //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Now we have my testttttt");
		m_textLine += 15;



	}

	static Test* Create()
	{
		return new Bodies;
	}
};

#endif