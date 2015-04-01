
#include <string>
#include "cocos2d.h"
#include "Player.h"

using namespace std;
using namespace cocos2d;

Player::Player(string name)
	:_hp(100),
	_name(name),
	_position(Vec2::ZERO)
{
	_image = Sprite::create("Textures/CloseNormal.png");
}

Player::~Player()
{
	CC_SAFE_RELEASE_NULL(_image);
}

void Player::add(Node* parent)
{
	parent->addChild(_image);
}