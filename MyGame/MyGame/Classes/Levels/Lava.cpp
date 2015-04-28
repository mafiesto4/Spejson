
#include "Lava.h"

using namespace cocos2d;

bool Lava::init()
{
	//if (!Layer::init())
	if (!LayerGradient::initWithColor(Color4B(250, 220, 0, 0), Color4B(255, 0, 0, 255)))
	{
		return false;
	}

	setContentSize(Size(10000, 600));
	setPosition(Vec2(-5000, -800));
	setAnchorPoint(Vec2(0.5f, 1.0f));

	// uruchom funckje update(...)
	scheduleUpdateWithPriority(1410);

	
	return true;
}

float Lava::getLevel()
{
	return getPositionY();
}

void Lava::update(float dt)
{
	// move lava
	setPositionY(getPositionY() + 1.3f);
}
