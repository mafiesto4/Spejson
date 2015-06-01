
#pragma once

#include <cocos2d.h>

class Level;

class Background : public cocos2d::Sprite
{
private:

	Level* _level;

public:

	Background(Level* level);
	~Background();

	bool init() override;

	static Background* create(Level* level)
	{
		Background *pRet = new(std::nothrow) Background(level);
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
};
