#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../Levels/Chunk.h"
#include "WeaponPickup.h"

using namespace cocos2d;

WeaponPickup::WeaponPickup(Chunk* parent, Vec2 pos, Weapon::Type type)
	:Entity(parent),
	_type(type)
{
	char* path;
	switch (_type)
	{
		case Weapon::Type::Pistol: path = "Textures/pistol.png"; break;
		case Weapon::Type::MachineGun: path = "Textures/mGun.jpg"; break;
		case Weapon::Type::Freezer: path = "Textures/freezer.png"; break;
	}
	_image = Sprite::create(path);
	_image->setPosition(pos);
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	_parent->addChild(_image);
}

WeaponPickup::~WeaponPickup()
{
	if (_image)
	{
		_image->removeAllChildren();
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool WeaponPickup::update(Level* level, float dt)
{
	Vec2 posWS = _image->getPosition() + _parent->getPosition();
	auto player = Game::getInstance()->getPlayer();

	if (player->getBox().containsPoint(posWS))
	{
		player->selectWeapon(_type);
		return true;
	}

	return false;
}
