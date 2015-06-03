
#include <cocos2d.h>
#include <vector>

#include "../Types/List.h"
#include "Level.h"
#include "Chunk.h"

#include "ChunkTypes/ChunkBasic.h"
#include "ChunkTypes/ShoppingCenter.h"
#include "ChunkTypes/PreBossFight.h"

#include "../Objects/Ladder.h"
#include "../Objects/Coin.h"
#include "../Objects/Ammo.h"

#include "../Opponent/Fly/Fly.h"

using namespace cocos2d;

//bool noTexLoaded = true;

Chunk::~Chunk()
{
	_platforms.Clear();
	_walls.clear();
	_entities.ClearDelete();
}

bool Chunk::init()
{
	// Base
	if (!Layer::init())
	//if (!LayerColor::initWithColor(Color4B(rand() % 255, rand() % 255, rand() % 255, 255)))
	/*Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("Textures/bg.png");
	if (!texture)
	{
		return false;
	}
	if (noTexLoaded)
	{
		Texture2D::TexParams params{ GL_POINT, GL_POINT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
		texture->setTexParameters(params);
		noTexLoaded = false;
	}
	Rect rect = Rect::ZERO;
	rect.size = texture->getContentSize();
	if (!Sprite::initWithTexture(texture, rect, false))*/
	{
		return false;
	}

	// Set size
	setContentSize(getDesireSize());

	// Calculate path point
	calculatePathPoint();

	// Good
	return true;
}

void Chunk::update(Level* level, float dt)
{
	// Update all entities
	for (int i = 0; i < _entities.Count(); i++)
	{
		auto e = _entities[i];
		if (e->update(level, dt))
		{
			_entities.RemoveAt(i);
			delete e;
		}
	}
}

void Chunk::calculatePathPoint()
{
	auto size = getContentSize();
	int prevX = _previousChunk != nullptr ? _previousChunk->getPathPointForPrevChunk().x : 0;
	do
	{
		auto pos = Vec2(rand() % static_cast<int>(size.width * 0.6f), rand() % static_cast<int>(size.height * 0.5f));
		_pathPoint = getPosition() + AlignPointToGrid(pos + size * 0.1f);

		//const int LOCAL_I = 0;
		//_pathPoint = getPosition() + Vec2(LOCAL_I % (int)size.width, 64);
		//LOCAL_I += CHUNKS_BLOCK_SIZE;

	} while (abs(_pathPoint.x - prevX) <= 2);
}

void Chunk::Spread(Level* level)
{
	if (_nextChunk == nullptr)
	{
		// Switch chunk type
		Vec2 top = getPosition() + Vec2(0, getContentSize().height);
		if (top.y < CHUNKS_DEFAULT_HEIGHT * 4)// pierwsze kilka hunkow zawsze musza byc zwykle zeby sie grac nie przerazil, arazil, obarazil
		{
			_nextChunk = ChunkBasic::create(level, this, top);
		}
		else
		{
			switch (rand() % 17)
			{
				case 0: _nextChunk = PreBossFight::create(level, this, top); break;
				case 1:
				case 2: _nextChunk = ShoppingCenter::create(level, this, top); break;
				default: _nextChunk = ChunkBasic::create(level, this, top); break;
			}
		}

		// Generate itself
		generate();
	}
	else
	{
		_nextChunk->Spread(level);
	}
}

Sprite* Chunk::addPlatform(Vec2 location, float width)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/brick.png");
	Texture2D::TexParams tp = { GL_POINT, GL_POINT, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, width, CHUNKS_BLOCK_SIZE));
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sprite->setPosition(Vec2(location.x + width / 2, location.y));

	// Create body
	auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.52f));
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	sprite->setPhysicsBody(body);

	// Add and back to the future
	addChild(sprite);
	_platforms.Add(sprite);
	return sprite;
}

void Chunk::removePlatform(Sprite* platform)
{
	_platforms.Remove(platform);
	platform->removeFromParentAndCleanup(true);
}

Ladder* Chunk::addLadder(Vec2 location, float height)
{
	Ladder* result = new Ladder(this, location, height);
	_entities.Add(result);
	return result;
}

void Chunk::addCoin(Vec2 location)
{
	_entities.Add(new Coin(this, location));
}

void Chunk::addAmmo(Vec2 location)
{
	_entities.Add(new Ammo(this, location));
}

void Chunk::addWall(char dir)
{
	// Load texture and ensure its repeating
	Texture2D* texture = TextureCache::sharedTextureCache()->addImage("Textures/dirt.png");
	Texture2D::TexParams tp = { GL_POINT, GL_POINT, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(tp);

	// Calculate size and location
	Vec2 size, position;
	auto chunkSize = getContentSize();
	switch (dir)
	{
		// super dluga podloga dla basefloorchunk
		case '_':
		{
			size = Vec2(chunkSize.width * 3, CHUNKS_DEFAULT_HEIGHT);
			position = Vec2(chunkSize.width / 2, -size.y / 2);
		}
		break;

		// podloga
		case 's':
		{
			size = Vec2(chunkSize.width, CHUNKS_DEFAULT_HEIGHT);
			position = Vec2(size.x / 2, -size.y / 2);
		}
		break;

		// sciana na lewo
		case 'a':
		{
			size = Vec2(CHUNKS_WIDTH, chunkSize.height);
			position = Vec2(-size.x / 2, size.y / 2);
		}
		break;

		// sciana na prawo
		case 'd':
		{
			size = Vec2(CHUNKS_WIDTH, chunkSize.height);
			position = Vec2(size.x / 2 * 3, size.y / 2);
		}
		break;

		// error
		default: assert(0); break;
	}

	// Create sprite
	auto sprite = Sprite::createWithTexture(texture, Rect(0, 0, size.x, size.y));
	sprite->setPosition(position);
	//sprite->setTag(PHYSICS_TAG_GROUND);

	// Create body
	auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.52f));
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	sprite->setPhysicsBody(body);

	// Add and back to the future
	addChild(sprite);
	_walls.push_back(sprite);
}

Sprite* Chunk::platformAtPoint(const Vec2& point) const
{
	// Transform info local world
	Vec2 localPos = point - getPosition();

	// Check all platforms
	for (int i = 0; i < _platforms.Count(); i++)
	{
		auto node = _platforms[i];

		auto size = node->getContentSize();
		auto pos = node->getPosition();
		Rect rect = Rect(pos.x - size.width / 2 + 10, pos.y + 10 - CHUNKS_BLOCK_SIZE_HALF, size.width - 20, size.height - 20);

		if (rect.containsPoint(localPos))
		{
			//_platforms[i]->setColor(Color3B(220, 22, 22));
			return _platforms[i];
		}
	}

	// Check all walls
	/*for (int i = 0; i < _walls.size(); i++)
	{
		auto node = _walls[i];

		auto size = node->getContentSize();
		auto ladderOrgin = node->getPosition();// -Vec2(size.width / 2, size.height / 2);
		Rect ladderRect = Rect(ladderOrgin.x, ladderOrgin.y, size.width, size.height);

		if (ladderRect.containsPoint(localPos))
		{
			_walls[i]->setColor(Color3B(10, 220, 22));
			return _walls[i];
		}
	}*/

	return nullptr;
}

void Chunk::tryToSpawnAFly()
{
	if (getPosition().y > CHUNKS_DEFAULT_HEIGHT * 2 && rand() % 5 == 0)
	{
		spawnAFly();
	}
}

void Chunk::spawnAFly()
{
	// Calculate control points
	auto size = getContentSize();
	Vec2 p1 = Vec2(rand() % static_cast<int>(size.width * 0.5f), rand() % static_cast<int>(size.height * 0.4f));
	Vec2 p2;
	int iters = 0;
	do
	{
		p2 = Vec2(rand() % static_cast<int>(size.width * 0.6f), rand() % static_cast<int>(size.height * 0.5f));

		if (iters++ > 10000) return;

	} while (p1.distance(p2) < 500);

	// Spawn
	_entities.Add(new Fly(this, p1, p2));
}
