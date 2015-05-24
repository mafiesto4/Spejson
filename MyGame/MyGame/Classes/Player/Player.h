
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
	EventListenerMouse* _mouse;
	Label* playerPosLabel;
	Level* _level;

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

	// points and money $.$ xd
	int _score;
	int _cash;

	// Weapons stuff
	Weapon* _weapons[(int)Weapon::Type::MAX];
	Weapon* _selectedGun;

public:

	float fireRate = 1;
	int lifes = 1;

	Player(std::string name);
	~Player();

	void setupForLevel(Level* level, Vec2 spawnPoint);
	void update(float dt);
	bool onContactBegin(PhysicsContact& contact);
	void onContactSeperate(PhysicsContact& contact);

	void pickupWeapon(Weapon::Type type);
	void selectWeapon(Weapon::Type type);

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

	bool ifMovingRight() const
	{
		return _rightDirection;
	}

	void addCash(int _value)
	{
		_cash += _value;
	}

	int Cash() const
	{
		return _cash;
	}

	void onDamage(bool pushRight);

	void addScore(int addedScore)
	{
		_score += addedScore;
	}

	void setImmune()
	{
		_immune = true;
	}

	void clearLaddered()
	{
		_laddered = false;
	}

	bool getImmune() const
	{
		return _immune; 
	}

	bool getLaddered() const
	{
		return _laddered; 
	}

	int getScore() const
	{
		return _score;
	}
	
	int getCash() const
	{
		return _cash;
	}

	int getHP() const
	{
		return _hp;
	}

	Weapon* getGun() const
	{
		return _selectedGun;
	}

private:

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onMouseScroll(Event* event);
};
