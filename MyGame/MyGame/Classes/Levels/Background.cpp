
#include "Background.h"
#include "Level.h"

using namespace cocos2d;

Background::Background(Level* level)
	:_level(level)
{
	
}

Background::~Background()
{
	
}

bool Background::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("Textures/bg.png");
	if (texture == nullptr)
	{
		return false;
	}
	auto texSize = texture->getContentSize();

	Rect rect = Rect::ZERO;
	rect.size = texSize;
	
	if (!Sprite::initWithTexture(texture, rect))
	{
		return false;
	}

	setScaleX(visibleSize.width / texSize.width);
	setScaleY(visibleSize.height / texSize.height);

	setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	_level->addChild(this, -1000000);

	return true;
}
