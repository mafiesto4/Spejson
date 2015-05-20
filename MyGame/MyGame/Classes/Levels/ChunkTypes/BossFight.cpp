
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
	float platformY = CHUNKS_BLOCK_SIZE_HALF;
	float platformStart = 0;
	float platformWidth = (size.width * 0.5f);
	addPlatform(Vec2(platformStart, platformY), platformWidth);
	addPlatform(Vec2(platformStart + platformWidth + CHUNKS_BLOCK_SIZE, platformY), CHUNKS_BLOCK_SIZE * 4);

	// Spawn a boss
	const int bossSize = 100 + CHUNKS_BLOCK_SIZE_HALF;
	const int bossCenterPos = 150 + CHUNKS_BLOCK_SIZE_HALF;
	Vec2 p1 = Vec2(platformStart + bossSize, platformY + bossCenterPos);
	Vec2 p2 = Vec2(platformStart + platformWidth - bossSize, platformY + bossCenterPos);
	_entities.Add(new Boss(this, p1, p2));
}

void BossFight::onBossKilled()
{
	// Generate ladder for the player to allow him to go forward though the map
	auto size = getContentSize();
	float h = size.height - CHUNKS_BLOCK_SIZE * 2;
	Ladder* l = addLadder(Vec2(size.width * 0.5f + CHUNKS_BLOCK_SIZE * 3, CHUNKS_BLOCK_SIZE + h), h);

	// Animate it
	auto anim = MoveBy::create(5, Vec2(0, -h));
	l->getNode()->runAction(anim);
}