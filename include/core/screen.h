#ifndef SCREEN_H
#define SCREEN_H

#include "input.h"
#include "state.h"

class MenuControl
{
  std::vector<MenuControl *> children;

  MenuControl *tab_prev;
  MenuControl *tab_next;
protected:
  Vec2 origin;
  Vec2 size;

  bool hovered;
  bool focused;
public:
  MenuControl();

  MenuControl(Vec2 _origin, Vec2 _size);

  void
  set_hovered(bool _hovered);

  bool
  is_hovered() const;

  void
  set_focused(bool _focused);

  bool
  is_focused() const;

  /* Tab list */
  MenuControl *
  get_tab_prev();

  MenuControl *
  get_tab_next();

  void
  set_tab_prev(MenuControl *_prev);

  void
  set_tab_next(MenuControl *_next);

  /* Set the controls in this list to have each other as tab neighbors in
     the specified order. To make a cyclic list, make the first and last
     elements identical. */
  static void
  make_tab_list(std::vector<MenuControl *> controls);

  /* TODO: How do we handle this where the answer is not necessarily
     well-defined? Is it worth writing code to deal with this? */
  MenuControl *
  get_control_under(Vec2 position);

  virtual void
  update(float time_elapsed) = 0;

  virtual void
  draw() = 0;

  void
  update_children(float time_elapsed);

  void
  draw_children();

  /* Subclass helpers */
  void
  add_child(MenuControl *child);

  void
  remove_child(MenuControl *child);

  /* User input callbacks */
  virtual void
  mouse_button_update(MouseButton button, bool pressed);

  virtual void
  cursor_update(Vec2 position);

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

class Screen : public MenuControl
{
  Listener *listener;

  MenuControl *hovered_control;
  MenuControl *active_control;

  /* Called first, then passed to subclass-defined behavior */
  void
  _mouse_button_update(MouseButton button, bool pressed);

  void
  _cursor_update(Vec2 position);

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
  MenuControl *
  get_active_control();

  void
  set_active_control(MenuControl *_active_control);
public:
  Screen();

  virtual
  ~Screen();

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

  std::string
  get_text() const;
};

class ColorSelector : public MenuControl
{
  TextLine *r_component;
  TextLine *g_component;
  TextLine *b_component;
public:
  ColorSelector();

  ~ColorSelector();

  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

#endif
