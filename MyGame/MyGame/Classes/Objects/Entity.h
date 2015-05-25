#pragma once

#include "cocos2d.h"

class Level;
class Chunk;

// Single game entity interface
class Entity
{
protected:

	Chunk* _parent;
	cocos2d::Vec2 _spawnPos;
	float _animTime, _speed, _power;

	Entity(Chunk* parent)
		:_parent(parent)
	{
	}

	Entity(Chunk* parent, const cocos2d::Vec2& spawnPos)
		:_parent(parent),
		_spawnPos(spawnPos)
	{
		_animTime = (rand() % 100) / 100.0f;
		_power = 1.0f;
		_speed = 1.0f;
	}

public:

	// Virtual destructor
	virtual ~Entity() { }

	// Update entity
	// @returns True if entity wwants to be deleated
	virtual bool update(Level* level, float dt) { return false; }

protected:

	void setupAnim(int minSpeed, int maxSpeed, int minPower, int maxPower)
	{
		_speed = (float)(minSpeed + (rand() % (maxSpeed - minSpeed)));
		_power = (float)(minPower + (rand() % (maxPower - minPower)));
	}

	void animate(float dt, cocos2d::Node* node)
	{
		_animTime += dt * _speed;
		node->setPosition(cocos2d::Vec2(_spawnPos.x, _spawnPos.y + sin(_animTime) * _power));
	}
};
