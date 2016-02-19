#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <cstdlib>
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
				dirt[i][j] = nullptr;
			else
				dirt[i][j] = new Dirt(i, j);
		}
	}
	player = new FrackMan(this);
	int x = getLevel()/2+2;
	int B = min(x, 6);
	for(int i = 0; i < B; i++)
	{
		int x = rand() % 61;
		int y = rand() % 37 + 20;
		while(x >= 27 && x <= 33)
		{
			x = rand() % 61;
			y = rand() % 37 + 20;
		}
		actors.push_back(new Boulder(x, y, this));
		destroyDirt(x, y, x+3, y+3, false);
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	vector<Actor*>::iterator it;
	it = actors.begin();
	while(it != actors.end())
	{
		(*it)->doSomething();
		it++;
	}
	it = actors.begin();
	while(it != actors.end())
	{
		if((*it)->isActorAlive() == false)
		{
			delete *it;
			it = actors.erase(it);
		}
		else
			it++;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete player;
	for(int i = 0; i < VIEW_WIDTH; i++)
		for(int j = 0; j < VIEW_HEIGHT-4; j++)
			delete dirt[i][j];
}

void StudentWorld::destroyDirt(int startX, int startY, int endX, int endY, bool isFrack)
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
	if(dirtDeleted && isFrack)
		playSound(SOUND_DIG);
}

Dirt* StudentWorld::getDirt(int x, int y)
{
	return dirt[x][y];
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
