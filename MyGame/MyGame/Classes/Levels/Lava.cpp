
#include "Lava.h"
#include "../Game.h"
#include "Chunk.h"
#include "../Objects/Shop.h"
#include "../Opponent/Boss/Boss.h"

using namespace cocos2d;

bool Lava::init()
{
	if (!initWithColor(Color4B(250, 220, 0, 0), Color4B(255, 0, 0, 255)))
	{
		return false;
	}

	setContentSize(Size(10000, 600));
	setPosition(Vec2(-800, -CHUNKS_DEFAULT_HEIGHT * 4));
	setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/lava2.png");
	Texture2D::TexParams tp = { GL_POINT, GL_POINT, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	auto lava = Sprite::createWithTexture(texture, Rect(0, 0, 10000, 1000));
	lava->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	addChild(lava);


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
		float speed = (playerPosY < posY + CHUNKS_DEFAULT_HEIGHT * 8) ? 2.0f : 0.32f;
		float lavaLevel = posY + speed;
		setPositionY(lavaLevel);

		// Apply damage to the player
		if (playerPosY < lavaLevel)
		{
			player->applyDamage(10000000);
		}
		else if (playerPosY < lavaLevel + 400)
		{
			player->applyDamage(1);
		}
	}
}
