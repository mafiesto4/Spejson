
#include <cocos2d.h>
#include <random>
#include "ChunkBasic.h"
#include "../../Opponent/Alien1/Alien1.h"
#include "../../Opponent/Shooting/Shooting.h"

using namespace cocos2d;
using namespace std;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(0, 100000);

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

	// Try to spawn a fly
	tryToSpawnAFly();

	// Convert path points to the node space
	Vec2 pos = getPosition();
	Vec2 prevPP = _previousChunk->getPathPointForPrevChunk();
	auto prevSize = _previousChunk->getContentSize();

	Vec2 startNS = Vec2(prevPP.x - pos.x, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 middleNS = _pathPoint - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 endNS = Vec2(_pathPoint.x - pos.x, size.height - CHUNKS_BLOCK_SIZE_HALF);

	Vec2 prevMiddleNS = prevPP - pos + Vec2(0, CHUNKS_BLOCK_SIZE_HALF);
	Vec2 prevEndNS = Vec2(prevPP.x - pos.x, prevSize.height - CHUNKS_BLOCK_SIZE_HALF);

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
	int width = right - left;
	int platformRightX = (int)(platformX + width);
	addPlatform(Vec2(platformX, middleNS.y), width);

	// Try to spawn alien
	if (width > 3 * CHUNKS_BLOCK_SIZE && distr(gen) % 100 < 90)
	{
		float ppy = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
		Vec2 p1 = Vec2(platformX + CHUNKS_BLOCK_SIZE * 2, ppy);
		Vec2 p2 = Vec2(platformX + width - CHUNKS_BLOCK_SIZE, ppy);
		Opponent* op;
		if (distr(gen) % 2 == 0)
		{
			op = new Alien1(this, p1, p2);
		}
		else
		{
			op = new Shooting(this, p1, p2);
		}
		_entities.Add(op);
	}

	// Generate ladders
	int ladderH1 = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
	int ladderStart2 = middleNS.y + CHUNKS_BLOCK_SIZE_HALF;
	float ladderH = _pathPoint.y - prevPP.y;
	addLadder(Vec2(startNS.x, ladderH1 - ladderH), ladderH);

	// Generate random stuff in the chunk but do not cross the path
	for (int y = CHUNKS_BLOCK_SIZE_HALF; y < size.height; y += CHUNKS_BLOCK_SIZE)
	{
		for (int x = 0; x < size.width; x += CHUNKS_BLOCK_SIZE)
		{
			// Check if its a platform or a ladder
			if ((y == middleNS.y && x >= platformX && x <= platformRightX)
				|| (x == middleNS.y && x <= endNS.x)
				|| (y <= ladderH1 && x == startNS.x)
				|| (y >= ladderStart2 && x == endNS.x))
				continue;

			Vec2 p(x, y);

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
				addPlatform(p, width);

				// Ensure not to add platforms over that one on this row
				x += width;
			}
			// Check if can generate entity
			else
			{
				int rand = distr(gen) % 200;
				p.x += CHUNKS_BLOCK_SIZE_HALF;
				switch (rand)
				{
					// Coin
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					{
						addCoin(p);
					}
					break;

					// Ammo
					case 6:
					{
						addAmmo(p);
					}
					break;
				}
			}
		}
	}

#endif
}
