
#include <cocos2d.h>
#include "Ladder.h"
#include "../Game.h"
#include "../Levels/Chunk.h"

using namespace cocos2d;

Ladder::Ladder(Chunk* parent, const cocos2d::Vec2& location, int height)
	:Entity(parent, location),
	_isActive(false),
	_image(nullptr)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/ladder.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	_image = Sprite::createWithTexture(texture, Rect(0, 0, CHUNKS_BLOCK_SIZE, height));
	_image->scheduleUpdateWithPriority(1000);
	_image->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_image->setPosition(location);
	_parent->addChild(_image);
}

Ladder::~Ladder()
{
	if (_image)
	{
		_image->removeFromParentAndCleanup(true);
		_image = nullptr;
	}
}

bool Ladder::update(Level* level, float dt)
{
	// Cache some data
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	Rect playerRect = player->getBox();
	auto size = _image->getContentSize();
	auto ladderOrgin = _image->getPosition() + _parent->getPosition();
	Rect ladderRect = Rect(ladderOrgin.x, ladderOrgin.y, size.width, size.height);

	// Make player rect smaller
	playerRect.origin += Vec2(playerRect.size.width * 0.2f, playerRect.size.height * 0.1f);
	playerRect.size = Vec2(playerRect.size.width * 0.6f, playerRect.size.height * 0.8f);

	if (ladderRect.intersectsRect(playerRect))
	{
		if (player->getLaddered())
		{
			_isActive = true;
		}

		if (_isActive)
		{
			player->markLadderUse();
			_image->setColor(Color3B(0, 255, 0));
		}
	}
	else
	{
		_image->setColor(Color3B(255, 255, 255));
		_isActive = false;
	}

	return false;
}
