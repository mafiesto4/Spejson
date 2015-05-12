
#include <cocos2d.h>
#include <random>
#include "ChunkBasic.h"

using namespace cocos2d;
using namespace std;

Size ChunkBasic::getDesireSize()
{
	return Size(CHUNKS_WIDTH, CHUNKS_DEFAULT_HEIGHT);
}

void ChunkBasic::generate()
{
	// Cache size
	auto size = getContentSize();
	auto size2 = size * 0.5f;

	// Add walls
	addWall('a');
	addWall('d');

	// Convert path points to the node space
	Vec2 pos = getPosition();
	Vec2 startNS = Vec2(_previousChunk->getPathPoint().x - pos.x, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 middleNS = _pathPoint - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 endNS = Vec2(_pathPoint.x - pos.x, size.height - CHUNKS_BLOCK_SIZE_HALF);

#if CHUNKS_DEBUG_PATH
	while (startNS != middleNS)
	{
		addPlatform(startNS, CHUNKS_BLOCK_SIZE);

		if (startNS.x < middleNS.x)
			startNS.x += CHUNKS_BLOCK_SIZE;
		else if(startNS.x > middleNS.x)
			startNS.x -= CHUNKS_BLOCK_SIZE;
		else if(startNS.y < middleNS.y)
			startNS.y += CHUNKS_BLOCK_SIZE;
		else if(startNS.y > middleNS.y)
			startNS.y -= CHUNKS_BLOCK_SIZE;
	}
	while (startNS != endNS)
	{
		addPlatform(startNS, CHUNKS_BLOCK_SIZE);

		if (startNS.x < endNS.x)
			startNS.x += CHUNKS_BLOCK_SIZE;
		else if(startNS.x > endNS.x)
			startNS.x -= CHUNKS_BLOCK_SIZE;
		else if(startNS.y < endNS.y)
			startNS.y += CHUNKS_BLOCK_SIZE;
		else if (startNS.y > endNS.y)
			startNS.y -= CHUNKS_BLOCK_SIZE;
	}
	addPlatform(startNS, CHUNKS_BLOCK_SIZE);
#else

	// Generate horizontal platform
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
	int platformRightX = (int)(platformX + right - left);
	addPlatform(Vec2(platformX, middleNS.y), right - left);

	// Generate ladders
	int ladderH1 = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
	int ladderH2 = endNS.y - middleNS.y;
	addLadder(Vec2(startNS.x, 0), ladderH1);
	addLadder(Vec2(endNS.x, middleNS.y + CHUNKS_BLOCK_SIZE_HALF), ladderH2);

	// uncomment to use a non-deterministic seed
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 100000);

	// Generate random stuff in the chunk but do not cross the path
	for (int y = CHUNKS_BLOCK_SIZE + CHUNKS_BLOCK_SIZE_HALF; y < size.height; y += CHUNKS_BLOCK_SIZE)
	{
		for (int x = 0; x < size.width; x += CHUNKS_BLOCK_SIZE)
		{
			// Check the chance to generate a platform
			if (distr(gen) % 100 < 6)
			//if (rand() % 100 < 6)
			{
				// Randomize platform width
				int width = distr(gen) % 10 * CHUNKS_BLOCK_SIZE;

				// test vs chunk bounds
				width = min(x + width, (int)size.width) - x;

				// test vs main platform
				if (y - middleNS.y >= 0 && y - middleNS.y < 3 * CHUNKS_BLOCK_SIZE && x <= platformRightX)
					width = min(x + width, platformX) - x;

				// test vs lower ladder
				if (y <= ladderH1 + CHUNKS_BLOCK_SIZE * 2 && x <= startNS.x)
					width = min(x + width, (int)startNS.x) - x;

				// test vs higher ladder
				if (y >= middleNS.y && x <= endNS.x)
					width = min(x + width, (int)endNS.x) - x;

				// Check if path won't be crossed
				if (width < CHUNKS_BLOCK_SIZE)
					continue;

				// Add platform
				addPlatform(Vec2(x, y), width);

				// Ensure not to add platforms over that one on this row
				x += width;
			}
		}
	}

#endif
}
