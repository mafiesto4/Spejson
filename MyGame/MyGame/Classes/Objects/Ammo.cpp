#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Ammo.h"
using namespace cocos2d;

Ammo::Ammo(Node& level, Vec2 _pos)
{

	_value = 1;
	_image = Sprite::create("Textures/Bullets-icon.png");
	_image->setPosition(_pos);
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	level.addChild(_image);

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


void Ammo::setupForLevel()
{




}

Node* Ammo::getSprite()
{
	return _image;
}