#ifndef CONSTSPEEDTEST_H
#define CONSTSPEEDTEST_H

class ConstSpeedTest : public Test
{


public:
	//enumeration of possible input states
	enum _moveState {
		MS_STOP,
		MS_LEFT,
		MS_RIGHT,
	};

	//class member variables
	b2Body* body;
	_moveState moveState;

	int remainingJumpSteps;

	ConstSpeedTest() {
		remainingJumpSteps = 0;

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

		moveState = MS_STOP;
	} //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "ConstSpeedTest is running");
		m_textLine += 15;

		/*
		b2Vec2 vel = body->GetLinearVelocity();
		switch (moveState)
		{
		case MS_LEFT:  vel.x = b2Max(vel.x - 0.1f, -5.0f); break;
		case MS_STOP:  vel.x *= 0.98; break;
		case MS_RIGHT: vel.x = b2Min(vel.x + 0.1f, 5.0f); break;
		}
		body->SetLinearVelocity(vel);
		*/
		/*
		b2Vec2 vel = body->GetLinearVelocity();
		float force = 0;
		switch (moveState)
		{
		case MS_LEFT:  if (vel.x > -5) force = -50;  break;
		case MS_STOP:  force = vel.x * -10; break;
		case MS_RIGHT: if (vel.x <  5) force = 50; break;
		}
		body->ApplyForce(b2Vec2(force, 0), body->GetWorldCenter(), true);
		*/
		b2Vec2 vel = body->GetLinearVelocity();
		float desiredVel = 0;
		switch (moveState)
		{
		case MS_LEFT:  desiredVel = -5; break;
		case MS_STOP:  desiredVel = 0; break;
		case MS_RIGHT: desiredVel = 5; break;
		}
		float velChange = desiredVel - vel.x;
		float force = body->GetMass() * velChange / (1 / 60.0); //f = mv/t
		body->ApplyForce(b2Vec2(force, 0), body->GetWorldCenter(), true);


		if (remainingJumpSteps > 0) {
			body->ApplyForce(b2Vec2(0, 500), body->GetWorldCenter(), true);
			remainingJumpSteps--;
		}

	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'q': //move left
			moveState = MS_LEFT;
			break;
		case 'w': //stop
			moveState = MS_STOP;
			break;
		case 'e': //move right
			moveState = MS_RIGHT;
			break;
		case 'j': //jump
		{
			b2Vec2 vel = body->GetLinearVelocity();
			vel.y = 10;//upwards - don't change x velocity
			body->SetLinearVelocity(vel);
		}
			break;
		case 'k': //jump
			remainingJumpSteps = 6;// 1/10th of a second at 60Hz
			break;
		case 'l':
		{
			//to change velocity by 10
			float impulse = body->GetMass() * 10;
			body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter(), true);
		}
			break;
		default:
			//run default behaviour
			Test::Keyboard(key);
		}
	}

	static Test* Create()
	{
		return new ConstSpeedTest;
	}
};

#endif