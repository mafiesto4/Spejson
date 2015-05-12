
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include <math.h>
#include "Level.h"

// Chunks generator configuration stuff (hej sir, have you got some weed? cause i need it)
#define CHUNKS_DEBUG_PATH 0
#define CHUNKS_INITIAL_TREE_SIZE (4) // ile chunkow wygenerowac na start
#define CHUNKS_BLOCK_SIZE (64) // szerokosc/wysokosc pojedynczego bloczka
#define CHUNKS_WIDTH (2048) // szerokosc chunkow
#define CHUNKS_DEFAULT_HEIGHT (CHUNKS_JUMP_HEIGHT * 2) // wysokosc defaultowa chunkow
#define CHUNKS_JUMP_HEIGHT (256) // na jak wysoka platforme player moze wskoczyc
#define CHUNKS_JUMP_DISTANCE (320) // jak daleko player moze skoczyc w bok
#define CHUNKS_GAP_SIZE (CHUNKS_JUMP_HEIGHT - CHUNKS_BLOCK_SIZE) // rozmiar przestszeni pomiedzy kolejnymi poziomami platform
#define CHUNKS_BLOCK_SIZE_HALF (int)(CHUNKS_BLOCK_SIZE * 0.5f)
#define CHUNKS_CREATE(__TYPE__) \
static __TYPE__* create(Level* level, Chunk* parent, const Vec2& pos) \
{ \
	__TYPE__ *result = new(std::nothrow) __TYPE__(parent); \
	if (result && result->init()) \
	{ \
		result->autorelease(); \
		result->setPosition(pos); \
		level->addChunk(result); \
		result->calculatePathPoint(); \
		return result; \
	} \
	delete result; \
	result = nullptr; \
	return nullptr; \
}

// Describes single chunk with own logic and children
class Chunk : public cocos2d::LayerColor
{
	friend Chunk;

public:

	// Chunk type
	enum Type
	{
		Basic = 0,
		BaseFloor,
		ShoppingCentre,
		TwoWays,

		MAX
	};

protected:

	Chunk* _nextChunk, *_previousChunk;
	cocos2d::Vec2 _pathPoint;

public:

	Chunk(Chunk* prevChunk)
		:_nextChunk(nullptr),
		_previousChunk(prevChunk)
	{
	}

	~Chunk();
	
	// Init chunk
	bool init();

	// Gets chunk type
	inline virtual Type getType() = 0;

	// Returns path point for current chunk
	inline cocos2d::Vec2 getPathPoint() const
	{
		return _pathPoint;
	}

	// spread chunk; generate new chunks
	virtual void Spread(Level* level);

	// Called when chunk needs to be deleted
	// Returns next chunk in the tree
	virtual Chunk* onDelete(Level* level)
	{
		return _nextChunk;
	}

	// Get chunks tree top location
	virtual Vec2 getTreeTop()
	{
		if (_nextChunk)
			return _nextChunk->getTreeTop();
		return getPosition() + Vec2(0, getContentSize().height);
	}

public:

	// Set point to the chunks blocky grid
	static cocos2d::Vec2 AlignPointToGrid(cocos2d::Vec2 pathPoint)
	{
		auto leftBorder = CHUNKS_BLOCK_SIZE * cocos2d::Vec2(::floor((int)pathPoint.x / CHUNKS_BLOCK_SIZE), ::floor((int)pathPoint.y / CHUNKS_BLOCK_SIZE));
		auto rightBorder = leftBorder + Vec2(CHUNKS_BLOCK_SIZE, CHUNKS_BLOCK_SIZE);
		return ((pathPoint - leftBorder) < (rightBorder - pathPoint)) ? leftBorder : rightBorder;
	}

protected:

	virtual cocos2d::Size getDesireSize() = 0;
	virtual void calculatePathPoint();
	virtual void generate() = 0;
	Sprite* addPlatform(Vec2 location, float width);
	Sprite* addLadder(Vec2 location, float height);
	Sprite* addWall(char dir);

public:

	bool LoadChunkTemplates();
};