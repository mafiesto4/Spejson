#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Ammo.h"
#include "../Levels/Chunk.h"
#include <SimpleAudioEngine.h>
using namespace cocos2d;

Ammo::Ammo(Chunk* parent, Vec2 pos)
	:Entity(parent, pos),
	_image(nullptr)
{
	_image = Sprite::create("Textures/ammo1.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Textures/ammo.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("Textures/ammoA.plist");
	_image->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("ammoA"))));
	_image->setPosition(pos + Vec2(32, 0));
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	_parent->addChild(_image);

	setupAnim(3, 4, 4, 5);
}

Ammo::~Ammo()
{
	if (_image)
	{
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Ammo::update(Level* level, float dt)
{
	// Cache data
	auto player = Game::getInstance()->getPlayer();
	Vec2 pos = _image->getPosition() + _parent->getPosition() - Vec2(CHUNKS_BLOCK_SIZE_HALF, CHUNKS_BLOCK_SIZE_HALF);
	auto size = _image->getContentSize();
	Rect box = Rect(pos.x, pos.y, size.width, size.height);

	// Test collision
	if (player->getBox().intersectsRect(box))
	{
		// Pickup ammo
		player->onPickupAmmo();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/pickupAmmo.wav");
		return true;
	}

	// Sprite animation
	animate(dt, _image);

	return false;
}
