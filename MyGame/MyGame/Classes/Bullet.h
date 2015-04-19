#pragma once

#include <cocos2d.h>

struct Bullet
{
	bool ShotByPlayer;
	cocos2d::Vec2 Position;
	cocos2d::Vec2 Direction;
	float Damage;
	float Speed;
	float DistanceLeft;
};
