
#include <cocos2d.h>
#include "ChunkBaseFloor.h"

using namespace cocos2d;

bool ChunkBaseFloor::init()
{
	// Base
	if (!Chunk::init())
	{
		return false;
	}

	// Resize
	setContentSize(Size(CHUNKS_WIDTH, CHUNKS_JUMP_HEIGHT));

	return true;
}

void ChunkBaseFloor::Generate()
{
	// Cache size
	Vec2 size = getContentSize();
	Vec2 size2 = size * 0.5f;

	// Add floor
	addPlatform(Vec2(size2.x, CHUNKS_BLOCK_SIZE / -2), size.x - CHUNKS_BLOCK_SIZE);

	// Add walls
	addWall(Vec2(0, size2.y), size.y);
	addWall(Vec2(size.x, size2.y), size.y);

	auto playerName = Label::createWithTTF("ssdscsc", "Fonts/Marker Felt.ttf", 20);
	auto playerNameSize = playerName->getContentSize();
	playerName->setAnchorPoint(Vec2::ZERO);
	playerName->setPosition(Vec2::ZERO);
	addChild(playerName);
}
