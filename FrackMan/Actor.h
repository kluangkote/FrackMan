#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
  : GraphObject(imageID, startX, startY, dir, size, depth)
  {
    myWorld = world;
    setVisible(true);
  }

  virtual ~Actor()
  {
  }

  virtual void doSomething() = 0;

  StudentWorld* getWorld()
  {
    return myWorld;
  }
private:
  StudentWorld* myWorld;
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
  virtual void doSomething();
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
  void doSomething();
};

#endif // ACTOR_H_
