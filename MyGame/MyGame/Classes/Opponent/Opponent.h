#pragma once

#include "cocos2d.h"
#include "../Objects/Entity.h"

using namespace cocos2d;

class Opponent : public Entity
{
public:

	enum State
	{
		Undefined = 0,
		PatrollingA,
		PatrollingB,
		Shooting,
		SearchForPlayer,
		FollowingPlayer,

		MAX
	};

protected:

	int _hp;
	Node* _node;
	State _state;

public:

	Opponent(Chunk* parent);
	~Opponent();

	virtual bool update(Level* level, float dt);

	Node* getSprite() const
	{
		return _node;
	}

	State GetState() const
	{
		return _state;
	}

	float getHP() const
	{
		return _hp;
	}

	Rect getBox() const
	{
		return _node ? _node->getBoundingBox() : Rect();
	}

	virtual void onDamage(float damage)
	{
		_hp -= damage;
	}

	// Get opponent movement speed
	virtual float getSpeed() const
	{
		return 200.0f;
	}
};