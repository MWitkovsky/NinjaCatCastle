#ifndef COLLISIONFILTER_H
#define COLLISIONFILTER_H

#include <vector>

enum _entityCategory {
	BOUNDARY = 0x0001,
	FRIENDLY_SHIP = 0x0002,
	ENEMY_SHIP = 0x0004,
	FRIENDLY_AIRCRAFT = 0x0008,
	ENEMY_AIRCRAFT = 0x0010,
};

//outside and before the FooTest class
class BallCol1 {
public:
	//class member variables
	b2Body* m_body;
	float m_radius;

	//Ball class member variable
	//bool m_contacting;
	int m_numContacts;
	b2Color m_color;

public:
	// Ball class constructor
	BallCol1(b2World* world, float radius, b2Color color, uint16 categoryBits, uint16 maskBits) {
		m_color = color;
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

		myFixtureDef.filter.categoryBits = categoryBits;
		myFixtureDef.filter.maskBits = maskBits;

		m_body->SetUserData(this); //set this Ball object in the body's user data
		//m_contacting = false;
	}
	~BallCol1() {}

	void startContact() { m_numContacts++; }
	void endContact() { m_numContacts--; }

	//BallCol1::render
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

		glColor3f(m_color.r, m_color.g, m_color.b);

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

	//in BallCol1 class
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


class MyContactListener1 : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol1*>(bodyUserData)->startContact();

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol1*>(bodyUserData)->startContact();

	}

	void EndContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol1*>(bodyUserData)->endContact();

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol1*>(bodyUserData)->endContact();

	}
};

//at global scope
MyContactListener1 myContactListenerInstance1;


class CollisionFilter : public Test
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
	std::vector<BallCol1*> balls;

	CollisionFilter() {

		m_world->SetGravity(b2Vec2(0, 0));

		b2Color red(1, 0, 0);
		b2Color green(0, 1, 0);

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
		//BallCol1* ball = new BallCol1(m_world, 1);
		//balls.push_back(ball);

		//large and green are friendly ships
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol1(m_world, 3, green, FRIENDLY_SHIP, BOUNDARY | FRIENDLY_SHIP | ENEMY_SHIP));
		//large and red are enemy ships
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol1(m_world, 3, red, ENEMY_SHIP, BOUNDARY | FRIENDLY_SHIP | ENEMY_SHIP));
		//small and green are friendly aircraft
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol1(m_world, 1, green, FRIENDLY_AIRCRAFT, BOUNDARY | ENEMY_AIRCRAFT));
		//small and red are enemy aircraft
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol1(m_world, 1, red, ENEMY_AIRCRAFT, BOUNDARY | FRIENDLY_AIRCRAFT));


		m_world->SetContactListener(&myContactListenerInstance1);

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
		return new CollisionFilter;
	}
};




#endif