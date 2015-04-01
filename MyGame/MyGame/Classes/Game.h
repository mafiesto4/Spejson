
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "Player\Player.h"
#include "Levels\Level1.h"

using namespace cocos2d;

class Game
{
private:

	Player* _player;

	Game(){};  // Private so that it can  not be called
	Game(Game const&){};            // copy constructor is private
	Game& operator=(Game const&){}; // assignment operator is private
	static Game* _instance;

public:

	static Game* getInstance()
	{
		if (!_instance)
			_instance = new Game;
		return _instance;
	}

	Player* getPlayer()
	{
		return _player;
	}

	void loadProfile(std::string profileName);
};
