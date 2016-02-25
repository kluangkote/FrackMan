#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <cstdlib>
#include <queue>
using namespace std;

struct Position
{
    int x;
    int y;
};

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{

	// DIRT
	for(int i = 0; i < VIEW_WIDTH; i++)
	{
		for(int j = 0; j < VIEW_HEIGHT; j++)
		{
			if((i >= 30 && i <= 33 && j >= 4) || j >= 60)
				dirt[i][j] = nullptr;
			else
				dirt[i][j] = new Dirt(i, j);
		}
	}

	// FRACKMAN
	player = new FrackMan(this);

  // PROTESTERS

  int i = getLevel() * 10 + 30;
  int probabilityOfHardcore = min(90, i);
  int isHardCore = rand() % probabilityOfHardcore;
  if(isHardCore == 1)
    actors.push_back(new HardcoreProtester(this));
  else
    actors.push_back(new RegularProtester(this));
  numberOfProtesters++;

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
		int y = rand() % 57;
		while(x >= 27 && x <= 33)
		{
			x = rand() % 61;
			y = rand() % 57;
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
		int y = rand() % 57;
		while(x >= 27 && x <= 33)
		{
			x = rand() % 61;
			y = rand() % 57;
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

  int m = 2+getLevel()*1.5;
  int P = min(15, m);
  int i = getLevel()*10+30;
  int probabilityOfHardcore = min(90, i);
  int isHardCore = rand() % probabilityOfHardcore;
  int x = 200-getLevel();
  if(ticks == max(25, x) && numberOfProtesters < P)
  {
    if(isHardCore == 1)
      actors.push_back(new HardcoreProtester(this));
    else
      actors.push_back(new RegularProtester(this));
    numberOfProtesters++;
  }
  if(ticks > max(25, x))
    ticks = max(25, x);
  else
    ticks++;

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
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	if(player->getHealth() <= 0)
	{
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}

	it = actors.begin();
	while(it != actors.end())
	{
		if((*it)->isActorAlive() == false)
		{
      if((*it)->canGetAnnoyed())
        numberOfProtesters--;
			delete *it;
			it = actors.erase(it);
		}
		else
			it++;
	}

	setDisplayText();
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

void StudentWorld::setDisplayText()
{
	int score = player->getPoints();
	int level = getLevel();
	int lives = getLives();
	int health = player->getHealth()*10;
	int squirts = player->getWater();
	int gold = player->getGold();
	int sonar = player->getSonar();
	// Next, create a string from your statistics, of the form:
	// Scr: 321000 Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Sonar: 1 Oil Left: 2
	string s = formatStats(score, level, lives, health, squirts, gold, sonar, barrel);
	// Finally, update the display text at the top of the screen with your // newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatStats(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels)
{
	string ans = "";
	string scoreString = to_string(score);
	if(scoreString.size() == 1)
		ans += "Scr: 00000" + scoreString;
	else if(scoreString.size() == 2)
		ans += "Scr: 0000" + scoreString;
	else if(scoreString.size() == 3)
		ans += "Scr: 000" + scoreString;
	else if(scoreString.size() == 4)
		ans += "Scr: 00" + scoreString;
	else if(scoreString.size() == 5)
		ans += "Scr: 0" + scoreString;
	else
		ans += "Scr: " + scoreString;
	string levelString = to_string(level);
	if(levelString.size() == 1)
		ans += "  Lvl:  " + levelString;
	else
		ans += "  Lvl: " + levelString;
	ans += "  Lives: " + to_string(lives);
	string healthString = to_string(health);
	if(healthString.size() == 3)
		ans += "  Hlth: " + healthString + "%";
	else if(healthString.size() == 2)
		ans += "  Hlth:  " + healthString + "%";
	else
		ans += "  Hlth: " + healthString;
	string squirtString = to_string(squirts);
	if(squirtString.size() == 1)
		ans += "  Wtr:  " + squirtString;
	else
		ans += "  Wtr: " + squirtString;
	string goldString = to_string(gold);
	if(goldString.size() == 1)
		ans += "  Gld:  " + goldString;
	else
		ans += "  Gld: " + goldString;
	string sonarString = to_string(sonar);
	if(sonarString.size() == 1)
		ans += "  Sonar:  " + sonarString;
	else
		ans += "  Sonar: " + sonarString;
	string barrelString = to_string(barrels);
	if(barrelString.size() == 1)
		ans += "  Oil Left:  " + barrelString;
	else
		ans += "  Oil Left: " + barrelString;
	return ans;
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
		if(y < 60 && dirt[x][y+4] == nullptr && dirt[x+1][y+4] == nullptr && dirt[x+2][y+4] == nullptr && dirt[x+3][y+4] == nullptr)
			return true;
	}
	if(dir == GraphObject::left)
	{
		if(x > 0 && dirt[x-1][y] == nullptr && dirt[x-1][y+1] == nullptr && dirt[x-1][y+2] == nullptr && dirt[x-1][y+3] == nullptr)
			return true;
	}
	if(dir == GraphObject::right)
	{
		if(x < 60 && dirt[x+4][y] == nullptr && dirt[x+4][y+1] == nullptr && dirt[x+4][y+2] == nullptr && dirt[x+4][y+3] == nullptr)
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

bool StudentWorld::annoyFrack(int damage)
{
	if(damage == 100)
	{
		if(touchingBoulder(getFrackX(), getFrackY(), player->getDirection(), 3, player))
		{
			player->getAnnoyed(damage);
			decLives();
			return true;
		}
	}
	if(damage == 2)
	{
		player->getAnnoyed(damage);
		return true;
	}
	return false;
}

bool StudentWorld::annoyProtesters(Actor* actor, int damage)
{
	bool annoyed = false;
	for(int i = 0; i < actors.size(); i++)
	{
		if(actors[i]->canGetAnnoyed())
		{
			if(getRadius(actor->getX(), actor->getY(), actors[i]->getX(), actors[i]->getY()) <= 3)
			{
				actors[i]->getAnnoyed(damage);
				annoyed = true;
			}
		}
	}
	return annoyed;
}

bool StudentWorld::pickUpGold(Actor* actor)
{
	for(int i = 0; i < actors.size(); i++)
	{
		if(actors[i]->canGetAnnoyed())
		{
			if(getRadius(actor->getX(), actor->getY(), actors[i]->getX(), actors[i]->getY()) <= 3)
			{
				actors[i]->receiveGold();
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::findShortestToFrack()
{
  for(int i = 0; i < VIEW_WIDTH; i++)
  {
    for(int j = 0; j < VIEW_HEIGHT; j++)
    {
      findFrack[i][j] = -1;
    }
  }
	queue<Position> q;
	Position curr;
	curr.x = getFrackX();
	curr.y = getFrackY();
	q.push(curr);
	findFrack[curr.x][curr.y] = 0;
	while(!q.empty())
	{
		curr = q.front();
		q.pop();
    if(canMove(curr.x, curr.y, GraphObject::up) && !touchingBoulder(curr.x, curr.y, GraphObject::up, 3, player) && findFrack[curr.x][curr.y+1] < 0)
		{
			Position north;
			north.x = curr.x;
			north.y = curr.y+1;
			q.push(north);
			findFrack[north.x][north.y] = findFrack[curr.x][curr.y] + 1;
		}
  	if(canMove(curr.x, curr.y, GraphObject::right) && !touchingBoulder(curr.x, curr.y, GraphObject::right, 3, player) && findFrack[curr.x+1][curr.y] < 0)
		{
			Position east;
			east.x = curr.x+1;
			east.y = curr.y;
			q.push(east);
			findFrack[east.x][east.y] = findFrack[curr.x][curr.y] + 1;
		}
    if(canMove(curr.x, curr.y, GraphObject::down) && !touchingBoulder(curr.x, curr.y, GraphObject::down, 3, player) && findFrack[curr.x][curr.y-1] < 0)
		{
			Position south;
			south.x = curr.x;
			south.y = curr.y-1;
			q.push(south);
			findFrack[south.x][south.y] = findFrack[curr.x][curr.y] + 1;
		}
    if(canMove(curr.x, curr.y, GraphObject::left) && !touchingBoulder(curr.x, curr.y, GraphObject::left, 3, player) && findFrack[curr.x-1][curr.y] < 0)
		{
			Position west;
			west.x = curr.x-1;
      west.y = curr.y;
			q.push(west);
			findFrack[west.x][west.y] = findFrack[curr.x][curr.y] + 1;
		}
	}
}

void StudentWorld::layOutShortestPath()
{
  for(int i = 0; i < VIEW_WIDTH; i++)
  {
    for(int j = 0; j < VIEW_HEIGHT; j++)
    {
      pathToExit[i][j] = -1;
    }
  }
	queue<Position> q;
	Position curr;
	curr.x = 60;
	curr.y = 60;
	q.push(curr);
	pathToExit[curr.x][curr.y] = 0;
	while(!q.empty())
	{
		curr = q.front();
		q.pop();
    if(canMove(curr.x, curr.y, GraphObject::up) && !touchingBoulder(curr.x, curr.y, GraphObject::up, 3, player) && pathToExit[curr.x][curr.y+1] < 0)
		{
			Position north;
			north.x = curr.x;
			north.y = curr.y+1;
			q.push(north);
			pathToExit[north.x][north.y] = pathToExit[curr.x][curr.y] + 1;
		}
  	if(canMove(curr.x, curr.y, GraphObject::right) && !touchingBoulder(curr.x, curr.y, GraphObject::right, 3, player) && pathToExit[curr.x+1][curr.y] < 0)
		{
			Position east;
			east.x = curr.x+1;
			east.y = curr.y;
			q.push(east);
			pathToExit[east.x][east.y] = pathToExit[curr.x][curr.y] + 1;
		}
    if(canMove(curr.x, curr.y, GraphObject::down) && !touchingBoulder(curr.x, curr.y, GraphObject::down, 3, player) && pathToExit[curr.x][curr.y-1] < 0)
		{
			Position south;
			south.x = curr.x;
			south.y = curr.y-1;
			q.push(south);
			pathToExit[south.x][south.y] = pathToExit[curr.x][curr.y] + 1;
		}
    if(canMove(curr.x, curr.y, GraphObject::left) && !touchingBoulder(curr.x, curr.y, GraphObject::left, 3, player) && pathToExit[curr.x-1][curr.y] < 0)
		{
			Position west;
			west.x = curr.x-1;
            west.y = curr.y;
			q.push(west);
			pathToExit[west.x][west.y] = pathToExit[curr.x][curr.y] + 1;
		}
	}
}

GraphObject::Direction StudentWorld::getShortestDirection(int x, int y, bool hardcore)
{
  int shortest = INT_MAX;
  GraphObject::Direction dir = GraphObject::up;
  if(hardcore)
  {
    if(y < 60 && findFrack[x][y+1] >= 0)
  	{
  		shortest = findFrack[x][y+1];
      dir = GraphObject::up;
  	}
  	if(y > 0 && findFrack[x][y-1] >= 0)
  	{
  		if(findFrack[x][y-1] < shortest)
  		{
  			shortest = findFrack[x][y-1];
        dir = GraphObject::down;
  		}
  	}
  	if(x < 60 && findFrack[x+1][y] >= 0)
  	{
  			if(findFrack[x+1][y] < shortest)
  			{
  				shortest = findFrack[x+1][y];
          dir = GraphObject::right;
  			}
  	}
  	if(x > 0 && findFrack[x-1][y] >= 0)
  	{
  		if(findFrack[x-1][y] < shortest)
  		{
  			shortest = findFrack[x-1][y];
        dir = GraphObject::left;
  		}
    }
  }
  else
  {
    layOutShortestPath();
    if(y < 60 && pathToExit[x][y+1] >= 0)
    {
      shortest = pathToExit[x][y+1];
      dir = GraphObject::up;
    }
    if(y > 0 && pathToExit[x][y-1] >= 0)
    {
      if(pathToExit[x][y-1] < shortest)
      {
        shortest = pathToExit[x][y-1];
        dir = GraphObject::down;
      }
    }
    if(x < 60 && pathToExit[x+1][y] >= 0)
    {
      if(pathToExit[x+1][y] < shortest)
      {
        shortest = pathToExit[x+1][y];
        dir = GraphObject::right;
      }
    }
    if(x > 0 && pathToExit[x-1][y] >= 0)
    {
      if(pathToExit[x-1][y] < shortest)
      {
        shortest = pathToExit[x-1][y];
        dir = GraphObject::left;
      }
    }
  }
	return dir;
}

bool StudentWorld::seeFrack(int x, int y, GraphObject::Direction& dir)
{
  if(x == getFrackX())
  {
    if(y < getFrackY())
    {
      dir = GraphObject::up;
      return true;
    }
    if(y > getFrackY())
    {
      dir = GraphObject::down;
      return true;
    }
  }
  else if(y == getFrackY())
  {
    if(x < getFrackX())
    {
      dir = GraphObject::right;
      return true;
    }
    if(x > getFrackX())
    {
      dir = GraphObject::left;
      return true;
    }
  }
  else
  {
    dir = GraphObject::none;
    return false;
  }
  return false;
}

int StudentWorld::stepsAwayFromFrack(int x, int y)
{
  findShortestToFrack();
  return findFrack[x][y];
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
