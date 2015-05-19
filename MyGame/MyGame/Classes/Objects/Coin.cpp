#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Coin.h"
#include "../Levels/Chunk.h"
using namespace cocos2d;

Coin::Coin(Chunk* parent, Vec2 pos)
	:Entity(parent)
{
	_value = 1;
	_image = Sprite::create("Textures/super_mario_coin.png");
	_image->setPosition(pos + Vec2(24,0));
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	_parent->addChild(_image);
}

Coin::~Coin()
{
	if (_image)
	{
		_image->removeFromPhysicsWorld();
		_image->removeAllChildren();
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Coin::update(Level* level, float dt)
{
	Vec2 coinPos = _image->getPosition() + _parent->getPosition();

	auto player = Game::getInstance()->getPlayer();
	if (player->getBox().containsPoint(coinPos))
	{
		player->addCash(_value);
		return true;
	}

	return false;
}
