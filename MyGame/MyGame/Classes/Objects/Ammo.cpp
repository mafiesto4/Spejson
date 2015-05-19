#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Ammo.h"
#include "../Levels/Chunk.h"
using namespace cocos2d;

Ammo::Ammo(Chunk* parent, Vec2 pos)
	:Entity(parent)
{
	_value = 1;
	_image = Sprite::create("Textures/ammoPack1.png");
	_image->setPosition(pos + Vec2(32, 0));
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	_parent->addChild(_image);
}

Ammo::~Ammo()
{
	if (_image)
	{
		_image->removeFromPhysicsWorld();
		_image->removeAllChildren();
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Ammo::update(Level* level, float dt)
{
	Vec2 ammoPos = _image->getPosition() + _parent->getPosition();

	auto player = Game::getInstance()->getPlayer();
	if (player->getBox().containsPoint(ammoPos))
	{
		//player->addCash(_value);
		/// TODO add ammo
		return true;
	}

	return false;
}
