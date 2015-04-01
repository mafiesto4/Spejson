
#include <string>
#include "cocos2d.h"
#include "Player.h"
#include "Game.h"

using namespace std;
using namespace cocos2d;

Player::Player(string name)
	:_hp(100),
	_name(name),
	_position(Vec2(50, 50)),
	_keyboard(nullptr)
{
	_image = Sprite::create("Textures/CloseNormal.png");
	_image->setPosition(_position);
}

Player::~Player()
{
	CC_SAFE_RELEASE_NULL(_image);
	CC_SAFE_RELEASE_NULL(_keyboard);
}

void Player::setupForLevel(Level1* level)
{
	// sprawdz czy keyboard listner zstal stworzony
	if (&_keyboard != NULL)
	{
		// Initializing and binding 
		auto keyboard = EventListenerKeyboard::create();
		keyboard->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
		keyboard->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
		auto director = Director::getInstance();
		auto eventDispatcher = director->getEventDispatcher();

		eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, level);

	}
	
	// add node to the level
	level->addChild(_image);
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	{
		float y = _image->getPositionY();
		y += 5;
		_image->setPositionY(y);
	}
	break;
	case EventKeyboard::KeyCode::KEY_S:
	{
		float y = _image->getPositionY();
		y -= 5;
		_image->setPositionY(y);
	}
	break;
	case EventKeyboard::KeyCode::KEY_D:
	{
		float x = _image->getPositionX();
		x += 5;
		_image->setPositionX(x);
	}
	break;
	case EventKeyboard::KeyCode::KEY_A:
	{
		float x = _image->getPositionX();
		x -= 5;
		_image->setPositionX(x);
	}
	break;
	default:
		break;
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

}