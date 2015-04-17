#pragma once

#include <string> 
#include "cocos2d.h"
#include "Player\Weapons\Weapon.h"
#include "Levels\Level1.h"

using namespace cocos2d;

class Pistol : public Weapon 
{



	public:
		void setupForPlayer(Node* level);
};