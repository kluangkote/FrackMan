#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		amtOfDirt = 0;
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp()
	{
	}

	void destroyDirt(int startX, int startY, int endX, int endY);

private:
	Dirt* dirt[VIEW_WIDTH][VIEW_HEIGHT-4];
	int amtOfDirt;
	FrackMan* player;
};

#endif // STUDENTWORLD_H_
