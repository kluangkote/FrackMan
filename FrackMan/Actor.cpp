


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
        if(getWorld()->touchingBoulder(getX(), getY(), left, 3, this))
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
        if(getWorld()->touchingBoulder(getX(), getY(), right, 3, this))
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
        if(getWorld()->touchingBoulder(getX(), getY(), up, 3, this))
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
        if(getWorld()->touchingBoulder(getX(), getY(), down, 3, this))
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
      case 'Z':
      case 'z':
      if(sonar > 0)
      {
        getWorld()->sonar();
        sonar--;
      }
      break;
      case KEY_PRESS_SPACE:
      if(water > 0)
      {
        int waterX = getX();
        int waterY = getY();
        bool canPlace = true;
        StudentWorld* world = getWorld();
        if(getDirection() == left)
        {
          if(waterX-4 < 0)
          {
            canPlace = false;
            break;
          }
          for(int i = waterX; i > waterX-4; i--)
          {
            if(!world->canMove(i, waterY, left))
            {
              canPlace = false;
              break;
            }
          }
          if(canPlace && !world->touchingBoulder(waterX-4, waterY, left, 3, this))
          {
            getWorld()->addSquirt();
            water--;
          }
        }
        if(getDirection() == right)
        {
          waterX += 4;
          if(world->canMove(waterX-1, waterY, right) && !world->touchingBoulder(waterX, waterY, right, 3, this))
          {
            getWorld()->addSquirt();
            water--;
          }
        }
        if(getDirection() == up)
        {
          waterY += 4;
          if(world->canMove(waterX, waterY-1, up) && !world->touchingBoulder(waterX, waterY, up, 3, this))
          {
            getWorld()->addSquirt();
          }
        }
        if(getDirection() == down)
        {
          waterY -= 4;
          if(waterY < 0)
          {
            canPlace = false;
            break;
          }
          for(int i = waterY; i < waterY+4; i++)
          {
            if(!world->canMove(waterX, i-1, up))
            {
              canPlace = false;
              break;
            }
          }
          if(canPlace && !world->touchingBoulder(waterX, waterY, down, 3, this))
          {
            getWorld()->addSquirt();
            water--;
          }
        }
      }
      break;
      case KEY_PRESS_TAB:
      if(gold > 0)
      {
        getWorld()->addBribe();
        gold--;
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
  sonar += 2;
}

void FrackMan::addPoints(int pointsToAdd)
{
  points += pointsToAdd;
}

void FrackMan::addGold()
{
  gold++;
}

int FrackMan::getWater()
{
  return water;
}

int FrackMan::getSonar()
{
  return sonar;
}
int FrackMan::getPoints()
{
  return points;
}

int FrackMan::getGold()
{
  return gold;
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
    if(!myWorld->touchingBoulder(getX(), getY(), down, 3, this) && myWorld->canMove(getX(), getY(), down))
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

void Buried::showSelf()
{
  int frackX = getWorld()->getFrackX();
  int frackY = getWorld()->getFrackY();
  if(isHidden() && getWorld()->getRadius(getX(), getY(), frackX, frackY) <= 4)
  {
    setVisible(true);
    setHidden(false);
    return;
  }
}

void OilBarrel::doSomething()
{
  showSelf();
  Goodie::doSomething(SOUND_FOUND_OIL);
  if(!isActorAlive())
  {
    getWorld()->addPointsToFrack(1000);
    getWorld()->barrelFound();
  }
}

void GoldNugget::doSomething()
{
  if(!myBribe)
  {
    showSelf();
    Goodie::doSomething();
    if(!isActorAlive())
    {
      getWorld()->addPointsToFrack(10);
      getWorld()->addGold();
    }
  }
  else
  {
    if(ticks > 0)
    {
      ticks--;
    }
    else
      setAlive(false);
  }
}

void Squirt::doSomething()
{
  if(distance == 4)
  {
    setAlive(false);
    return;
  }
  if(getWorld()->touchingBoulder(getX(), getY(), myDir, 3, this) || !getWorld()->canMove(getX(), getY(), myDir))
  {
    setAlive(false);
  }
  else
  {
    distance++;
    if(myDir == up)
      moveTo(getX(), getY()+1);
    if(myDir == down)
      moveTo(getX(), getY()-1);
    if(myDir == left)
      moveTo(getX()-1, getY());
    if(myDir == right)
      moveTo(getX()+1, getY());
  }
}
