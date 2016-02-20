#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
using namespace std;
class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool sh = true)
  : GraphObject(imageID, startX, startY, dir, size, depth)
  {
    myWorld = world;
    setVisible(true);
    isAlive = true;
    share = sh;
  }

  virtual ~Actor()
  {
  }

  virtual void doSomething()
  {
    if(!isAlive)
      return;
  }

  bool isActorAlive()
  {
    return isAlive;
  }

  void setAlive(bool alive)
  {
    isAlive = alive;
  }

  StudentWorld* getWorld()
  {
    return myWorld;
  }

  bool canShareSpace()
  {
    return share;
  }
private:
  StudentWorld* myWorld;
  bool isAlive;
  bool share;
};

class Goodie : public Actor
{
public:
  Goodie(int imageID, int x, int y, StudentWorld* world)
  : Actor(imageID, x, y, world, right, 1, 2)
  {
  }

  virtual ~Goodie()
  {
  }

  void virtual doSomething();
};

class PopUpGoodie : public Goodie
{
public:
  PopUpGoodie(int imageID, int x, int y, StudentWorld* world);

  virtual ~PopUpGoodie()
  {
  }

  void virtual doSomething();
private:
  int ticks;
};

class Dirt : public Actor
{
public:
  Dirt(int startX, int startY) : Actor(IID_DIRT, startX, startY, nullptr, right, 0.25, 3)
  {
  }

  virtual ~Dirt()
  {
  }
};

class FrackMan : public Actor
{
public:
  FrackMan(StudentWorld* world) : Actor(IID_PLAYER, 30, 60, world, right, 1.0, 0)
  {
    water = 5;
    sonar = 1;
  }

  virtual ~FrackMan()
  {
  }

  virtual void doSomething();

  void addWater();

  void addSonar();
private:
  int water;
  int sonar;
};

class Boulder : public Actor
{
public:
  Boulder(int x, int y, StudentWorld* world) : Actor(IID_BOULDER, x, y, world, down, 1.0, 1, false)
  {
    state = "stable";
    countTicks = 0;
  }

  virtual ~Boulder()
  {
  }

  virtual void doSomething();
private:
  string state;
  int countTicks;
};

class WaterPool : public PopUpGoodie
{
public:
  WaterPool(int x, int y, StudentWorld* world) : PopUpGoodie(IID_WATER_POOL, x, y, world)
  {
  }

  virtual ~WaterPool()
  {
  }

  virtual void doSomething();
private:
  int ticks;
};

class SonarKit : public PopUpGoodie
{
public:
  SonarKit(StudentWorld* world) : PopUpGoodie(IID_SONAR, 0, 60, world)
  {
  }

  virtual ~SonarKit()
  {
  }

  virtual void doSomething();
};

#endif // ACTOR_H_
