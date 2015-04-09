#ifndef COLLISIONCALLBACK_H
#define COLLISIONCALLBACK_H

#include <vector>



//outside and before the FooTest class
class BallCol {
public:
	//class member variables
	b2Body* m_body;
	float m_radius;

	//Ball class member variable
	//bool m_contacting;
	int m_numContacts;

public:
	// Ball class constructor
	BallCol(b2World* world, float radius) {
		m_body = NULL;
		m_radius = radius;

		m_numContacts = 0;

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

		m_body->SetUserData(this); //set this Ball object in the body's user data
		//m_contacting = false;
	}
	~BallCol() {}

	void startContact() { m_numContacts++; }
	void endContact() { m_numContacts--; }

	//BallCol::render
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

		if (m_numContacts > 0)
			glColor3f(1, 0, 0);//red
		else
			glColor3f(1, 1, 1);//white

		//circle outline
		glBegin(GL_LINE_LOOP);
		for (float a = 0; a < 360 * DEGTORAD; a += 30 * DEGTORAD)
			glVertex2f(sinf(a), cosf(a));
		glEnd();
	}

	float mini(float a, float b){
		if (a > b){
			return b;
		}
		else{
			return a;
		}
	}

	//in BallCol class
	void renderAtBodyPosition() {

		//get current position from Box2D
		b2Vec2 pos = m_body->GetPosition();
		float angle = m_body->GetAngle();

		//call normal render at different position/rotation
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		glRotatef(angle * RADTODEG, 0, 0, 1);//OpenGL uses degrees here
		glScalef(m_radius, m_radius, 1); //add this to correct size
		render();//normal render at (0,0)
		glPopMatrix();
	}
};


class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol*>(bodyUserData)->startContact();

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol*>(bodyUserData)->startContact();

	}

	void EndContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol*>(bodyUserData)->endContact();

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol*>(bodyUserData)->endContact();

	}
};

//at global scope
MyContactListener myContactListenerInstance;


class CollisionCallback : public Test
{
public:


	//b2ContactListener
	// Called when two fixtures begin to touch
	//virtual void BeginContact(b2Contact* contact);

	// Called when two fixtures cease to touch
	//virtual void EndContact(b2Contact* contact);

	//b2Contact
	// Get the first fixture in this contact
	b2Fixture* GetFixtureA();

	// Get the second fixture in this contact
	b2Fixture* GetFixtureB();

	//FooTest class member variable
	std::vector<BallCol*> balls;

	CollisionCallback() {

		m_world->SetGravity(b2Vec2(0, 0));

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
		//BallCol* ball = new BallCol(m_world, 1);
		//balls.push_back(ball);

		for (int i = 0; i < 40; i++) {
			float radius = 1 + 2 * (rand() / (float)RAND_MAX); //random between 1 - 3
			BallCol* ball = new BallCol(m_world, radius);
			balls.push_back(ball);
		}


		m_world->SetContactListener(&myContactListenerInstance);

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
		return new CollisionCallback;
	}
};




#endif