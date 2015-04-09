#ifndef OWNOBJECT_H
#define OWNOBJECT_H

#include <vector>

//outside and before the FooTest class
class Ball {
public:
	//class member variables
	b2Body* m_body;
	float m_radius;

public:
	// Ball class constructor
	Ball(b2World* world, float radius) {
		m_body = NULL;
		m_radius = radius;

		//set up dynamic body, store in class variable
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.position.Set(0, 20);
		m_body = world->CreateBody(&myBodyDef);

		//add circle fixture
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0);
		circleShape.m_radius = m_radius; //use class variable
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &circleShape;
		myFixtureDef.density = 1;
		m_body->CreateFixture(&myFixtureDef);
	}
	~Ball() {}

	//Ball::render
	void render() {

		glColor3f(1, 1, 1);//white

		//nose and eyes
		glPointSize(4);
		glBegin(GL_POINTS);
		glVertex2f(0, 0);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glEnd();

		//mouth
		glBegin(GL_LINES);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.16, -0.6);
		glVertex2f(0.16, -0.6);
		glVertex2f(0.5, -0.5);
		glEnd();

		//circle outline
		glBegin(GL_LINE_LOOP);
		for (float a = 0; a < 360 * DEGTORAD; a += 30 * DEGTORAD)
			glVertex2f(sinf(a), cosf(a));
		glEnd();
	}
	//in Ball class
	void renderAtBodyPosition() {

		//get current position from Box2D
		b2Vec2 pos = m_body->GetPosition();
		float angle = m_body->GetAngle();

		//call normal render at different position/rotation
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		glRotatef(angle * RADTODEG, 0, 0, 1);//OpenGL uses degrees here
		render();//normal render at (0,0)
		glPopMatrix();
	}
};



class OwnObject : public Test
{
public:
	//FooTest class member variable
	std::vector<Ball*> balls;

	OwnObject() {
		
		//a static body
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(0, 0);
		b2Body* staticBody = m_world->CreateBody(&myBodyDef);

		//shape definition
		b2PolygonShape polygonShape;

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;

		//add four walls to the static body
		polygonShape.SetAsBox(20, 1, b2Vec2(0, 0), 0);//ground
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(20, 1, b2Vec2(0, 40), 0);//ceiling
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(1, 20, b2Vec2(-20, 20), 0);//left wall
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(1, 20, b2Vec2(20, 20), 0);//right wall
		staticBody->CreateFixture(&myFixtureDef);

		//add ball entity to scene in constructor
		Ball* ball = new Ball(m_world, 1);
		balls.push_back(ball);

	} //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Own Object is running");
		m_textLine += 15;

		//in Step() function, after Test::Step()
		for (int i = 0; i < balls.size(); i++)
			balls[i]->renderAtBodyPosition();
	}

	static Test* Create()
	{
		return new OwnObject;
	}
};

#endif