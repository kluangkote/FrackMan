#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

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

private:
	Dirt* dirt[VIEW_HEIGHT-4][VIEW_HEIGHT];
	int amtOfDirt;
	FrackMan* player;
};

#endif // STUDENTWORLD_H_
