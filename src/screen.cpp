#include "screen.h"

Screen::Screen(GameState *_state) :
  state(_state)
{

}

GameState *
Screen::get_state()
{
  return state;
}
