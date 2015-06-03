#pragma once

#include "cocos2d.h"

class DeadScreen : public cocos2d::Scene
{
private:

	int _score;

	DeadScreen(int score)
		:_score(score)
	{ }

public:

	bool init() override;

	static DeadScreen* create(int score)
	{
		DeadScreen *pRet = new(std::nothrow) DeadScreen(score);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

	void backCallback(Ref* pSender);
};
