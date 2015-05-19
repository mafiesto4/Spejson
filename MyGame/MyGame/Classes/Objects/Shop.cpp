#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Shop.h"
#include "../Levels/Chunk.h"
using namespace cocos2d;

Shop::Shop(Chunk* parent, Vec2 pos)
	:Entity(parent),
	_isOver(false)
{
	_image = Sprite::create("Textures/shop1.png");
	_image->setPosition(pos + Vec2(64, 0));
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	_parent->addChild(_image);
}

Shop::~Shop()
{
	if (_image)
	{
		_image->removeFromPhysicsWorld();
		_image->removeAllChildren();
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Shop::update(Level* level, float dt)
{
	// Check if player is over the shop
	Vec2 pos = _image->getPosition() + _parent->getPosition();
	auto player = Game::getInstance()->getPlayer();
	bool isOver = player->getBox().containsPoint(pos);
	
	// Check if can show shop stuff
	if (!_isOver && isOver)
	{
		MessageBox("gracz wlazl w zsklep", "sklep z ziolem");
	}

	// Store state
	_isOver = isOver;

	return false;
}
