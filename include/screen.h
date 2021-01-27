#ifndef SCREEN_H
#define SCREEN_H

#include "input.h"

struct MenuButton
{
  std::string text;
  float height;

  std::function<void()> target;

  bool highlighted;
  bool pressed;

  MenuButton(std::string _text, float _height, std::function<void()> _target);

  void
  update();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class Screen
{
public:
  Screen();

  virtual void
  update(float time_elapsed) = 0;

  virtual void
  draw_custom() = 0;

  virtual void
  to_appear();

  virtual void
  to_disappear();
};

#endif
