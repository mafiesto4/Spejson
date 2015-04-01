
#pragma once

#include <string> 
#include "cocos2d.h"
#include "Levels\Level1.h"

using namespace cocos2d;

class Player
{
	private:
		int _hp;
		std::string _name;
		Vec2 _position;
		Sprite* _image;
		EventListenerKeyboard* _keyboard;

	public:

		Player(std::string name);
		~Player();

		void setupForLevel(Level1* level);

private:

	void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};