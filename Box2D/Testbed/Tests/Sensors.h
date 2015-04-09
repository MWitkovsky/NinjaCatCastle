#ifndef SENSORS_H
#define SENSORS_H

#include <vector>

enum _entityCategory1 {
	BOUNDARY1 = 0x0001,
	FRIENDLY_SHIP1 = 0x0002,
	ENEMY_SHIP1 = 0x0004,
	FRIENDLY_AIRCRAFT1 = 0x0008,
	ENEMY_AIRCRAFT1 = 0x0010,
	FRIENDLY_TOWER1 = 0x0020,
	RADAR_SENSOR1 = 0x0040,
};

//outside and before the FooTest class
class BallCol2 {
public:
	//class member variables
	b2Body* m_body;
	float m_radius;

	//Ball class member variable
	//bool m_contacting;
	int m_numContacts;
	b2Color m_color;
	std::vector<BallCol2*> visibleEnemies;

public:
	void radarAcquiredEnemy(BallCol2* enemy) {
		visibleEnemies.push_back(enemy);
	}
	void radarLostEnemy(BallCol2* enemy) {
		visibleEnemies.erase(std::find(visibleEnemies.begin(), visibleEnemies.end(), enemy));
	}

	// Ball class constructor
	BallCol2(b2World* world, float radius, b2Color color, uint16 categoryBits, uint16 maskBits) {
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
	~BallCol2() {}

	void startContact() { m_numContacts++; }
	void endContact() { m_numContacts--; }

	//BallCol2::render
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

		if (visibleEnemies.size() > 0)
			glColor3f(1, 1, 0); //yellow
		else
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

	//in BallCol2 class
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


class MyContactListener2 : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol2*>(bodyUserData)->startContact();

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol2*>(bodyUserData)->startContact();

	}

	void EndContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol2*>(bodyUserData)->endContact();

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<BallCol2*>(bodyUserData)->endContact();

	}
};

//at global scope
MyContactListener2 myContactListenerInstance2;


class Sensors : public Test
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
	std::vector<BallCol2*> balls;

	Sensors() {

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

		/*
		//large and green are friendly ships
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol2(m_world, 3, green, FRIENDLY_SHIP1, BOUNDARY1 | FRIENDLY_SHIP1 | ENEMY_SHIP1));
		//large and red are enemy ships
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol2(m_world, 3, red, ENEMY_SHIP1, BOUNDARY1 | FRIENDLY_SHIP1 | ENEMY_SHIP1));
		//small and green are friendly aircraft
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol2(m_world, 1, green, FRIENDLY_AIRCRAFT1, BOUNDARY1 | ENEMY_AIRCRAFT1));
		//small and red are enemy aircraft
		for (int i = 0; i < 5; i++)
			balls.push_back(new BallCol2(m_world, 1, red, ENEMY_AIRCRAFT1, BOUNDARY1 | FRIENDLY_AIRCRAFT1));
			*/

		BallCol2* ship = new BallCol2(m_world, 3, green, FRIENDLY_SHIP, BOUNDARY1 | FRIENDLY_TOWER1);
		b2CircleShape circleShape;
		circleShape.m_radius = 8;
		myFixtureDef.shape = &circleShape;
		myFixtureDef.isSensor = true;
		myFixtureDef.filter.categoryBits = RADAR_SENSOR1;
		myFixtureDef.filter.maskBits = ENEMY_AIRCRAFT1;//radar only collides with aircraft
		ship->m_body->CreateFixture(&myFixtureDef);
		balls.push_back(ship);

		//three enemy aircraft
		for (int i = 0; i < 3; i++)
			balls.push_back(new BallCol2(m_world, 1, red, ENEMY_AIRCRAFT1, BOUNDARY1 | RADAR_SENSOR1));

		//a tower entity
		BallCol2* tower = new BallCol2(m_world, 1, green, FRIENDLY_TOWER1, FRIENDLY_SHIP1);
		float radius = 8;
		b2Vec2 vertices[8];
		vertices[0].Set(0, 0);
		for (int i = 0; i < 7; i++) {
			float angle = i / 6.0 * 90 * DEGTORAD;
			vertices[i + 1].Set(radius * cosf(angle), radius * sinf(angle));
		}
		polygonShape.Set(vertices, 8);
		myFixtureDef.shape = &polygonShape;
		tower->m_body->CreateFixture(&myFixtureDef);

		//make the tower rotate at 45 degrees per second
		tower->m_body->SetAngularVelocity(45 * DEGTORAD);
		tower->m_body->SetType(b2_kinematicBody);
		balls.push_back(tower);


		m_world->SetContactListener(&myContactListenerInstance2);

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
		return new Sensors;
	}
};




#endif