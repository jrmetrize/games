#ifndef SCREEN_H
#define SCREEN_H

#include "input.h"

struct MenuButton
{
  std::string text;
  Vec2 origin;
  Vec2 size;

  std::function<void()> target;

  bool highlighted;
  bool pressed;

  MenuButton(std::string _text, Vec2 _origin, Vec2 _size, std::function<void()> _target);

  void
  update();

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

struct MenuSwitch
{
  bool value;
  Vec2 origin;
  Vec2 size;

  std::function<void(bool)> value_changed;

  int highlighted;
  int pressed;

  MenuSwitch(Vec2 _origin, Vec2 _size, std::function<void(bool)> _value_changed);

  void
  update();

  void
  draw();

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
