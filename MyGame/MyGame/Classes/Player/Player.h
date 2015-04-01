#pragma once

#include <string> 
#include "cocos2d.h"

class Player
{
	private:
		int _hp;
		std::string _name;
		cocos2d::Vec2 _position;
		cocos2d::Sprite* _image;

	public:

		Player(std::string name);
		~Player();

		void add(cocos2d::Node* parent);
};