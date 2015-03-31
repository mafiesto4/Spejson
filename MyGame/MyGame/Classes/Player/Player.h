#pragma once

#include <string> 
#include "cocos2d.h"

using std::string; 
using namespace cocos2d; //xd 

class Player
{
	private:
		int _hp;
		string _name;
		Vec2 _position;
		Sprite* _image;

	public:
		Player(string name)
			:_hp(100), 
			_name(name),
			_position(Vec2::ZERO)
		{
			_image = Sprite::create("Textures/CloseNormal.png");
		}

		~Player()
		{
			CC_SAFE_RELEASE_NULL(_image);
		}


		void add(Node* parent)
		{
			parent->addChild(_image);
		}
};