
#include <cocos2d.h>
#include "ChunkBasic.h"
#include "../ChunkTemplate.h"

using namespace cocos2d;

bool ChunkBasic::init()
{
	// Base
	if (!Chunk::init())
	{
		return false;
	}

	return true;
}

void ChunkBasic::Generate()
{
	// Get predefined chunk template
	int index = _seed % ChunkTemplate::ChunkBasic.size();
	ChunkTemplate* ct = ChunkTemplate::ChunkBasic[index];

	// Resize
	setContentSize(Size(CHUNKS_WIDTH, (ct->Height - 1) * CHUNKS_JUMP_HEIGHT));
	Vec2 size = getContentSize();
	Vec2 size2 = size * 0.5f;

	// Generate chunk structure
	int offsetX, offsetY = size.y;
	for (int y = 0; y < ct->Height; y++)
	{
		offsetX = 0;
		for (int x = 0; x < ct->Width; x++)
		{
			switch (ct->Get(x,y))
			{
				case CT_FLOOR:
				case CT_ENTER:
				case CT_QUIT:
				{
					addPlatform(Vec2(x * CHUNKS_BLOCK_SIZE * 3, offsetY), CHUNKS_BLOCK_SIZE * 3);
				}
				break;
				default:
					break;
			}
		}
		offsetY -= CHUNKS_JUMP_HEIGHT;
	}
}
