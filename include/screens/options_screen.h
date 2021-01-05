#ifndef OPTIONS_SCREEN_H
#define OPTIONS_SCREEN_H

#include "screen.h"

class OptionsScreen : public Screen
{
public:
  OptionsScreen(GameState *_state);

  ~OptionsScreen();

  void
  draw_custom(GraphicsServer *graphics_server);
};

#endif
