
#pragma once

#include <string> 
#include "cocos2d.h"
#include "Levels\Level.h"
#include "Weapons/Weapon.h"

using namespace cocos2d;

#define USE_FREE_CAM 0
#define GOD_MODE 0
#define PLAYER_SCALE 2
#define GROUNDED_EPSOLON (10*PLAYER_SCALE)

class Player
{
private:

	int _hp;
	float _damageIntensity;
	std::string _name;
	Sprite* _image;
	PhysicsBody* _body;
	EventListenerKeyboard* _keyboard;
	EventListenerMouse* _mouse;
	Level* _level;
	Vec2 _prevPos;

	float _maxHeight;

	// Input
	bool _isPressingW, _isPressingS, _isPressingA, _isPressingD;

	// Movement
	bool _laddered;
	bool _immune;
	float _time;
	bool _wantsJump;
	bool _grounded;
	bool _doubleJumpFlag;
	bool _rightDirection;
	bool _isUsingLadder;
#if USE_FREE_CAM
	bool _useBoost;
#endif

	enum States
	{
		IDLE, 
		MOVE, 
		FALLING,
		JUMPING
	};
	States currentState;
	States stateToBe;
	Weapon::Type currentWeapon;

	// points and money $.$ xd
	int _score;
	int _cash;

	// Weapons stuff
	Weapon* _weapons[(int)Weapon::Type::MAX];
	Weapon* _selectedGun;

public:

	float fireRate;
	int lifes;

	bool _isMovingUp;

	bool OverLadder;

	Player(std::string name);
	~Player();

	void setupForLevel(Level* level, Vec2 spawnPoint);
	void update(float dt);

	void pickupWeapon(Weapon::Type type);
	void selectWeapon(Weapon::Type type);

	void onPickupAmmo();

	bool hasWeapon(Weapon::Type type) const
	{
		return _weapons[(int)type]->CanUse;
	}

	Node* getNode() const
	{
		return _image;
	}

	Vec2 getPosition() const
	{
		return _image->getPosition();
	}

	Rect getBox() const
	{
		auto box =  _image->getBoundingBox();
		//box.size.width *= PLAYER_SCALE;
		//box.size.height *= PLAYER_SCALE;
		//box.origin -= Vec2(box.size.width / 2, box.size.height / 2);
		return box;
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

	void onBuyMed()
	{
		_hp = 100;
	}

	void applyDamage(float damage);
	void onDamage(bool pushRight);

	void addScore(int addedScore)
	{
		_score += addedScore;
	}

	void setImmune()
	{
		_immune = true;
	}

	bool getLaddered() const
	{
		return _laddered;
	}

	bool getImmune() const
	{
		return _immune; 
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
	void onMouseDown(Event* event);
	void onMouseUp(Event* event);
	void onMouseScroll(Event* event);
};
