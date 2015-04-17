#include <string>
#include "cocos2d.h"
#include "Opponent\Opponent.h"
#include "Game.h"
#include "Box2D\Box2D.h"

using namespace std;
using namespace cocos2d;
Opponent::Opponent(string name)
	:_hp(100),
	_name(name),
	_image(nullptr),
	_body(nullptr)
{

}

Opponent::~Opponent()
{
	CC_SAFE_RELEASE_NULL(_image);
}
