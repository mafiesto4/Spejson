
#include "Lava.h"
#include "../Game.h"
#include "Chunk.h"
#include "../Objects/Shop.h"
#include "../Opponent/Boss/Boss.h"

using namespace cocos2d;

bool Lava::init()
{
	if (!LayerGradient::initWithColor(Color4B(250, 220, 0, 0), Color4B(255, 0, 0, 255)))
	{
		return false;
	}

	setContentSize(Size(10000, 600));
	setPosition(Vec2(-100, -CHUNKS_DEFAULT_HEIGHT * 4));
	setAnchorPoint(Vec2(0.5f, 1.0f));

	scheduleUpdateWithPriority(1410);

	return true;
}

float Lava::getLevel()
{
	return getPositionY();
}

void Lava::update(float dt)
{
	// Cahe data
	auto player = Game::getInstance()->getPlayer();
	auto playerPosY = player->getPosition().y;
	float posY = getPositionY();

	// Check if player is not using shop and there is no bosses spawned
	if (!AnyShopInUse && Bosses == 0)
	{
		// Move lava
		float speed = (playerPosY < posY + CHUNKS_DEFAULT_HEIGHT * 8) ? 2.0f : 0.3f;
		float lavaLevel = posY + speed;
		setPositionY(lavaLevel);

		// Apply damage to the player
		if (playerPosY < lavaLevel + 100)
		{
			player->applyDamage(10000000);
		}
		else if (playerPosY < lavaLevel + 400)
		{
			player->applyDamage(1);
		}
	}
}
