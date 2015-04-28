#pragma once

#include <iostream>
#include <string>
#include <vector>

#define CT_AIR ' '
#define CT_QUIT 'Q'
#define CT_ENTER 'E'
#define CT_FLOOR '-'

class ChunkTemplate
{
public:

	int Width;
	int Height;
	char* Layout;

	ChunkTemplate()
		:Layout(nullptr)
	{

	}

	~ChunkTemplate()
	{
		if (Layout)
			delete[] Layout;
	}

	char Get(int x, int y)
	{
		return Layout[x + y * Width];
	}

	void Set(int x, int y, char c)
	{
		Layout[x + y * Width] = c;
	}

	char* operator[](int y)
	{
		return &Layout[y * Width];
	}

	bool Load(std::ifstream& in);

public:

	static std::vector<ChunkTemplate*> ChunkBasic;
	static bool LoadTemplates();

private:

	static bool LoadTemplate(std::vector<ChunkTemplate*>* result, const char* path);
};
