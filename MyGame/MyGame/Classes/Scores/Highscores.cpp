
#include "Highscores.h"
#include "cocos2d.h"
#include <fstream>

using namespace cocos2d;
using namespace std;

Highscores Scores;
const char* scoresFilename = "score.dat";

Highscores::Highscores()
{
	
}

void Highscores::load()
{
	// Clear table
	_table.Clear();

	// Load entries
	std::fstream plik;
	plik.open(scoresFilename, std::ios::in);
	if (plik.good())
	{
		int cnt, score;
		std::string name;
		plik >> cnt;
		for (int i = 0; i < cnt; i++)
		{
			plik >> name;
			plik >> score;
			_table.Add(Entry(name, score));
		}
		plik.close();
	}
}

void Highscores::save()
{
	// Save entries
	std::fstream plik;
	plik.open(scoresFilename, std::ios::out);
	if (plik.good())
	{
		int cnt = _table.Count();
		plik << cnt << endl;
		for (int i = 0; i < cnt; i++)
		{
			plik << _table[i].Name << endl << _table[i].Score << endl;
		}
		plik.close();
	}
}

void Highscores::addRecord(std::string name, int score)
{
	// Add element
	_table.Add(Entry(name, score));

	// Sort results
	_table.Sort();
}
