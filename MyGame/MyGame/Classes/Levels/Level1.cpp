
#include "Level1.h"
#include "..\Player\Player.h"
#include "HUD\GameHUD.h"
#include "Game.h"

using namespace cocos2d;

bool Level1::init()
{
	// inicjalizacja sceny z cocos'a
	if (!Scene::init())
	{
		return false;
	}

	// stworz Head Up Display
	_hud = GameHUD::create();
	addChild(_hud);

	// setup player
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	player->setupForLevel(this);

	return true;
}
