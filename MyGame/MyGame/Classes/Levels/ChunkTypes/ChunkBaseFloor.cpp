
#include <cocos2d.h>
#include "ChunkBaseFloor.h"
#include "../../Game.h"
#include "../../Objects/WeaponPickup.h"

using namespace cocos2d;

Size ChunkBaseFloor::getDesireSize()
{
	return Size(CHUNKS_WIDTH, CHUNKS_JUMP_HEIGHT);
}

void ChunkBaseFloor::generate()
{
	// Cache size
	auto size = getContentSize();
	auto size2 = size * 0.5f;

	// Add walls
	addWall('a');
	addWall('d');
	addWall('_');

	// Convert path points to the node space
	Vec2 pos = getPosition();
	Vec2 startNS = Vec2(-pos.x, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 middleNS = _pathPoint - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 endNS = Vec2(_pathPoint.x - pos.x, size.height - CHUNKS_BLOCK_SIZE_HALF);

#if CHUNKS_DEBUG_PATH
	while (startNS != middleNS)
	{
		addPlatform(startNS, CHUNKS_BLOCK_SIZE);

		if (startNS.x < middleNS.x)
			startNS.x += CHUNKS_BLOCK_SIZE;
		else if (startNS.x > middleNS.x)
			startNS.x -= CHUNKS_BLOCK_SIZE;
		else if (startNS.y < middleNS.y)
			startNS.y += CHUNKS_BLOCK_SIZE;
		else if (startNS.y > middleNS.y)
			startNS.y -= CHUNKS_BLOCK_SIZE;
	}
	while (startNS != endNS)
	{
		addPlatform(startNS, CHUNKS_BLOCK_SIZE);

		if (startNS.x < endNS.x)
			startNS.x += CHUNKS_BLOCK_SIZE;
		else if (startNS.x > endNS.x)
			startNS.x -= CHUNKS_BLOCK_SIZE;
		else if (startNS.y < endNS.y)
			startNS.y += CHUNKS_BLOCK_SIZE;
		else if (startNS.y > endNS.y)
			startNS.y -= CHUNKS_BLOCK_SIZE;
	}
	addPlatform(startNS, CHUNKS_BLOCK_SIZE);
#else

	// Generate ladder
	addLadder(Vec2(endNS.x, 0), size.height);

#endif

	// ³ejpons tu pikap
	_entities.Add(new WeaponPickup(this, Vec2(600, 50), Weapon::Type::MachineGun));
	_entities.Add(new WeaponPickup(this, Vec2(900, 50), Weapon::Type::Freezer));
}
