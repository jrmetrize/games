#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include "screen.h"

class TrainingLevelController;

class LevelScreen : public Screen
{
  TrainingLevelController *level;
public:
  LevelScreen(GameState *_state);

  ~LevelScreen();

  void
  update(InputMonitor *input, float time_elapsed);

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
