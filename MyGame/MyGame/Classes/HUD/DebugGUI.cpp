
#include "DebugGUI.h"
using namespace cocos2d;
using namespace std;

#define VALUES_CNT 7
string names[VALUES_CNT];
string values[VALUES_CNT];

bool DebugGUI::init()
{
	if (!Node::init())
	{
		return false;
	}

	// pobierz dane o rozmiarze okna i srodku widoku
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_label = Label::createWithTTF("", "Fonts/arial.ttf", 24);
	_label->setAnchorPoint(Vec2(1, 1));
	_label->setPosition(Vec2(origin.x + visibleSize.width - 10, origin.y + visibleSize.height - 10));
	addChild(_label);
	
	scheduleUpdate();

	return true;
}

void DebugGUI::update(float dt)
{
#if USE_DEBUG_LAYER
	string text;
	for (int i = 0; i < VALUES_CNT; i++)
	{
		text += names[i] + " = " + values[i] + '\n';
	}
	_label->setString(text);
#endif
}

void DebugGUI::setVal(int index, const std::string name, const unsigned int value)
{
#if USE_DEBUG_LAYER
	std::ostringstream ss;
	ss << value;
	setVal(index, name, ss.str());
#endif
}

void DebugGUI::setVal(int index, const std::string name, const bool value)
{
#if USE_DEBUG_LAYER
	std::ostringstream ss;
	if (value)
		ss << "true";
	else
		ss << "false";
	setVal(index, name, ss.str());
#endif
}

void DebugGUI::setVal(int index, const std::string name, const int value)
{
#if USE_DEBUG_LAYER
	std::ostringstream ss;
	ss << value;
	setVal(index, name, ss.str());
#endif
}

void DebugGUI::setVal(int index, const string name, const float value)
{
#if USE_DEBUG_LAYER
	std::ostringstream ss;
	ss << value;
	setVal(index, name, ss.str());
#endif
}

void DebugGUI::setVal(int index, const string name, const cocos2d::Vec2& value)
{
#if USE_DEBUG_LAYER
	std::ostringstream ss;
	ss << '(' << (int)value.x << ", " << (int)value.y << ')';
	setVal(index, name, ss.str());
#endif
}

void DebugGUI::setVal(int index, const string name, const char* value)
{
#if USE_DEBUG_LAYER
	assert(index >= 0 & index < VALUES_CNT);
	names[index] = name;
	values[index] = value;
#endif
}

void DebugGUI::setVal(int index, const string name, const string value)
{
#if USE_DEBUG_LAYER
	assert(index >= 0 & index < VALUES_CNT);
	names[index] = name;
	values[index] = value;
#endif
}
