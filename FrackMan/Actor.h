#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
using namespace std;
class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool s = true)
  : GraphObject(imageID, startX, startY, dir, size, depth)
  {
    myWorld = world;
    setVisible(true);
    isAlive = true;
    share = s;
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
  }

  virtual ~FrackMan()
  {
  }

  virtual void doSomething();
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

#endif // ACTOR_H_
