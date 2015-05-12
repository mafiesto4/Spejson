
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "../Chunk.h"

class ChunkBaseFloor : public Chunk
{
public:

	ChunkBaseFloor(Chunk* prevChunk)
		:Chunk(prevChunk)
	{ }

	~ChunkBaseFloor()
	{ }

	CHUNKS_CREATE(ChunkBaseFloor);

	inline Type getType() override
	{
		return Type::BaseFloor;
	}

private:

	cocos2d::Size getDesireSize() override;
	void generate() override;
};
