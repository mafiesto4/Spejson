#pragma once

#include <string> 
#include "cocos2d.h"
//#include "Levels\Level.h"

using namespace cocos2d;
class Level;


class Weapon {
protected:
	Sprite* _weaponImage;
	int _damage;
	int _fireRate;
	bool isAutomatic;


public:
	//Weapon();
	//virtual ~Weapon();


	virtual void update(float dt) = 0;

	void weaponSetup();

	void setupForNode(Node* level);
	Node* getSprite();
	virtual void onMouseDown(Vec2 pos, Level* level)
	{


	}
	virtual void onMouseUp(Vec2 pos, Level* level)
	{

	}


};