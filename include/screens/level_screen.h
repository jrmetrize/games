#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include "screen.h"

class Level;
class LevelState;

class LevelScreen : public Screen
{
  Level *level;
  LevelState *level_state;
public:
  LevelScreen(GameState *_state);

  ~LevelScreen();

  void
  update(InputMonitor *input, float time_elapsed);

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
