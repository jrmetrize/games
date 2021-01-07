#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "screen.h"

class TitleScreen : public Screen
{
public:
  TitleScreen(GameState *_state);

  ~TitleScreen();

  void
  update(InputMonitor *input, float time_elapsed);

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
