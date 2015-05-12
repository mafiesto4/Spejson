
#include "Lava.h"
#include "../Game.h"

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
	float lavaLevel = getPositionY();// +0.8f;
	setPositionY(lavaLevel);

	// Apply damage to the player
	auto player = Game::getInstance()->getPlayer();
	if (player && player->getPosition().y < lavaLevel)
	{
		MessageBox("Player has been killed by lava!", "Death");
		Director::getInstance()->end();
	}
}
