#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
using namespace std;
class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0,
  bool sh = true, bool annoy = false) : GraphObject(imageID, startX, startY, dir, size, depth)
  {
    myWorld = world;
    setVisible(true);
    isAlive = true;
    share = sh;
    hidden = false;
    canAnnoy = annoy;
  }

  virtual ~Actor()
  {
  }

  virtual void doSomething()
  {
    if(!isAlive)
      return;
  }

  virtual bool getAnnoyed(int damage)
  {
      return false;
  }

  bool isActorAlive()
  {
    return isAlive;
  }

  void moveInDirection(Direction dir);

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

  bool canGetAnnoyed()
  {
    return canAnnoy;
  }

  virtual bool receiveGold()
  {
    return false;
  }

  void setHidden(bool state)
  {
    hidden = state;
  }

  bool isHidden()
  {
    return hidden;
  }
private:
  StudentWorld* myWorld;
  bool isAlive;
  bool share;
  bool hidden;
  bool canAnnoy;
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

  void virtual doSomething(int sound);
};

class PopUpGoodie : public Goodie
{
public:
  PopUpGoodie(int imageID, int x, int y, StudentWorld* world);

  virtual ~PopUpGoodie()
  {
  }

  virtual void doSomething();
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

class People : public Actor
{
public:
  People(int imageID, int x, int y, StudentWorld* world, Direction dir, int size, int depth, int health)
  : Actor(imageID, x, y, world, dir, size, depth, true, true)
  {
    myHealth = health;
  }

  virtual ~People()
  {
  }

  virtual bool getAnnoyed(int damage)
  {
    return false;
  }

  int getHealth()
  {
    return myHealth;
  }

  void setHealth(int health)
  {
    myHealth = health;
  }
private:
  int myHealth;
};

class FrackMan : public People
{
public:
  FrackMan(StudentWorld* world) : People(IID_PLAYER, 30, 60, world, right, 1.0, 0, 10)
  {
    water = 5;
    sonar = 1;
    gold = 0;
    points = 0;
  }

  virtual ~FrackMan()
  {
  }

  virtual void doSomething();

  virtual bool getAnnoyed(int damage);

  void addWater();

  void addSonar();

  void addPoints(int pointsToAdd);

  void addGold();

  int getWater();

  int getSonar();

  int getPoints();

  int getGold();
private:
  int water;
  int sonar;
  int points;
  int gold;
};

class Protester : public People
{
public:
  Protester(int imageID, StudentWorld* world, int health);

  virtual ~Protester()
  {
  }

  virtual void doSomething();

  virtual bool hardcoreCalculate()
  {
    return false;
  }

  bool clearPath(Direction dir);

  virtual bool getAnnoyed(int damage);

  bool getState()
  {
    return leaveField;
  }

  void setStateToTrue()
  {
    leaveField = true;
  }

  void setRestTicks(int i)
  {
    restTicks = i;
  }

  int getRestTicks()
  {
    return restTicks;
  }

  int getTicksSinceShouted()
  {
    return ticksSinceShouted;
  }

  void setTicksSinceShouted(int i)
  {
    ticksSinceShouted = i;
  }

  int movedPerpendicular()
  {
    return perpendicularTicks;
  }

  void setMovedPerpendicular(int i)
  {
    perpendicularTicks = i;
  }

  int getNumSquaresToMoveInCurDirection()
  {
    return numSquaresToMoveInCurDirection;
  }

  void setNumSquaresToMoveInCurDirection(int i)
  {
    numSquaresToMoveInCurDirection = i;
  }
private:
  bool leaveField;
  int restTicks;
  int ticksSinceShouted;
  int perpendicularTicks;
  int numSquaresToMoveInCurDirection;
};

class RegularProtester : public Protester
{
public:
  RegularProtester(StudentWorld* world) : Protester(IID_PROTESTER, world, 5)
  {
  }

  virtual ~RegularProtester()
  {
  }

  virtual bool receiveGold();

  virtual bool getAnnoyed(int damage);
};

class HardcoreProtester : public Protester
{
public:
  HardcoreProtester(StudentWorld* world) : Protester(IID_HARD_CORE_PROTESTER, world, 20)
  {
  }

  virtual ~HardcoreProtester()
  {
  }

  virtual bool hardcoreCalculate();

  virtual bool receiveGold();

  virtual bool getAnnoyed(int damage);
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

class Buried : public Goodie
{
public:
  Buried(int imageID, int x, int y, StudentWorld* world) : Goodie(imageID, x, y, world)
  {
    setVisible(false);
    setHidden(true);
  }

  virtual ~Buried()
  {
  }

  virtual void doSomething()
  {
    Actor::doSomething();
  }

  void showSelf();
};

class OilBarrel : public Buried
{
public:
    OilBarrel(int startX, int startY, StudentWorld* world) : Buried(IID_BARREL, startX, startY, world)
    {
    }
    virtual ~OilBarrel()
    {
    }
    virtual void doSomething();
};

class GoldNugget : public Buried
{
public:
  GoldNugget(int startX, int startY, bool bribe, StudentWorld* world) : Buried(IID_GOLD, startX, startY, world)
  {
    myBribe = bribe;
    if(myBribe)
    {
      setVisible(true);
      ticks = 100;
    }
    else
      ticks = -1;
  }
  virtual ~GoldNugget()
  {
  }
  virtual void doSomething();
private:
  bool myBribe;
  int ticks;
};

class Squirt : public Actor
{
public:
  Squirt(int x, int y, Direction dir, StudentWorld* world) : Actor(IID_WATER_SPURT, x, y, world, dir, 1.0, 1)
  {
    myDir = dir;
    distance = 0;
  }
  virtual ~Squirt()
  {
  }
  virtual void doSomething();
private:
  int distance;
  Direction myDir;
};

#endif // ACTOR_H_
