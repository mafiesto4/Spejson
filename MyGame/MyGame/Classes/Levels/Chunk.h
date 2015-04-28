
#pragma once

#include <vector>
#include <cocos2d.h>
#include <string.h>
#include "Level.h"

// Chunks generator configuration stuff (hej sir, have you got some weed? cause i need it)
#define CHUNKS_INITIAL_TREE_SIZE (4) // ile chunkow wygenerowac na start
#define CHUNKS_BLOCK_SIZE (64) // szerokosc/ysokosc pojedynczego bloczka
#define CHUNKS_WIDTH (2048) // szerokosc chunkow
#define CHUNKS_JUMP_HEIGHT (250) // na jak wysoka platforme player moze wskoczyc
#define CHUNKS_JUMP_DISTANCE (300) // jak daleko player moze skoczyc w bok
#define CHUNKS_GAP_SIZE (CHUNKS_JUMP_HEIGHT - CHUNKS_BLOCK_SIZE) // rozmiar przestszeni pomiedzy kolejnymi poziomami platform
#define CHUNKS_CREATE(__TYPE__, ...) \
static __TYPE__* create() \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
	if (pRet && pRet->init()) \
	{ \
		pRet->autorelease(); \
		pRet->Generate(##__VA_ARGS__); \
		return pRet; \
	} \
	delete pRet; \
	pRet = nullptr; \
	return nullptr; \
}
#define CHUNKS_INVALID_ENTRANCE (-1)

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
		TwoWays
	};

protected:

	int _seed;
	Chunk* _child;
	int _entrances[4];

public:

	Chunk()
		:_child(nullptr)
	{
	}

	~Chunk();

	virtual bool init();

	// Gets chunk type
	inline virtual Type getType() = 0;

	// Generate chunk interior
	virtual void Generate() = 0;

	// spread chunk; generate new chunks
	virtual void Spread(Level* level);

	// Called when chunk needs to be deleted
	// Returns its child
	virtual Chunk* onDelete(Level* level)
	{
		return _child;
	}

	// Get chunks tree top location
	virtual Vec2 getTreeTop()
	{
		if (_child)
			return _child->getTreeTop();
		return getPosition() + Vec2(0, getContentSize().height);
	}

protected:

	Sprite* addPlatform(Vec2 location, float width);
	Sprite* addWall(Vec2 location, float height);

public:

	bool LoadChunkTemplates();
};