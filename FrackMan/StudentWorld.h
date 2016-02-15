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

	virtual int init()
	{
		for(int i = 0; i < VIEW_HEIGHT-4; i++)
		{
			for(int j = 0; j < VIEW_WIDTH; j++)
			{
				if(j >= 30 && j <= 33 && i > 4)
				{
				}
        else
        {
					dirt[i][j] = new Dirt(j, i);
					amtOfDirt++;
        }
			}
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
	}

private:
	Dirt* dirt[VIEW_HEIGHT-4][VIEW_HEIGHT];
	int amtOfDirt;
};

#endif // STUDENTWORLD_H_
