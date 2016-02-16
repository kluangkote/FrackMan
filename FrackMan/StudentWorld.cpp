#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	for(int i = 0; i < VIEW_WIDTH; i++)
	{
		for(int j = 0; j < VIEW_HEIGHT-4; j++)
		{
			if(i >= 30 && i <= 33 && j >= 4)
			{
				dirt[i][j] = nullptr;
			}
			else
			{
				dirt[i][j] = new Dirt(i, j);
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

void StudentWorld::cleanUp()
{
	delete player;
	for(int i = 0; i < VIEW_WIDTH; i++)
	{
		for(int j = 0; j < VIEW_HEIGHT-4; j++)
		{
			delete dirt[i][j];
		}
	}
}

void StudentWorld::destroyDirt(int startX, int startY, int endX, int endY)
{
	bool dirtDeleted = false;
	for(int i = startX; i <= endX; i++)
	{
		for(int j = startY; j <= endY; j++)
		{
			if(j <= 59)
			{
				if(dirt[i][j] != nullptr)
					dirtDeleted = true;
				delete dirt[i][j];
				dirt[i][j] = nullptr;
			}
		}
	}
	if(dirtDeleted)
		playSound(SOUND_DIG);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
