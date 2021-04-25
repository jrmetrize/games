#ifndef SCREEN_H
#define SCREEN_H

#include "input.h"
#include "state.h"

struct MenuControl
{
  Vec2 origin;
  Vec2 size;

  MenuControl(Vec2 _origin, Vec2 _size);

  virtual void
  update() = 0;

  virtual void
  draw() = 0;

  virtual void
  mouse_pressed(MouseButton button, bool button_pressed) = 0;

  void
  play_highlight_sound();

  void
  play_confirm_sound();
};

class Screen
{
  Listener *listener;
protected:
  /* User input callbacks */
  virtual void
  mouse_button_update(MouseButton button, bool pressed);

  virtual void
  scroll_update(Vec2 scroll);

  virtual void
  key_update(Key key, bool pressed);

  virtual void
  gamepad_button_update(GamepadButton button, bool presed);

  virtual void
  char_update(unsigned int codepoint);
public:
  Screen();

  virtual
  ~Screen();

  virtual void
  update(float time_elapsed) = 0;

  virtual void
  draw() = 0;

  virtual void
  resize(Vec2 size);

  void
  install_listener();

  void
  remove_listener();

  virtual void
  to_appear();

  virtual void
  to_disappear();
};

struct MenuButton : public MenuControl
{
  std::string text;

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

struct MenuSwitch : public MenuControl
{
  bool value;

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

struct MenuSlider : public MenuControl
{
  float value;

  std::function<void(float)> value_changed;

  bool highlighted;
  bool pressed;

  MenuSlider(Vec2 _origin, Vec2 _size, std::function<void(float)> _value_changed);

  void
  update();

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

struct MenuSelector : public MenuControl
{
  PropertyData *property;

  int highlighted;
  int pressed;

  MenuSelector(Vec2 _origin, Vec2 _size, PropertyData *_property);

  void
  update();

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

struct TextLine : public MenuControl
{
  std::string text;
  uint32_t cursor_pos;

  bool highlighted;
  bool pressed;

  TextLine(Vec2 _origin, Vec2 _size, std::string _text);

  void
  update();

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

struct ColorSelector : public MenuControl
{
  void
  update();

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

#endif
