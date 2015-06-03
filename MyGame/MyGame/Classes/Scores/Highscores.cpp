
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
	fstream plik;
	plik.open(scoresFilename, ios::in);
	if (plik.good())
	{
		int cnt, score;
		string name;
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
	fstream plik;
	plik.open(scoresFilename, ios::out);
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

void Highscores::addRecord(string name, int score)
{
	// Add element
	_table.Add(Entry(name, score));

	// Sort results
	_table.Sort();

	// Remove lowest results
	while (_table.Count() > 8)
	{
		_table.RemoveAt(_table.Count() - 1);
	}
}
