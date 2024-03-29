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
  bool sh = true, bool annoy = false);
  virtual ~Actor()
  {
  }
  virtual void doSomething();
  virtual void getAnnoyed(int damage)
  {
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
  virtual void receiveGold()
  {
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

class Goodie : public Actor
{
public:
  Goodie(int imageID, int x, int y, StudentWorld* world, int sound = SOUND_GOT_GOODIE)
  : Actor(imageID, x, y, world, right, 1, 2)
  {
    mySound = sound;
  }
  virtual ~Goodie()
  {
  }
  virtual void doSomething();
private:
  int mySound;
};

class PopUpGoodie : public Goodie
{
public:
  PopUpGoodie(int imageID, int x, int y, StudentWorld* world);
  virtual ~PopUpGoodie()
  {
  }
  virtual void doSomething();
  bool pickedUp();
private:
  int ticks;
  bool pickedUpByFrack;
};

class Buried : public Goodie
{
public:
  Buried(int imageID, int x, int y, StudentWorld* world, int sound = SOUND_GOT_GOODIE);
  virtual ~Buried()
  {
  }
  void showSelf();
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
  FrackMan(StudentWorld* world);
  virtual ~FrackMan()
  {
  }
  virtual void doSomething();
  virtual void getAnnoyed(int damage);
  void addWater();
  void addSonar();
  void addGold();
  int getWater();
  int getSonar();
  int getGold();
private:
  int water;
  int sonar;
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
  virtual void getAnnoyed(int damage);
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
  virtual void receiveGold();
  virtual void getAnnoyed(int damage);
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
  virtual void receiveGold();
  virtual void getAnnoyed(int damage);
};

class Boulder : public Actor
{
public:
  Boulder(int x, int y, StudentWorld* world);
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

class OilBarrel : public Buried
{
public:
    OilBarrel(int startX, int startY, StudentWorld* world) : Buried(IID_BARREL, startX, startY, world, SOUND_FOUND_OIL)
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
  GoldNugget(int startX, int startY, bool bribe, StudentWorld* world);
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
  Squirt(int x, int y, Direction dir, StudentWorld* world);
  virtual ~Squirt()
  {
  }
  virtual void doSomething();
private:
  int distance;
  Direction myDir;
};

#endif // ACTOR_H_
