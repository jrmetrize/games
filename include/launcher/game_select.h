#ifndef GAME_SELECT_H
#define GAME_SELECT_H

#include "core/screen.h"

namespace Launcher
{

class GameSelectScreen : public Screen
{
public:
  GameSelectScreen();

  ~GameSelectScreen();

  void
  update(float time_elapsed);

  void
  draw_custom();
};

}

#endif
