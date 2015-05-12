#include <string>
#include "cocos2d.h"
#include "Game.h"
#include "Box2D\Box2D.h"
#include "Objects/Coin.h"
using namespace cocos2d;

Coin::Coin(Node& level, Vec2 _pos)
{

	_value = 1;
	_image = Sprite::create("Textures/super_mario_coin.png");
	_image->setPosition(_pos);
	_image->setAnchorPoint(Vec2(0.5, 0.5));
	level.addChild(_image);

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


void Coin::setupForLevel()
{




}

Node* Coin::getSprite()
{
	return _image;
}