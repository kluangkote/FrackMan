#include "Actor.h"
#include "StudentWorld.h"

void FrackMan::doSomething()
{
  Actor::doSomething();
  int ch;
  if (getWorld()->getKey(ch) == true) {
    switch(ch){
      case KEY_PRESS_LEFT:
      if(getDirection() != left)
        setDirection(left);
      else
      {
        if(getWorld()->touchingBoulder(getX()-1, getY(), 3, this))
          return;
        if(getX() > 0)
        {
          moveTo(getX()-1, getY());
          getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3, true);
        }
        else
          moveTo(getX(), getY());
      }
      break;
      case KEY_PRESS_RIGHT:
      if(getDirection() != right)
        setDirection(right);
      else
      {
        if(getWorld()->touchingBoulder(getX()+1, getY(), 3, this))
          return;
        if(getX() < 60)
        {
          moveTo(getX()+1, getY());
          getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3, true);
        }
        else
          moveTo(getX(), getY());
      }
      break;
      case KEY_PRESS_UP:
      if(getDirection() != up)
        setDirection(up);
      else
      {
        if(getWorld()->touchingBoulder(getX(), getY()+1, 3, this))
          return;
        if(getY() < 60)
        {
          moveTo(getX(), getY()+1);
          getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3, true);
        }
        else
          moveTo(getX(), getY());
      }
      break;
      case KEY_PRESS_DOWN:
      if(getDirection() != down)
        setDirection(down);
      else
      {
        if(getWorld()->touchingBoulder(getX(), getY()-1, 3, this))
          return;
        if(getY() > 0)
        {
          moveTo(getX(), getY()-1);
          getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3, true);
        }
        else
          moveTo(getX(), getY());
      }
      break;
    }
  }
}

void FrackMan::addWater()
{
  water += 5;
}

void FrackMan::addSonar()
{
  sonar++;
}

void FrackMan::addPoints(int pointsToAdd)
{
  points += pointsToAdd;
}

void Goodie::doSomething(int sound = SOUND_GOT_GOODIE)
{
  int frackX = getWorld()->getFrackX();
  int frackY = getWorld()->getFrackY();
  if(getWorld()->getRadius(getX(), getY(), frackX, frackY) <= 3)
  {
    getWorld()->playSound(sound);
    setAlive(false);
  }
}

PopUpGoodie::PopUpGoodie(int imageID, int x, int y, StudentWorld* world)
: Goodie(imageID, x, y , world)
{
  int i = 300-10*getWorld()->getLevel();
  ticks = max(100, i);
}

void PopUpGoodie::doSomething()
{
  ticks--;
  Goodie::doSomething();
  if(ticks == 0)
    setAlive(false);
}

void Boulder::doSomething()
{
  Actor::doSomething();
  StudentWorld* myWorld = getWorld();
  if(state == "stable")
  {
    if(myWorld->canMove(getX(), getY(), down))
      state = "waiting";
    else
      return;
  }
  if(state == "waiting")
  {
    countTicks++;
    if(countTicks == 30)
    {
      state = "falling";
      getWorld()->playSound(SOUND_FALLING_ROCK);
    }
  }
  if(state == "falling")
  {
    if(!myWorld->touchingBoulder(getX(), getY()-1, 3, this) && myWorld->canMove(getX(), getY(), down))
      moveTo(getX(), getY()-1);
    else
      setAlive(false);
  }
}

void WaterPool::doSomething()
{
    PopUpGoodie::doSomething();
    if(!isActorAlive())
      getWorld()->addWaterToGun();
}

void SonarKit::doSomething()
{
  PopUpGoodie::doSomething();
  if(!isActorAlive())
    getWorld()->addSonarKit();
}

void OilBarrel::doSomething()
{
  Actor::doSomething();
  int frackX = getWorld()->getFrackX();
  int frackY = getWorld()->getFrackY();
  if(hidden() && getWorld()->getRadius(getX(), getY(), frackX, frackY) <= 4)
  {
    setVisible(true);
    setStatusToVisible();
    return;
  }
  Goodie::doSomething(SOUND_FOUND_OIL);
  if(!isActorAlive())
  {
    getWorld()->addPointsToFrack(1000);
    getWorld()->barrelFound();
  }
}
