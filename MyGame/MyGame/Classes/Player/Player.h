
#pragma once

#include <string> 
#include "cocos2d.h"
#include "Levels\Level.h"

using namespace cocos2d;

class Player
{
	private:
		int _hp;
		std::string _name;
		Sprite* _image;
		PhysicsBody* _body;
		EventListenerKeyboard* _keyboard;
		Label* label;
	

		// Movement
		bool _wantsJump;
		bool _wantsMoveLeft;
		bool _wantsMoveRight;
		bool _grounded;
		bool _rightDirection=true;
		
		//points and money $.$ xd

		int _score;
		int _cash;

	public:

		Weapon* _bron;
		float fireRate=1;

		Player(std::string name);
		~Player();


		bool ifMovingRight()
		{
			if (_rightDirection) return true;
			return false;
		}

		void setupForLevel(Level* level);
		void update(float dt);
		bool onContactBegin(PhysicsContact& contact);
		void onContactSeparate(PhysicsContact& contact);
		void onDamage(float _damage);

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

		void addCash(int _value)
		{
			_cash += _value;
		}

private:

	void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};