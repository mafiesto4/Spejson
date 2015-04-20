
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

		Node* getNode() const
		{
			return _image;
		}

		Vec2 getPosition() const
		{
			return _image ? _image->getPosition() : Vec2::ZERO;
		}

		Rect getBox() const
		{
			return _image ? _image->getBoundingBox() : Rect();
		}

		void applyDamage(float damage)
		{
			_hp -= damage;
			if (_hp <= 0)
			{
				_hp = 0;
				MessageBox("Player zostal zabity!", "SMIERC");
			}
		}

private:

	void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};