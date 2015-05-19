
#include <cocos2d.h>
#include "Ladder.h"
#include "../Game.h"
#include "../Levels/Chunk.h"

using namespace cocos2d;

Ladder::Ladder(const cocos2d::Vec2& location, int height)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/ladder.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	_sprite = Sprite::createWithTexture(texture, Rect(0, 0, CHUNKS_BLOCK_SIZE, height));
	_sprite->scheduleUpdateWithPriority(1000);
	_sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_sprite->setPosition(location);
}

void Ladder::update(class Chunk* parent, float dt)
{
	// Cache some data
	auto game = Game::getInstance();
	auto player = game->getPlayer();
	Rect playerRect = player->getBox();
	auto size = _sprite->getContentSize();
	auto ladderOrgin = _sprite->getPosition() + parent->getPosition();
	Rect ladderRect = Rect(ladderOrgin.x, ladderOrgin.y, size.width, size.height);

	// Make player rect smaller
	playerRect.origin += Vec2(playerRect.size.width * 0.2f, playerRect.size.height * 0.1f);
	playerRect.size = Vec2(playerRect.size.width * 0.6f, playerRect.size.height * 0.8f);

	if (ladderRect.intersectsRect(playerRect))
	{
		player->markLadderUse();
		_sprite->setColor(Color3B(0, 255, 0));
	}
	else
	{
		_sprite->setColor(Color3B(255, 255, 255));
	}
}
