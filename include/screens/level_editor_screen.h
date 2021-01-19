#ifndef LEVEL_EDITOR_SCREEN_H
#define LEVEL_EDITOR_SCREEN_H

#include "screen.h"

class TrainingLevelController;

class LevelEditorScreen : public Screen
{
  TrainingLevelController *level;

  bool paused;
public:
  LevelEditorScreen(GameState *_state);

  ~LevelEditorScreen();

  void
  update(InputMonitor *input, float time_elapsed);

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
