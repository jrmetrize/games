#ifndef LEVEL_EDITOR_SCREEN_H
#define LEVEL_EDITOR_SCREEN_H

#include "core/screen.h"

class TrainingLevelController;

class LevelEditorScreen : public Screen
{
  TrainingLevelController *level;

  bool paused;
public:
  LevelEditorScreen();

  ~LevelEditorScreen();

  void
  update(float time_elapsed);

  void
  draw_custom();
};

#endif
