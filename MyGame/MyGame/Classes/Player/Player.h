
#pragma once

#include <string> 
#include "cocos2d.h"
#include "Levels\Level.h"

using namespace cocos2d;

#define USE_FREE_CAM 0
#define GOD_MODE 0

class Player
{
	private:
		int _hp;
		std::string _name;
		Sprite* _image;
		PhysicsBody* _body;
		EventListenerKeyboard* _keyboard;
		Label* playerPosLabel;

		// Movement
		bool _laddered = false;  //czy ma wciœniête "w" w polu drabiny
		bool _immune = false;
		float _time = 0;
		bool _wantsJump;
		bool _wantsMoveLeft;
		bool _wantsMoveRight;
		bool _grounded;
		bool _rightDirection = true;
		bool _isUsingLadder = false;
		bool _wantsDown;
#if USE_FREE_CAM
		bool _useBoost;
#endif

		//points and money $.$ xd
		int _score;
		int _cash;

	public:

		Weapon* _bron;
		float fireRate = 1;

		Player(std::string name);
		~Player();

		bool ifMovingRight() const
		{
			if (_rightDirection) return true;
			return false;
		}

		void setupForLevel(Level* level, Vec2 spawnPoint);
		void update(float dt);
		bool onContactBegin(PhysicsContact& contact);
		void onContactSeperate(PhysicsContact& contact);

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
#if !USE_FREE_CAM
			_hp -= damage;
			if (_hp <= 0)
			{
				_hp = 0;
				MessageBox("Player zostal zabity!", "SMIERC");
				Director::getInstance()->end();
			}
#endif
		}

		void markLadderUse();
		
		void addCash(int _value)
		{
			_cash += _value;
		}

		int Cash() const
		{
			return _cash;
		}

		void onDamage(bool pushRight);

		void setImmune()
		{
			_immune = true;
		}

		void clearLaddered()
		{
			_laddered = false;
		}


		bool getImmune() { return _immune; }
		bool getLaddered() { return _laddered; }

private:

	void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};