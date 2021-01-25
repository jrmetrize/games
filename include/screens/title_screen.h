#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "screen.h"
#include "input.h"
#include <string>

class TitleScreen : public Screen
{
  Listener listener;

  void
  draw_button(float y, std::string text);

  void
  mouse_pressed(MouseButton button, bool pressed);
public:
  TitleScreen();

  ~TitleScreen();

  void
  update(float time_elapsed);

  void
  draw_custom();
};

#endif
