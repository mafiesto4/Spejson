#pragma once

class Level;
class Chunk;

// Single game entity interface
class Entity
{
protected:

	Chunk* _parent;

	Entity(Chunk* parent)
		:_parent(parent)
	{ }

public:

	// Virtual destructor
	virtual ~Entity() { }

	// Update entity
	// @returns True if entity wwants to be deleated
	virtual bool update(Level* level, float dt) { return false; }
};
