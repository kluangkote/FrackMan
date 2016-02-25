#include "Actor.h"
#include "StudentWorld.h"

void Actor::moveInDirection(Direction dir)
{
  if(dir == down)
    moveTo(getX(), getY()-1);
  if(dir == up)
    moveTo(getX(), getY()+1);
  if(dir == left)
    moveTo(getX()-1, getY());
  if(dir == right)
    moveTo(getX()+1, getY());
}

Protester::Protester(int imageID, StudentWorld* world, int health)
: People(imageID, 60, 60, world, left, 1.0, 0, health)
{
  leaveField = false;
  int i = 3-getWorld()->getLevel()/4;
  restTicks = max(0, i);
  ticksSinceShouted = 15;
  perpendicularTicks = 0;
  numSquaresToMoveInCurDirection = rand() % 61 + 8;
}

bool Protester::clearPath(Direction directionOfFrack)
{
  bool canGoToFrack = true;
  int potentialX = getX();
  int potentialY = getY();
  while(getWorld()->getRadius(potentialX, potentialY, getWorld()->getFrackX(), getWorld()->getFrackY()) > 4)
  {
    if(!getWorld()->canMove(potentialX, potentialY, directionOfFrack)
    || getWorld()->touchingBoulder(potentialX, potentialY, directionOfFrack, 3, this))
    {
      canGoToFrack = false;
      break;
    }
    if(directionOfFrack == left)
      potentialX--;
    if(directionOfFrack == right)
      potentialX++;
    if(directionOfFrack == up)
      potentialY++;
    if(directionOfFrack == down)
      potentialY--;
  }
  if(canGoToFrack)
    return true;
  return false;
}

void Protester::doSomething()
{
  Actor::doSomething();
  if(getRestTicks() > 1)
  {
    setRestTicks(getRestTicks()-1);
    return;
  }
  if(getState())
  {
    if(getX() == 60 && getY() == 60)
      setAlive(false);
    else
    {
      Direction dir = getWorld()->getShortestDirection(getX(), getY(), false);
      if(getDirection() != dir)
        setDirection(dir);
      moveInDirection(getDirection());
      int i = 3-getWorld()->getLevel()/4;
      setRestTicks(max(0, i));
      return;
    }
  }
  else
  {
    StudentWorld* world = getWorld();
    Direction directionOfFrack;
    bool canYellLeft = getDirection() == left && getX() >= world->getFrackX() && getY() == world->getFrackY();
    bool canYellUp = getDirection() == up && getY() <= world->getFrackY() && getX() == world->getFrackX();
    bool canYellRight = getDirection() == right && getX() <= world->getFrackX() && getY() == world->getFrackY();
    bool canYellDown = getDirection() == down && getY() >= world->getFrackY() && getX() == world->getFrackX();
    if(world->getRadius(getX(), getY(), world->getFrackX(), world->getFrackY()) <= 4
    && (canYellLeft || canYellUp || canYellRight || canYellDown))
    {
      if(getTicksSinceShouted() >= 15)
      {
        setTicksSinceShouted(0);
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->annoyFrack(2);
        return;
      }
      else
      setTicksSinceShouted(getTicksSinceShouted()+1);
    }
    else if(world->getRadius(getX(), getY(), world->getFrackX(), world->getFrackY()) > 4 && hardcoreCalculate())
    {
      int i = 3-getWorld()->getLevel()/4;
      setRestTicks(max(0, i));
      return;
    }
    else if(getWorld()->seeFrack(getX(), getY(), directionOfFrack) && clearPath(directionOfFrack))
    {
      setDirection(directionOfFrack);
      moveInDirection(getDirection());
    }
    else
    {
      if(getNumSquaresToMoveInCurDirection() <= 0)
      {
        bool pickedNewDirection = false;
        while(!pickedNewDirection)
        {
          int newDirection = rand() % 4;
          if(newDirection == 0)
          {
            if(getWorld()->canMove(getX(), getY(), up) && !getWorld()->touchingBoulder(getX(), getY(), up, 3, this))
            {
              pickedNewDirection = true;
              setDirection(up);
            }
          }
          if(newDirection == 1)
          {
            if(getWorld()->canMove(getX(), getY(), down) && !getWorld()->touchingBoulder(getX(), getY(), down, 3, this))
            {
              pickedNewDirection = true;
              setDirection(down);
            }
          }
          if(newDirection == 2)
          {
            if(getWorld()->canMove(getX(), getY(), left) && !getWorld()->touchingBoulder(getX(), getY(), left, 3, this))
            {
              pickedNewDirection = true;
              setDirection(left);
            }
          }
          if(newDirection == 3)
          {
            if(getWorld()->canMove(getX(), getY(), right) && !getWorld()->touchingBoulder(getX(), getY(), right, 3, this))
            {
              pickedNewDirection = true;
              setDirection(right);
            }
          }
        }
        setNumSquaresToMoveInCurDirection(rand() % 61 + 8);
      }
      else
      {
        if(getDirection() == right || getDirection() == left)
        {
          if(((getWorld()->canMove(getX(), getY(), up) && !getWorld()->touchingBoulder(getX(), getY(), up, 3, this))
          || (getWorld()->canMove(getX(), getY(), down) && !getWorld()->touchingBoulder(getX(), getY(), down, 3, this)))
          && movedPerpendicular() >= 200)
          {
            setMovedPerpendicular(0);
            if(getWorld()->canMove(getX(), getY(), up) && !getWorld()->touchingBoulder(getX(), getY(), up, 3, this)
            && getWorld()->canMove(getX(), getY(), down) && !getWorld()->touchingBoulder(getX(), getY(), down, 3, this))
            {
              int dir = rand() % 2;
              if(dir == 0)
              setDirection(up);
              else
              setDirection(down);
            }
            else if(getWorld()->canMove(getX(), getY(), up) && !getWorld()->touchingBoulder(getX(), getY(), up, 3, this))
            setDirection(up);
            else
            setDirection(down);
            setNumSquaresToMoveInCurDirection(rand() % 61 + 8);
          }
          else
          setMovedPerpendicular(movedPerpendicular()+1);
        }
        if(getDirection() == up || getDirection() == down)
        {
          if(((getWorld()->canMove(getX(), getY(), left) && !getWorld()->touchingBoulder(getX(), getY(), left, 3, this))
          || (getWorld()->canMove(getX(), getY(), right) && !getWorld()->touchingBoulder(getX(), getY(), right, 3, this)))
          && movedPerpendicular() >= 200)
          {
            setMovedPerpendicular(0);
            if(getWorld()->canMove(getX(), getY(), left) && !getWorld()->touchingBoulder(getX(), getY(), left, 3, this)
            && getWorld()->canMove(getX(), getY(), right) && !getWorld()->touchingBoulder(getX(), getY(), right, 3, this))
            {
              int dir = rand() % 2;
              if(dir == 0)
              setDirection(left);
              else
              setDirection(right);
            }
            else if(getWorld()->canMove(getX(), getY(), left) && !getWorld()->touchingBoulder(getX(), getY(), left, 3, this))
            setDirection(left);
            else
            setDirection(right);
            setNumSquaresToMoveInCurDirection(rand() % 61 + 8);
          }
          else
          setMovedPerpendicular(movedPerpendicular()+1);
        }
      }
      setTicksSinceShouted(getTicksSinceShouted()+1);
      if(getWorld()->canMove(getX(), getY(), getDirection()) && !getWorld()->touchingBoulder(getX(), getY(), getDirection(), 3, this))
      {
        moveInDirection(getDirection());
        setNumSquaresToMoveInCurDirection(getNumSquaresToMoveInCurDirection()+1);
      }
      else
      setNumSquaresToMoveInCurDirection(0);
    }
  }
  int i = 3-getWorld()->getLevel()/4;
  setRestTicks(max(0, i));
}

bool Protester::getAnnoyed(int damage)
{
  setHealth(getHealth()-damage);
  if(getHealth() <= 0)
  {
    if(!getState())
      getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
    setStateToTrue();
    setRestTicks(0);
    if(damage == 100)
      getWorld()->addPointsToFrack(500);
  }
  else
  {
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    int i = 100-getWorld()->getLevel()*10;
    setRestTicks(max(50, i));
  }
  return true;
}

void FrackMan::doSomething()
{
  Actor::doSomething();
  int ch;
  if (getWorld()->getKey(ch) == true) {
    switch(ch){

      // DIRECTIONS
      case KEY_PRESS_LEFT:
      if(getDirection() != left)
        setDirection(left);
      else
      {
        if(getWorld()->touchingBoulder(getX(), getY(), left, 3, this))
          return;
        if(getX() > 0)
        {
          moveInDirection(left);
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
          moveInDirection(right);
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
          moveInDirection(up);
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
          moveInDirection(down);
          getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3, true);
        }
        else
          moveTo(getX(), getY());
      }
      break;

      // SONAR
      case 'Z':
      case 'z':
      if(sonar > 0)
      {
        getWorld()->sonar();
        sonar--;
      }
      break;

      // SQUIRT
      case KEY_PRESS_SPACE:
      if(water > 0)
      {
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        water--;
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
            getWorld()->addSquirt();
        }
        if(getDirection() == right)
        {
          waterX += 4;
          if(world->canMove(waterX-1, waterY, right) && !world->touchingBoulder(waterX, waterY, right, 3, this))
            getWorld()->addSquirt();
        }
        if(getDirection() == up)
        {
          waterY += 4;
          if(world->canMove(waterX, waterY-1, up) && !world->touchingBoulder(waterX, waterY, up, 3, this))
            getWorld()->addSquirt();
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
            getWorld()->addSquirt();
        }
      }
      break;

      // GOLD
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

bool FrackMan::getAnnoyed(int damage)
{
  setHealth(getHealth()-damage);
  if(getHealth() <= 0)
  {
    getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
  }
  return true;
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

bool RegularProtester::receiveGold()
{
  getWorld()->addPointsToFrack(25);
  getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
  setHealth(0);
  setStateToTrue();
  return true;
}

bool RegularProtester::getAnnoyed(int damage)
{
  Protester::getAnnoyed(damage);
  if(getHealth() <= 0 && damage == 2)
    getWorld()->addPointsToFrack(100);
  return true;
}

bool HardcoreProtester::receiveGold()
{
  getWorld()->addPointsToFrack(50);
  getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
  int i = 100-getWorld()->getLevel()*10;
  setRestTicks(max(50, i));
  return true;
}

bool HardcoreProtester::getAnnoyed(int damage)
{
  Protester::getAnnoyed(damage);
  if(getHealth() <= 0 && damage == 2)
    getWorld()->addPointsToFrack(250);
  return true;
}

bool HardcoreProtester::hardcoreCalculate()
{
  int M = 16+getWorld()->getLevel()*2;
  if(getWorld()->stepsAwayFromFrack(getX(), getY()) <= M)
  {
    Direction dir = getWorld()->getShortestDirection(getX(), getY(), true);
    if(getDirection() != dir)
      setDirection(dir);
    moveInDirection(getDirection());
    return true;
  }
  return false;
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
      if(getWorld()->pickUpGold(this))
      {
        setAlive(false);
      }
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
  if(getWorld()->annoyProtesters(this, 2))
    setAlive(false);
  if(getWorld()->touchingBoulder(getX(), getY(), myDir, 3, this) || !getWorld()->canMove(getX(), getY(), myDir))
  {
    setAlive(false);
  }
  else
  {
    distance++;
    moveInDirection(myDir);
  }
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
    getWorld()->annoyProtesters(this, 100);
    if(getWorld()->annoyFrack(100))
      setAlive(false);
    else if(!myWorld->touchingBoulder(getX(), getY(), down, 3, this) && myWorld->canMove(getX(), getY(), down))
      moveInDirection(down);
    else
      setAlive(false);
  }
}
