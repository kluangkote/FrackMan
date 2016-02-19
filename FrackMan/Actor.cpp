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

void Boulder::doSomething()
{
  Actor::doSomething();
  StudentWorld* myWorld = getWorld();
  if(state == "stable")
  {
    Dirt* dirt1 = myWorld->getDirt(getX(), getY()-1);
    Dirt* dirt2 = myWorld->getDirt(getX()+1, getY()-1);
    Dirt* dirt3 = myWorld->getDirt(getX()+2, getY()-1);
    Dirt* dirt4 = myWorld->getDirt(getX()+3, getY()-1);
    if(dirt1 == nullptr && dirt2 == nullptr && dirt3 == nullptr && dirt4 == nullptr)
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
    Dirt* dirt1 = myWorld->getDirt(getX(), getY()-1);
    Dirt* dirt2 = myWorld->getDirt(getX()+1, getY()-1);
    Dirt* dirt3 = myWorld->getDirt(getX()+2, getY()-1);
    Dirt* dirt4 = myWorld->getDirt(getX()+3, getY()-1);
    if(getY() > 0 && dirt1 == nullptr && dirt2 == nullptr && dirt3 == nullptr && dirt4 == nullptr)
      moveTo(getX(), getY()-1);
    else
      setAlive(false);
  }
}
