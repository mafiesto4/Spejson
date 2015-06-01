
#pragma once

#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ChunkBaseFloor : public Chunk
{
public:

	ChunkBaseFloor(Chunk* prevChunk, Level* level)
		:Chunk(prevChunk, level)
	{ }

	~ChunkBaseFloor()
	{ }

	CHUNKS_CREATE(ChunkBaseFloor);

private:

	Size getDesireSize() override;
	void generate() override;
};
