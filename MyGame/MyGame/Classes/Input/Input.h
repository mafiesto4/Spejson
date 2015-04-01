
#pragma once

#include <cocos2d.h>
#include <string.h>

using namespace cocos2d;

class Input
{
private:

	Node* _node;
	EventListenerKeyboard* _keyboard;

	Input()
		:_node(nullptr),
		_keyboard(nullptr)
	{

	};

	Input(Input const&){};            // copy constructor is private
	Input& operator=(Input const&){}; // assignment operator is private
	static Input* _instance;

public:

	static Input* getInstance()
	{
		if (!_instance)
			_instance = new Input;
		return _instance;
	}

	// attach Input service to the cocs2d node
	void attachTo(Node* node);
	void detachFrom();

private:
	
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};
