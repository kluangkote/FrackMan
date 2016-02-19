#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	virtual ~StudentWorld()
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

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void destroyDirt(int startX, int startY, int endX, int endY, bool isFrack);

	bool canMove(int x, int y, GraphObject::Direction dir);

	double getRadius(int x, int y, int otherX, int otherY);

	bool touchingBoulder(int x, int y);

private:
	Dirt* dirt[VIEW_WIDTH][VIEW_HEIGHT-4];
	FrackMan* player;
	vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
