#pragma once

#include <string>
#include "../Types/List.h"

class Highscores
{
private:

	typedef struct Entry
	{
		std::string Name;
		int Score;

		Entry()
		{
			// Invalid entry
			Score = -1;
		}

		Entry(std::string name, int score)
			:Name(name),
			Score(score)
		{
		}

	} Entry;

	List<Entry, 32> _table;

public:

	Highscores();

	void load();
	void save();

	void addRecord(std::string name, int score);
};

extern Highscores Scores;
