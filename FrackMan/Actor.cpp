#include "Actor.h"
#include "StudentWorld.h"

void Dirt::doSomething()
{
}

void FrackMan::doSomething()
{
  int ch;
  if (myWorld->getKey(ch) == true) {
    switch(ch){
      case KEY_PRESS_LEFT:
        if(getX() > 0)
        {
          moveTo(getX()-1, getY());
          myWorld->destroyDirt(getX(), getY(), getX()+3, getY()+3);
        }
        break;
      case KEY_PRESS_RIGHT:
        if(getX() < 60)
        {
          moveTo(getX()+1, getY());
          myWorld->destroyDirt(getX(), getY(), getX()+3, getY()+3);
        }
        break;
      case KEY_PRESS_UP:
        if(getY() < 60)
        {
          moveTo(getX(), getY()+1);
          myWorld->destroyDirt(getX(), getY(), getX()+3, getY()+3);
        }
        break;
      case KEY_PRESS_DOWN:
        if(getY() > 0)
        {
          moveTo(getX(), getY()-1);
          myWorld->destroyDirt(getX(), getY(), getX()+3, getY()+3);
        }
        break;
    }
  }
}
