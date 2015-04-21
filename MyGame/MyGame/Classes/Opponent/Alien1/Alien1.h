#pragma once

#include <string> 
#include "cocos2d.h"
#include "Opponent\Opponent.h"

using namespace cocos2d;

class Alien1 : public Opponent
{
public:
	Alien1(std::string name, Node& parent);
	~Alien1();


	void update(float dt) override;
};