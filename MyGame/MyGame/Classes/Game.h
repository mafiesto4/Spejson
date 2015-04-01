
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "Player\Player.h"

using namespace cocos2d;

class Game
{
private:

	Player* _player;

	Game();

public:
	
	static Game& getSingleton()
	{
		static Game singleton;
		return singleton;
	}

	Player* getPlayer()
	{
		return _player;
	}

	void loadProfile(std::string profileName);
};
