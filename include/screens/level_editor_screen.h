#ifndef LEVEL_EDITOR_SCREEN_H
#define LEVEL_EDITOR_SCREEN_H

#include "screen.h"

class LevelEditorScreen : public Screen
{
public:
  LevelEditorScreen(GameState *_state);

  ~LevelEditorScreen();

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
