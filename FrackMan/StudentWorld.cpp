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
		for(int j = 0; j < VIEW_HEIGHT; j++)
		{
			if(i >= 30 && i <= 33 && j >= 4 || j >= 60)
				dirt[i][j] = nullptr;
			else
				dirt[i][j] = new Dirt(i, j);
		}
	}
	player = new FrackMan(this);

	// BOULDER
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

	// BARRELS OF OIL
	int a = 2 + getLevel();
	int L = min(a, 20);
	barrel = L;
	while(L > 0)
	{
		bool shouldAdd = true;
		int x = rand() % 61;
		int y = rand() % 61;
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
			actors.push_back(new OilBarrel(x, y, this));
			L--;
		}
	}

	// GOLD NUGGETS
	int c = 5-getLevel()/2;
	int G = max(c, 2);
	while(G > 0)
	{
		bool shouldAdd = true;
		int x = rand() % 61;
		int y = rand() % 61;
		while(x >= 27 && x <= 33)
		{
			x = rand() % 61;
			y = rand() % 61;
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
			actors.push_back(new GoldNugget(x, y, false, this));
			G--;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

// void findSpot(int numberOfItems)
// {
// 	int G = max(numberOfItems, 2);
// 	while(G > 0)
// 	{
// 		bool shouldAdd = true;
// 		int x = rand() % 61;
// 		int y = rand() % 61;
// 		while(x >= 27 && x <= 33)
// 		{
// 			x = rand() % 61;
// 			y = rand() % 61;
// 		}
// 		for(int i = 0; i < actors.size(); i++)
// 		{
// 			if(getRadius(x, y, actors[i]->getX(), actors[i]->getY()) <= 6)
// 			{
// 				shouldAdd = false;
// 				break;
// 			}
// 		}
// 		if(shouldAdd)
// 		{
// 			actors.push_back(new GoldNugget(x, y, this));
// 			G--;
// 		}
// 	}
// }

int StudentWorld::move()
{
	vector<Actor*>::iterator it;
	it = actors.begin();
	while(it != actors.end())
	{
		(*it)->doSomething();
		it++;
	}

	player->doSomething();

	int G = getLevel() * 25 + 300;
	int randomNumber = rand() % G + 1;
	if(randomNumber == 1)
	{
		int sonarOrWater = rand() % 5 + 1;
		if(sonarOrWater != 1)
		{
			int x = 0;
			int y = 0;
			bool add = false;
			while(!add)
			{
				x = rand() % 61;
				y = rand() % 61;
				if(dirt[x][y] == nullptr)
				{
					add = true;
					for(int i = x; i < x+4; i++)
					{
						for(int j = y; j < y+4; j++)
						{
							if(dirt[i][j] != nullptr)
							{
								add = false;
								break;
							}
						}
						if(!add)
							break;
					}
				}
			}
			actors.push_back(new WaterPool(x, y, this));
		}
		else
			actors.push_back(new SonarKit(this));
	}

	if(barrel == 0)
		return GWSTATUS_FINISHED_LEVEL;

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
	vector<Actor*>::iterator it;
	it = actors.begin();
	while(it != actors.end())
	{
		delete *it;
		it = actors.erase(it);
	}
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
		if(y > 0 && dirt[x][y-1] == nullptr && dirt[x+1][y-1] == nullptr && dirt[x+2][y-1] == nullptr && dirt[x+3][y-1] == nullptr)
			return true;
	}
	if(dir == GraphObject::up)
	{
		if(y < 63 && dirt[x][y+4] == nullptr && dirt[x+1][y+4] == nullptr && dirt[x+2][y+4] == nullptr && dirt[x+3][y+4] == nullptr)
			return true;
	}
	if(dir == GraphObject::left)
	{
		if(x > 0 && dirt[x-1][y] == nullptr && dirt[x-1][y+1] == nullptr && dirt[x-1][y+2] == nullptr && dirt[x-1][y+3] == nullptr)
			return true;
	}
	if(dir == GraphObject::right)
	{
		if(x < 63 && dirt[x+4][y] == nullptr && dirt[x+4][y+1] == nullptr && dirt[x+4][y+2] == nullptr && dirt[x+4][y+3] == nullptr)
			return true;
	}
	return false;
}

bool StudentWorld::touchingBoulder(int x, int y, GraphObject::Direction dir, double radiusLimit, Actor* actor)
{
    if(dir == GraphObject::left)
			x--;
    if(dir == GraphObject::right)
			x++;
    if(dir == GraphObject::up)
			y++;
    if(dir == GraphObject::down)
		y--;
	for(int i = 0; i < actors.size(); i++)
	{
		if(!actors[i]->canShareSpace() && getRadius(x, y, actors[i]->getX(), actors[i]->getY()) <= radiusLimit)
		{
			if(actors[i] != actor)
				return true;
		}
	}
	return false;
}

double StudentWorld::getRadius(int x, int y, int otherX, int otherY)
{
	return sqrt(pow(otherX-x,2)+pow(otherY-y,2));
}

int StudentWorld::getFrackX()
{
	return player->getX();
}

int StudentWorld::getFrackY()
{
	return player->getY();
}

void StudentWorld::addWaterToGun()
{
	player->addWater();
}

void StudentWorld::addSonarKit()
{
	player->addSonar();
}

void StudentWorld::barrelFound()
{
	barrel--;
}

void StudentWorld::addPointsToFrack(int points)
{
	player->addPoints(points);
}

void StudentWorld::addGold()
{
	player->addGold();
}

void StudentWorld::sonar()
{
	playSound(SOUND_SONAR);
	for(int i = 0; i < actors.size(); i++)
	{
		if(getRadius(getFrackX(), getFrackY(), actors[i]->getX(), actors[i]->getY()) <= 12)
		{
			actors[i]->setVisible(true);
			actors[i]->setHidden(false);
		}
	}
}

void StudentWorld::addSquirt()
{
	playSound(SOUND_PLAYER_SQUIRT);
	GraphObject::Direction direction = player->getDirection();
	if(direction == GraphObject::left)
		actors.push_back(new Squirt(getFrackX()-4, getFrackY(), direction, this));
	if(direction == GraphObject::right)
		actors.push_back(new Squirt(getFrackX()+4, getFrackY(), direction, this));
	if(direction == GraphObject::up)
		actors.push_back(new Squirt(getFrackX(), getFrackY()+4, direction, this));;
	if(direction == GraphObject::down)
		actors.push_back(new Squirt(getFrackX(), getFrackY()-4, direction, this));
}

void StudentWorld::addBribe()
{
	actors.push_back(new GoldNugget(getFrackX(), getFrackY(), true, this));
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
