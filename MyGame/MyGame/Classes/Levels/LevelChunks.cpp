
#include "Level.h"
#include "..\Player\Player.h"
#include "..\Player\Weapons\Pistol\Pistol.h"
#include "..\Opponent\Alien1\Alien1.h"
#include "HUD\GameHUD.h"
#include "Game.h"
#include "HUD\DebugGUI.h"
#include "Utilities.h"
#include "Levels/Chunk.h"
#include "Levels/ChunkTypes/ChunkBaseFloor.h"
#include <assert.h>

using namespace cocos2d;

void Level::cleanAllChunks()
{
	// Dispose all chunks
	for (int i = 0; i < _chunks.size(); i++)
	{
		_chunks[i]->removeFromParentAndCleanup(true);
	}
	_chunks.clear();
	_rootChunk = nullptr;
	_closestPToPlayerChunk = nullptr;
}

void Level::setupInitialMap()
{
	// Clean rpevious state
	cleanAllChunks();

	// Spawn root chunk and spread it to generate enough chunks
	ChunkBaseFloor* floor = ChunkBaseFloor::create();
	addChunk(floor);
	for (int i = 0; i < CHUNKS_INITIAL_TREE_SIZE; i++)
	{
		floor->Spread(this);
	}
}

void Level::flushChunks()
{
	auto game = Game::getInstance();
	if (!game)
		return;

	auto player = game->getPlayer();
	if (!player)
		return;
	Vec2 playerPos = player->getPosition();

	// Validate root chunk
	if (!_rootChunk)
	{
		// Error
		assert(0);
	}

	// Delete old chunks
	float toDeleteLevel = _lava->getLevel() - 100.0f;
	if (_rootChunk->getPositionY() + _rootChunk->getContentSize().height < toDeleteLevel)
	{
		// Spread chunk to prevent errors if need to
		while (_chunks.size() < 3)
			_rootChunk->Spread(this);

		// Mark chunk to delete and get next root
		auto newRoot = _rootChunk->onDelete(this);
		if (_rootChunk == _closestPToPlayerChunk)
			_closestPToPlayerChunk = nullptr;
		if (!newRoot)
		{
			// Error
			assert(0);
		}

		// Remove chunk and set new root
		_chunks.erase(std::remove(_chunks.begin(), _chunks.end(), _rootChunk), _chunks.end());
		_rootChunk->removeFromParentAndCleanup(true);
		_rootChunk = newRoot;
	}

	// Find chunk that is near player
	float minDist = 999999999;
	_closestPToPlayerChunk = _rootChunk;
	for (vector<Chunk*>::reverse_iterator i = _chunks.rbegin(); i != _chunks.rend(); i++)
	{
		float dist = (*i)->getPosition().distance(playerPos);
		if (dist < minDist)
		{
			minDist = dist;
			_closestPToPlayerChunk = *i;
		}
	}

	// Check if need to generate new chunks
	Vec2 top = _closestPToPlayerChunk->getTreeTop();
	if (top.distance(playerPos) < 2000)
	{
		_closestPToPlayerChunk->Spread(this);
	}

	DebugGUI::setVal(5, "Chunks count", _chunks.size());
}

void Level::addChunk(Chunk* chunk)
{
	assert(chunk);

	_chunks.push_back(chunk);
	addChild(chunk, -10000);
	
	if (_rootChunk == nullptr)
		_rootChunk = chunk;
}
