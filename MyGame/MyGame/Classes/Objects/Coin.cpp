#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Coin.h"
#include "../Levels/Chunk.h"

using namespace cocos2d;

Coin::Coin(Chunk* parent, Vec2 pos)
	:Entity(parent, pos),
	_image(nullptr)
{
	_image = Sprite::create("Textures/coin.png");
	_image->setPosition(pos);
	_image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_parent->addChild(_image);

	setupAnim(1, 4, 8, 10);
}

Coin::~Coin()
{
	if (_image)
	{
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Coin::update(Level* level, float dt)
{
	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 pos = _image->getPosition() + _parent->getPosition() - Vec2(CHUNKS_BLOCK_SIZE_HALF, CHUNKS_BLOCK_SIZE_HALF);
	auto size = _image->getContentSize();
	Rect box = Rect(pos.x, pos.y, size.width, size.height);

	// Test collision
	if (player->getBox().intersectsRect(box))
	{
		// Add cash
		player->addCash(1);
		return true;
	}
	
	// Sprite animation
	animate(dt, _image);

	return false;
}
