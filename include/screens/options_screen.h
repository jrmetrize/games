#ifndef OPTIONS_SCREEN_H
#define OPTIONS_SCREEN_H

#include "screen.h"

class OptionsScreen : public Screen
{
public:
  OptionsScreen();

  ~OptionsScreen();

  void
  update(float time_elapsed);

  void
  draw_custom();
};

#endif
