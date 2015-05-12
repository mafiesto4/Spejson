
#include <cocos2d.h>
#include "Ladder.h"
#include "../Game.h"

using namespace cocos2d;

void Ladder::update(float dt)
{
	auto game = Game::getInstance();
	if (!game)
		return;

	auto player = game->getPlayer();
	if (!player)
		return;


}
