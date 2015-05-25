#pragma once

#include <cocos2d.h>

#define BULLET_SIZE 10

struct Bullet
{
	bool ShotByPlayer;
	cocos2d::Vec2 Direction;
	float Damage;
	float Speed;
	float DistanceLeft;
	bool Luj;
	cocos2d::Sprite* Node;
};
