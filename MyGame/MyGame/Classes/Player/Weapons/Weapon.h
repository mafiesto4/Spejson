#pragma once

#include <string> 
#include "cocos2d.h"

using namespace cocos2d;

class Weapon {
	protected:
		Sprite* _weaponImage;
		int _damage;
		int _fireRate;
		bool isAutomatic;


	public:
		void shoot();
		void weaponSetup();

		Weapon() :_weaponImage(nullptr){}
};