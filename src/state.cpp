#include "state.h"

GameState::GameState() :
  should_close(false)
{

}

GameState::~GameState()
{

}

void
GameState::close_game()
{
  should_close = true;
}

bool
GameState::game_open() const
{
  return !should_close;
}
