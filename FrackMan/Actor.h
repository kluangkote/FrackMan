#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
  : GraphObject(imageID, startX, startY, dir, size, depth)
  {
    setVisible(true);
  }

  virtual ~Actor()
  {
  }

  virtual void doSomething() = 0;
};

class Dirt : public Actor
{
public:
  Dirt(int startX, int startY) : Actor(IID_DIRT, startX, startY, right, 0.25, 3)
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
  FrackMan(StudentWorld* world) : Actor(IID_PLAYER, 30, 60, right, 1.0, 0)
  {
    myWorld = world;
  }
  virtual ~FrackMan()
  {
  }
  void doSomething();
private:
  StudentWorld* myWorld;
};

#endif // ACTOR_H_
