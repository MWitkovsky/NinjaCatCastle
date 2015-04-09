#ifndef ANATOMYCOLLISION_H
#define ANATOMYCOLLISION_H

class AnatomyCollision : public Test
{
public:
	AnatomyCollision() { } //do nothing, no scene yet

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		m_debugDraw.DrawString(5, m_textLine, "Anatomy collision is running");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new AnatomyCollision;
	}
};

#endif