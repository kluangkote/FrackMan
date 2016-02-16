#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	for(int i = 0; i < VIEW_HEIGHT-4; i++)
	{
		for(int j = 0; j < VIEW_WIDTH; j++)
		{
			if(j >= 30 && j <= 33 && i > 4)
			{
				dirt[i][j] = nullptr;
			}
			else
			{
				dirt[i][j] = new Dirt(j, i);
				amtOfDirt++;
			}
		}
	}
	player = new FrackMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	return GWSTATUS_CONTINUE_GAME;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
