#pragma once

#include "ChunkTemplate.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<ChunkTemplate*> ChunkTemplate::ChunkBasic;

bool ChunkTemplate::Load(ifstream& in)
{
	char c;
	in >> Width;
	in >> c;
	in >> Height;

	if (Width <= 1 || Height <= 1)
	{
		// Error
		return true;
	}

	Layout = new char[Width * Height];

	for (int y = 0; y < Height; y++)
	{
		string s;
		getline(in, s);

		int x = 0;
		int rowI = y * Width;
		for (; x < s.length() && x < Width; x++)
		{
			Layout[x + rowI] = s[x];
		}
		for (; x < Width; x++)
		{
			Layout[x + rowI] = CT_AIR;
		}
	}


	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			cout << Get(x, y);
		}
		cout << endl;
	}

	return false;
}

bool ChunkTemplate::LoadTemplates()
{
	return LoadTemplate(&ChunkBasic, "ChunkTemplates/ChunkBasic.txt");
}

bool ChunkTemplate::LoadTemplate(std::vector<ChunkTemplate*>* result, const char* path)
{
	result->clear();

	ifstream in;
	in.open(path, ifstream::in);

	while (in.good())
	{
		ChunkTemplate* ct = new ChunkTemplate();
		if (ct->Load(in))
		{
			delete ct;
			break;
		}

		result->push_back(ct);
	}

	in.close();

	return false;
}
