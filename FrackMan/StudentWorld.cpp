#include "StudentWorld.h"
#include "Actor.h"
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
	while(B > 0)
	{
		bool shouldAdd = true;
		int x = rand() % 61;
		int y = rand() % 37 + 20;
		while(x >= 27 && x <= 33)
		{
			x = rand() % 61;
			y = rand() % 37 + 20;
		}
		for(int i = 0; i < actors.size(); i++)
		{
			if(getRadius(x, y, actors[i]->getX(), actors[i]->getY()) <= 6)
			{
				shouldAdd = false;
				break;
			}
		}
		if(shouldAdd)
		{
			actors.push_back(new Boulder(x, y, this));
			destroyDirt(x, y, x+3, y+3, false);
			B--;
		}
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

bool StudentWorld::canMove(int x, int y, GraphObject::Direction dir)
{
	if(dir == GraphObject::down)
	{
		if(touchingBoulder(x, y, 3))
			return false;
		if(y > 0 && dirt[x][y-1] == nullptr && dirt[x+1][y-1] == nullptr && dirt[x+2][y-1] == nullptr && dirt[x+3][y-1] == nullptr)
			return true;
	}
	if(dir == GraphObject::up)
	{
		if(touchingBoulder(x, y, 3))
			return false;
		if(y < 63 && dirt[x][y+1] == nullptr && dirt[x+1][y+1] == nullptr && dirt[x+2][y+1] == nullptr && dirt[x+3][y+1] == nullptr)
			return true;
	}
	if(dir == GraphObject::left)
	{
		if(touchingBoulder(x, y, 3))
			return false;
		if(x > 0 && dirt[x-1][y] == nullptr && dirt[x+1][y+1] == nullptr && dirt[x+2][y+2] == nullptr && dirt[x+3][y+3] == nullptr)
			return true;
	}
	if(dir == GraphObject::right)
	{
		if(touchingBoulder(x, y, 3))
			return false;
		if(x < 63 && dirt[x+1][y] == nullptr && dirt[x+1][y+1] == nullptr && dirt[x+2][y+2] == nullptr && dirt[x+3][y+3] == nullptr)
			return true;
	}
	return false;
}

bool StudentWorld::touchingBoulder(int x, int y, double radiusLimit)
{
	for(int i = 0; i < actors.size(); i++)
	{
		if(!actors[i]->canShareSpace() && getRadius(x, y, actors[i]->getX(), actors[i]->getY()) <= radiusLimit)
		{
			if(actors[i]->getX() != x || actors[i]->getY() != y)
				return true;
		}
	}
	return false;
}

double StudentWorld::getRadius(int x, int y, int otherX, int otherY)
{
	return sqrt(pow(otherX-x,2)+pow(otherY-y,2));
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
