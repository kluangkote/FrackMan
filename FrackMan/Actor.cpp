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
          moveTo(getX()-1, getY());
        break;
      case KEY_PRESS_RIGHT:
          moveTo(getX()+1, getY());
        break;
      case KEY_PRESS_UP:
        moveTo(getX(), getY()+1);
        break;
      case KEY_PRESS_DOWN:
        moveTo(getX(), getY()-1);
        break;
    }
  }
}
