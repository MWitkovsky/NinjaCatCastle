#ifndef TESTWORLD_H
#define TESTWORLD_H

class TestWorld : public Test
{
public:
	TestWorld() { } //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Test World!!!!!");
		m_textLine += 15;

		b2Vec2 gravity(0, -9.8); //normal earth gravity, 9.8 m/s/s straight down!
		bool doSleep = true;

		b2World* myWorld = new b2World(gravity);

		myWorld->SetGravity(b2Vec2(0, 0));

		float32 timeStep = 1 / 20.0;      //the length of time passed to simulate (seconds)
		int32 velocityIterations = 8;   //how strongly to correct velocity
		int32 positionIterations = 3;   //how strongly to correct position

		myWorld->Step(timeStep, velocityIterations, positionIterations);
	}

	static Test* Create()
	{
		return new TestWorld;
	}
};

#endif