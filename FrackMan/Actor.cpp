#include "Actor.h"
#include "StudentWorld.h"

void Dirt::doSomething()
{
}

void FrackMan::doSomething()
{
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
            getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3);
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
              getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3);
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
            getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3);
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
            getWorld()->destroyDirt(getX(), getY(), getX()+3, getY()+3);
          }
          else
            moveTo(getX(), getY());
        }
        break;
    }
  }
}
