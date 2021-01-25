#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include "screen.h"

class TrainingLevelController;

class LevelScreen : public Screen
{
  TrainingLevelController *level;
public:
  LevelScreen();

  ~LevelScreen();

  void
  update(float time_elapsed);

  void
  draw_custom();
};

#endif
