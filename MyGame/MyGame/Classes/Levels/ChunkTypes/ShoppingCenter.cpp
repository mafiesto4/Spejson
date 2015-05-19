
#include <cocos2d.h>
#include "ShoppingCenter.h"
#include "../../Game.h"
#include "../../Objects/Shop.h"

using namespace cocos2d;

Vec2 ShoppingCenter::getPathPointForPrevChunk() const
{
	return _pathPointForPrev;
}

Size ShoppingCenter::getDesireSize()
{
	return Size(CHUNKS_WIDTH, CHUNKS_JUMP_HEIGHT * 2);
}

void ShoppingCenter::calculatePathPoint()
{
	// Right in the middle, just like that!
	auto size = getContentSize();
	auto pos = getPosition();
	auto pathPoint = Vec2(size.width * 0.5f, 0);
	_pathPoint = pos + AlignPointToGrid(pathPoint);
	_pathPointForPrev = _pathPoint;
}

void ShoppingCenter::generate()
{
	// Cache size
	auto size = getContentSize();
	auto size2 = size * 0.5f;

	// Add walls
	addWall('a');
	addWall('d');

	// Convert path points to the node space
	Vec2 pos = getPosition();
	Vec2 prevPP = _previousChunk->getPathPointForPrevChunk();
	auto prevSize = _previousChunk->getContentSize();

	Vec2 startNS = Vec2(prevPP.x - pos.x, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 middleNS = _pathPoint - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 endNS = Vec2(_pathPoint.x - pos.x, size.height - CHUNKS_BLOCK_SIZE_HALF);

	Vec2 prevMiddleNS = prevPP - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 prevEndNS = Vec2(prevPP.x - pos.x, prevSize.height - CHUNKS_BLOCK_SIZE_HALF);

	// Generate horizontal platform (only if need to since path point can be on vertical ladders path)
	float left, right;
	int platformX;
	if (startNS.x < middleNS.x)
	{
		left = startNS.x;
		right = middleNS.x;
		platformX = left + CHUNKS_BLOCK_SIZE;
	}
	else
	{
		left = middleNS.x;
		right = startNS.x;
		platformX = left;
	}
	int width = right - left;
	if (width > CHUNKS_BLOCK_SIZE_HALF)
	{
		int platformRightX = (int)(platformX + width);
		addPlatform(Vec2(platformX, middleNS.y), width);
	}

	// Generate ladders
	int ladderH1 = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
	int ladderH2 = endNS.y - middleNS.y;
	int ladderStart2 = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
	float ladderH = _pathPoint.y - prevPP.y;
	addLadder(Vec2(startNS.x, ladderH1 - ladderH), ladderH);

	// Check if generate on left side and calculate shop location
	float shopsY = CHUNKS_BLOCK_SIZE * 2;
	float platformStart, platformWidth;
	if (startNS.x < endNS.x)
	{
		platformStart = right + CHUNKS_BLOCK_SIZE;
		platformWidth = size2.width - CHUNKS_BLOCK_SIZE;
	}
	else
	{
		platformStart = 0;
		platformWidth = left;
	}

	// Generate platform for shops
	addPlatform(Vec2(platformStart, shopsY), platformWidth);

	// Generate shops
	Vec2 shopPos = Vec2(platformStart + platformWidth / 2 - CHUNKS_BLOCK_SIZE, shopsY + CHUNKS_BLOCK_SIZE + CHUNKS_BLOCK_SIZE_HALF);
	_entities.Add(new Shop(this, shopPos));
}
