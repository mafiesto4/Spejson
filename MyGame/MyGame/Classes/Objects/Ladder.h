#pragma once

#include <cocos2d.h>
#include "../Levels/Chunk.h"

class Ladder : public cocos2d::Sprite
{
private:

	virtual void update(float dt);

public:

	static cocos2d::Sprite* Create(const cocos2d::Vec2& location, int height)
	{
		// Load texture and ensure its repeating
		Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/ladder.png");
		Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
		texture->setTexParameters(tp);

		// Create sprite
		auto ladder = Ladder::createWithTexture(texture, Rect(0, 0, CHUNKS_BLOCK_SIZE, height));
		ladder->scheduleUpdateWithPriority(1000);
		ladder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		ladder->setPosition(location);
		return ladder;
	}
};
