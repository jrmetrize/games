#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include "screen.h"

class LevelScreen : public Screen
{
public:
  LevelScreen(GameState *_state);

  ~LevelScreen();

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
