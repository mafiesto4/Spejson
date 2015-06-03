#pragma once

#include "cocos2d.h"

class ScoresScene : public cocos2d::Scene
{
public:

	bool init() override;

	CREATE_FUNC(ScoresScene);

	void backCallback(Ref* pSender);
};
