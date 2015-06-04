
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "Player\Player.h"
#include "Levels\Level.h"

using namespace cocos2d;
using namespace std;

// Physi Bodies Tags
#define PHYSICS_TAG_GROUND 40
#define PHYSICS_TAG_PLAYER 69
#define PHYSICS_TAG_STATIC 70

typedef unsigned char byte;

class Game
{
private:

	Player* _player;

	Game();
	Game(Game const&){};            // copy constructor is private
	Game& operator=(Game const&){}; // assignment operator is private
	static Game* _instance;

public:

	static Game* getInstance()
	{
		if (!_instance)
			_instance = new Game();
		return _instance;
	}

	Player* getPlayer()
	{
		return _player;
	}

	void play();
};
