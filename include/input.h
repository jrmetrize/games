#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#include "linear_algebra.h"

enum Key
{
  KeyA,
  KeyB,
  KeyC,
  KeyD,
  KeyE,
  KeyF,
  KeyG,
  KeyH,
  KeyI,
  KeyJ,
  KeyK,
  KeyL,
  KeyM,
  KeyN,
  KeyO,
  KeyP,
  KeyQ,
  KeyR,
  KeyS,
  KeyT,
  KeyU,
  KeyV,
  KeyW,
  KeyX,
  KeyY,
  KeyZ,
  Key1,
  Key2,
  Key3,
  Key4,
  Key5,
  Key6,
  Key7,
  Key8,
  Key9,
  Key0,
  KeyF1,
  KeyF2,
  KeyF3,
  KeyF4,
  KeyF5,
  KeyF6,
  KeyF7,
  KeyF8,
  KeyF9,
  KeyF10,
  KeyF11,
  KeyF12,
  KeyEscape,
  KeyTilde,
  KeyMinus,
  KeyEquals,
  KeyBackspace,
  KeyTab,
  KeyLeftBracket,
  KeyRightBracket,
  KeyBackslash,
  KeySemicolon,
  KeyApostrophe,
  KeyEnter,
  KeyComma,
  KeyPeriod,
  KeySlash,
  KeySpace,
  KeyUp,
  KeyDown,
  KeyLeft,
  KeyRight
};

enum GamepadButton
{
  ButtonA,
  ButtonB,
  ButtonX,
  ButtonY,
  DpadUp,
  DpadDown,
  DpadLeft,
  DpadRight,
  BumperL,
  BumperR,
  StickLeft,
  StickRight,
  ButtonStart,
  ButtonBack,
  ButtonMenu
};

class GameState;

class InputMonitor
{
  GLFWwindow *window;

  GameState *state;

  float
  input_threshold(float input, float threshold) const;

  float
  joystick_threshold(float input) const;
public:
  InputMonitor(GLFWwindow *_window, GameState *_state);

  ~InputMonitor();

  void
  close_window_pressed();

  bool
  is_key_down(Key key) const;

  Vec2
  get_mouse_position() const;

  bool
  is_left_mouse_down() const;

  bool
  is_right_mouse_down() const;

  bool
  is_middle_mouse_down() const;

  bool
  is_gamepad_available() const;

  Vec2
  gamepad_left_stick() const;

  Vec2
  gamepad_right_stick() const;

  float
  gamepad_left_trigger() const;

  float
  gamepad_right_trigger() const;

  bool
  is_gamepad_button_down(GamepadButton button) const;

  bool
  get_jump_input() const;
};

#endif
