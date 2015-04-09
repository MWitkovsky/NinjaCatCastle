#ifndef MAPLOADTEST_H
#define MAPLOADTEST_H

#include <string>
#include "b2dJson.h"

class MapLoadTest : public Test
{
public:
	MapLoadTest() {
	
		b2Vec2 gravity(0, -9.8); //normal earth gravity, 9.8 m/s/s straight down!
		bool doSleep = true;

		std::string errorMsg;
		b2dJson json;
		//b2World* myWorld = json.readFromFile("D:\\works\\cse380\\ninjacatcastle\\Box2D\\Testbed\\Tests\\bullet.json", errorMsg);
		b2World* myWorld = json.readFromFile("images.json", errorMsg);
		/*
		std::string errorMsg1;
		b2dJson json1;
		myWorld = json1.readFromFile("D:\\works\\cse380\\ninjacatcastle\\Box2D\\Testbed\\Tests\\bullet.json", errorMsg1);

		b2dJson json2;
		std::string mystring = json2.writeToString(myWorld);

		std::string errorMsg2;
		b2dJson json3;
		myWorld = json3.readFromString(mystring, errorMsg2);
		*/

		/*
		std::string errorMsg;
		b2dJson json1;
		b2World* myWorld = json1.readFromFile("D:\\works\\cse380\\ninjacatcastle\\Box2D\\Testbed\\Tests\\bullet.json", errorMsg);

		b2Fixture* finishLine = json1.getFixtureByName("finish");  // use json1 (level)

		b2Vec2 delta(1.2, 3.4); // move all bodies by this offset
		*/

		myWorld->SetGravity(gravity);
	}

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Map Load Test");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new MapLoadTest;
	}

	std::string getCurrentDirectoryOnWindows()
	{
		const unsigned long maxDir = 260;
		char currentDir[maxDir];
		GetCurrentDirectory(maxDir, currentDir);
		return std::string(currentDir);
	}
};

#endif