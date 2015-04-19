
#include "Game.h"
#include <iostream>
#include <fstream>
#include "Player\Player.h"

using namespace std;

// Global static pointer used to ensure a single instance of the class.
Game* Game::_instance = NULL;

Game::Game()
{

}

//////////////////////////////////////
// Profile file format:
//////////////////////////////////////
// Version: 1
//////////////////////////////////////
// - Version number: int
// - Player name: string
//////////////////////////////////////

void Game::loadProfile(string profileName)
{
	/*
	auto fileUtils = FileUtils::getInstance();
	const string profileFolderName = "Profiles/" + profileName;
	const string profileFileName = profileFolderName + "/profile.txt";

	// Ensure that profile directory exists
	if (!fileUtils->isDirectoryExist(profileFolderName))
	{
	fileUtils->createDirectory(profileFolderName);
	}

	// Check if profile file exists
	if (!fileUtils->isFileExist(profileFileName))
	{
	// create profile file for version no. 1
	ofstream profileFile;
	profileFile.open(profileFileName, ios::out);
	profileFile << 1 << endl;
	profileFile << profileName;
	profileFile.close();
	}
	*/

	// sworz gracza
	_player = new Player("xd");
}
