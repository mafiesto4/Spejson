
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


void Game::play()
{
	_player = new Player("xd");
	Director::getInstance()->replaceScene(Level::createScene());
}
