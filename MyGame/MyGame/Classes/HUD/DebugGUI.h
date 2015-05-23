
#pragma once

#include "cocos2d.h"
#include <string.h>

#define USE_DEBUG_LAYER 0

class DebugGUI : public cocos2d::Node
{
private:

	cocos2d::Label* _label;

public:

	virtual bool init();

	CREATE_FUNC(DebugGUI);

	virtual void update(float dt);
	
	static void setVal(int index, const std::string name, const unsigned int value);
	static void setVal(int index, const std::string name, const bool value);
	static void setVal(int index, const std::string name, const int value);
	static void setVal(int index, const std::string name, const float value);
	static void setVal(int index, const std::string name, const cocos2d::Vec2& value);
	static void setVal(int index, const std::string name, const std::string value);
	static void setVal(int index, const std::string name, const char* value);
};
