#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Shop.h"
#include "../Levels/Chunk.h"
using namespace cocos2d;

Shop::Shop(Chunk* parent, Vec2 pos)
	:Entity(parent, pos),
	_isOver(false),
	_image(nullptr)
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
	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 pos = _image->getPosition() + _parent->getPosition() - Vec2(CHUNKS_BLOCK_SIZE_HALF, CHUNKS_BLOCK_SIZE_HALF);
	auto size = _image->getContentSize();
	Rect box = Rect(pos.x, pos.y, size.width, size.height);

	// Test collision
	bool isOver = player->getBox().intersectsRect(box);

	// Check if can show shop stuff
	if (!_isOver && isOver)
	{
		MessageBox("gracz wlazl w szklep.\noj no bo to michal jest gupi", "sklep z ziolem");
	}

	// Store state
	_isOver = isOver;

	return false;
}
