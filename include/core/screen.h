#ifndef SCREEN_H
#define SCREEN_H

#include "input.h"
#include "state.h"

class MenuControl
{
protected:
  Vec2 origin;
  Vec2 size;
public:
  MenuControl();

  MenuControl(Vec2 _origin, Vec2 _size);

  virtual void
  update(float time_elapsed) = 0;

  virtual void
  draw() = 0;

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

  void
  play_highlight_sound();

  void
  play_confirm_sound();
};

class Screen
{
  Listener *listener;

  std::vector<MenuControl *> controls;
  MenuControl *active_control;

  /* Called first, then passed to subclass-defined behavior */
  void
  _mouse_button_update(MouseButton button, bool pressed);

  void
  _scroll_update(Vec2 scroll);

  void
  _key_update(Key key, bool pressed);

  void
  _gamepad_button_update(GamepadButton button, bool presed);

  void
  _char_update(unsigned int codepoint);
protected:
  /* Subclass helpers */
  void
  add_control(MenuControl *control);

  void
  remove_control(MenuControl *control);

  MenuControl *
  get_active_control();

  void
  set_active_control(MenuControl *_active_control);

  void
  update_controls(float time_elapsed);

  void
  draw_controls();

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
  update(float time_elapsed);

  virtual void
  draw();

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

class MenuButton : public MenuControl
{
  std::string text;

  std::function<void()> target;

  bool highlighted;
  bool pressed;
public:
  MenuButton(std::string _text, Vec2 _origin, Vec2 _size, std::function<void()> _target);

  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class MenuSwitch : public MenuControl
{
  bool value;

  std::function<void(bool)> value_changed;

  int highlighted;
  int pressed;
public:
  MenuSwitch(Vec2 _origin, Vec2 _size, std::function<void(bool)> _value_changed);

  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class MenuSlider : public MenuControl
{
  float value;

  std::function<void(float)> value_changed;

  bool highlighted;
  bool pressed;
public:
  MenuSlider(Vec2 _origin, Vec2 _size, std::function<void(float)> _value_changed);

  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class MenuSelector : public MenuControl
{
  PropertyData *property;

  int highlighted;
  int pressed;
public:
  MenuSelector(Vec2 _origin, Vec2 _size, PropertyData *_property);

  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class TextLine : public MenuControl
{
  std::string text;
  uint32_t cursor_pos;

  bool highlighted;
  bool pressed;
public:
  TextLine(Vec2 _origin, Vec2 _size, std::string _text);

  void
  update(float time_elapsed);

  void
  draw();

  void
  key_update(Key key, bool pressed);

  void
  char_update(unsigned int codepoint);

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class ColorSelector : public MenuControl
{
public:
  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

#endif
