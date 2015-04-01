
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "HUD\GameHUD.h"

class Level1 : public cocos2d::Scene
{
public:

	GameHUD* _hud;


	virtual bool init();

	CREATE_FUNC(Level1);
};
