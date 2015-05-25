
#include <cocos2d.h>
#include "BossFight.h"
#include "../../Game.h"
#include "../../Opponent/Boss/Boss.h"
#include "../../Objects/Ladder.h"

using namespace cocos2d;

Vec2 BossFight::getPathPointForPrevChunk() const
{
	return _pathPointForNext;
}

Size BossFight::getDesireSize()
{
	return Size(CHUNKS_WIDTH, CHUNKS_JUMP_HEIGHT * 4);
}

void BossFight::calculatePathPoint()
{
	auto size = getContentSize();
	auto pos = getPosition();
	auto pathPoint = Vec2(size.width * 0.5f + CHUNKS_BLOCK_SIZE * 3, 0);
	_pathPoint = pos + AlignPointToGrid(pathPoint);
	_pathPointForNext = pos + AlignPointToGrid(Vec2(pathPoint.x, size.height * 0.9));
	_pathPoint = pos + AlignPointToGrid(Vec2(pathPoint.x, 0));;
}

void BossFight::generate()
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

	Vec2 startNS = Vec2(prevPP.x - pos.x, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 middleNS = _pathPoint - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);

	// Generate ladder
	int ladderH1 = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
	float ladderH = _pathPoint.y - prevPP.y;
	addLadder(Vec2(startNS.x, ladderH1 - ladderH), ladderH);

	// Generate platforms for fight
	const float platformY = CHUNKS_BLOCK_SIZE_HALF;
	const float platformStart = 0;
	float platform1Width = (size.width * 0.5f);
	float platform2Width = size.width - platform1Width - CHUNKS_BLOCK_SIZE;
	addPlatform(Vec2(platformStart, platformY), platform1Width);
	addPlatform(Vec2(platformStart + platform1Width + CHUNKS_BLOCK_SIZE, platformY), platform2Width);

	// Spawn a boss
	const int bossSize = 100 + CHUNKS_BLOCK_SIZE_HALF;
	const int bossCenterPos = 64 + CHUNKS_BLOCK_SIZE_HALF;
	Vec2 p1 = Vec2(platformStart + bossSize, platformY + bossCenterPos);
	Vec2 p2 = Vec2(platformStart + platform1Width + CHUNKS_BLOCK_SIZE + platform2Width - bossSize, p1.y);
	_entities.Add(new Boss(this, p1, p2));

	// Add blocking platform
	_blockingPlatform = addPlatform(Vec2(0, size.height - CHUNKS_BLOCK_SIZE_HALF - CHUNKS_BLOCK_SIZE), size.width);
}

void BossFight::onBossKilled()
{
	// Remove blocking platform
	removePlatform(_blockingPlatform);

	// Generate ladder for the player to allow him to go forward though the map
	auto size = getContentSize();
	float h = size.height - CHUNKS_BLOCK_SIZE * 2;
	Ladder* l = addLadder(Vec2(size.width * 0.5f + CHUNKS_BLOCK_SIZE * 3, CHUNKS_BLOCK_SIZE + h), h);

	// Animate it
	auto anim = MoveBy::create(5, Vec2(0, -h));
	l->getNode()->runAction(anim);
}
