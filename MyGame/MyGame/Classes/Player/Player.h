
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
		Sprite* _image;
		PhysicsBody* _body;
		EventListenerKeyboard* _keyboard;
		
		// Movement
		bool _wantsJump;
		bool _wantsMoveLeft;
		bool _wantsMoveRight;

	public:

		Player(std::string name);
		~Player();

		void setupForLevel(Level1* level);
		void update(float dt);

		Vec2 getPosition() const
		{
			return _image ? _image->getPosition() : Vec2::ZERO;
		}

private:

	void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};