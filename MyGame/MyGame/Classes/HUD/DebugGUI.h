
#pragma once

#include "cocos2d.h"
#include <string.h>

class DebugGUI : public cocos2d::Node
{
private:

	cocos2d::Label* _label;

public:

	virtual bool init();

	CREATE_FUNC(DebugGUI);

	virtual void update(float dt);

	static void setVal(int index, std::string name, const cocos2d::Vec2&  value);
	static void setVal(int index, std::string name, std::string value);
};
