#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "core/screen.h"
#include "core/input.h"
#include <string>
#include <functional>

class TitleScreen : public Screen
{
  Listener listener;

  MenuButton *play_button;
  MenuButton *options_button;
  MenuButton *quit_button;

  void
  draw_button(MenuButton *button);

  void
  mouse_pressed(MouseButton button, bool pressed);
public:
  TitleScreen();

  ~TitleScreen();

  void
  update(float time_elapsed);

  void
  to_appear();

  void
  to_disappear();

  void
  draw_custom();

  void
  button_pressed(int button);
};

#endif
