
#include "Input.h"
#include <iostream>
#include <fstream>

using namespace std;

Input* Input::_instance = NULL;

void Input::attachTo(Node* node)
{
	// check if value won't change
	if (node == _node)
	{
		// Back
		return;
	}

	// check if detach
	if (node == NULL)
	{
		detachFrom();
		return;
	}

	// Cache data
	auto director = Director::getInstance();
	auto eventDispatcher = director->getEventDispatcher();

	// Set new parent node
	_node = node;
	
	// check if has keyboard events listner created
	if (_keyboard == NULL)
	{
		_keyboard = EventListenerKeyboard::create();
	}

	// attach events
	//_keyboard->onKeyPressed = CC_CALLBACK_2(Input::onKeyPressed, _node);
	//_keyboard->onKeyReleased = CC_CALLBACK_2(Input::onKeyReleased, node);
	eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboard, _node);

	// enable
	_keyboard->setEnabled(true);
}

void Input::detachFrom()
{
	_node = nullptr;
	if (_keyboard != NULL)
		_keyboard->setEnabled(false);
}

void Input::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

}

void Input::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

}
