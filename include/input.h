#ifndef INPUT_H
#define INPUT_H

// Include glad first to avoid errors with glfw's OpenGL declarations
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <optional>
#include <vector>

#include "linear_algebra.h"

enum MouseButton
{
  MouseButtonLeft,
  MouseButtonMiddle,
  MouseButtonRight
};

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

struct Listener
{
  std::optional<std::function<void(MouseButton, bool)>> mouse_button_handle;
  std::optional<std::function<void(Key, bool)>> key_handle;
  std::optional<std::function<void(GamepadButton, bool)>> gamepad_button_handle;
};

class InputMonitor
{
  static InputMonitor *instance;

  GLFWwindow *window;

  std::vector<Listener *> listeners;

  float
  input_threshold(float input, float threshold) const;

  float
  joystick_threshold(float input) const;
public:
  InputMonitor(GLFWwindow *_window);

  ~InputMonitor();

  static void
  set_instance(InputMonitor *_instance);

  static InputMonitor *
  get();

  void
  install_listener(Listener *listener);

  void
  remove_listener(Listener *listener);

  void
  mouse_button_press(MouseButton button, bool press);

  void
  key_changed(Key key, bool press);

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

class Button
{
  Listener listener;

  Vec2 origin;
  Vec2 size;

  bool enabled;
  bool state;

  std::function<void(void *)> callback;
  void *userdata;

  void
  mouse_button(MouseButton button, bool pressed);
public:
  Button(Vec2 _origin, Vec2 _size,
    const std::function<void(void *)> _callback, void *_userdata = nullptr);

  void
  set_origin(Vec2 _origin);

  void
  set_size(Vec2 _size);

  void
  set_enabled(bool _enabled);

  ~Button();
};

#endif
