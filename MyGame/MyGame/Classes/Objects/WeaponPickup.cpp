#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "../Levels/Chunk.h"
#include "WeaponPickup.h"
#include <SimpleAudioEngine.h>

using namespace cocos2d;

WeaponPickup::WeaponPickup(Chunk* parent, Vec2 pos, Weapon::Type type)
	:Entity(parent, pos),
	_image(nullptr),
	_type(type)
{
	char* path = nullptr;
	switch (_type)
	{
		case Weapon::Type::Pistol: path = "Textures/pistol.png"; break;
		case Weapon::Type::MachineGun: path = "Textures/mGun.png"; break;
		case Weapon::Type::Freezer: path = "Textures/freezer.png"; break;
		default: assert(0); break;
	}
	_image = Sprite::create(path);
	_image->setPosition(pos);
	_image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_parent->addChild(_image);

	setupAnim(2, 5, 2, 3);
}

WeaponPickup::~WeaponPickup()
{
	if (_image)
	{
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool WeaponPickup::update(Level* level, float dt)
{
	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 pos = _image->getPosition() + _parent->getPosition() - Vec2(CHUNKS_BLOCK_SIZE_HALF, CHUNKS_BLOCK_SIZE_HALF);
	auto size = _image->getContentSize();
	Rect box = Rect(pos.x, pos.y, size.width, size.height);

	// Test collision
	if (player->getBox().intersectsRect(box))
	{
		// Pickup weapon
		player->pickupWeapon(_type);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/pickupAmmo.wav");
		return true;
	}

	// Sprite animation
	animate(dt, _image);

	return false;
}
