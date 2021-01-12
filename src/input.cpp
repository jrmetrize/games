#include "input.h"
#include "state.h"

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

}

static void
char_callback(GLFWwindow *window, unsigned int codepoint)
{

}

static void
mouse_callback(GLFWwindow *window, double mouse_x, double mouse_y)
{

}

static void
mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{

}

static void
scroll_callback(GLFWwindow *window, double scroll_x, double scroll_y)
{

}

static void
window_close_callback(GLFWwindow *window)
{
  InputMonitor *input = (InputMonitor *)glfwGetWindowUserPointer(window);
  // TODO: check that this pointer is not nullptr
  input->close_window_pressed();
}

InputMonitor::InputMonitor(GLFWwindow *_window, GameState *_state) :
  window(_window),
  state(_state)
{
  //glfwSetKeyCallback(window, key_callback);
  //glfwSetCharCallback(window, char_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetWindowCloseCallback(window, window_close_callback);

  glfwSetWindowUserPointer(window, this);
}

float
InputMonitor::input_threshold(float input, float threshold) const
{
  return (fabs(input) >= threshold) ? input : 0.0f;
}

float
InputMonitor::joystick_threshold(float input) const
{
  return input_threshold(input, 0.001f);
}

InputMonitor::~InputMonitor()
{
  glfwSetWindowUserPointer(window, nullptr);
}

void
InputMonitor::close_window_pressed()
{
  state->close_game();
}

bool
InputMonitor::is_key_down(Key key) const
{
  int key_code;
  switch (key)
  {
    case KeyA: key_code = GLFW_KEY_A; break;
    case KeyB: key_code = GLFW_KEY_B; break;
    case KeyC: key_code = GLFW_KEY_C; break;
    case KeyD: key_code = GLFW_KEY_D; break;
    case KeyE: key_code = GLFW_KEY_E; break;
    case KeyF: key_code = GLFW_KEY_F; break;
    case KeyG: key_code = GLFW_KEY_G; break;
    case KeyH: key_code = GLFW_KEY_H; break;
    case KeyI: key_code = GLFW_KEY_I; break;
    case KeyJ: key_code = GLFW_KEY_J; break;
    case KeyK: key_code = GLFW_KEY_K; break;
    case KeyL: key_code = GLFW_KEY_L; break;
    case KeyM: key_code = GLFW_KEY_M; break;
    case KeyN: key_code = GLFW_KEY_N; break;
    case KeyO: key_code = GLFW_KEY_O; break;
    case KeyP: key_code = GLFW_KEY_P; break;
    case KeyQ: key_code = GLFW_KEY_Q; break;
    case KeyR: key_code = GLFW_KEY_R; break;
    case KeyS: key_code = GLFW_KEY_S; break;
    case KeyT: key_code = GLFW_KEY_T; break;
    case KeyU: key_code = GLFW_KEY_U; break;
    case KeyV: key_code = GLFW_KEY_V; break;
    case KeyW: key_code = GLFW_KEY_W; break;
    case KeyX: key_code = GLFW_KEY_X; break;
    case KeyY: key_code = GLFW_KEY_Y; break;
    case KeyZ: key_code = GLFW_KEY_Z; break;
    case Key1: key_code = GLFW_KEY_1; break;
    case Key2: key_code = GLFW_KEY_2; break;
    case Key3: key_code = GLFW_KEY_3; break;
    case Key4: key_code = GLFW_KEY_4; break;
    case Key5: key_code = GLFW_KEY_5; break;
    case Key6: key_code = GLFW_KEY_6; break;
    case Key7: key_code = GLFW_KEY_7; break;
    case Key8: key_code = GLFW_KEY_8; break;
    case Key9: key_code = GLFW_KEY_9; break;
    case Key0: key_code = GLFW_KEY_0; break;
    case KeyF1: key_code = GLFW_KEY_F1; break;
    case KeyF2: key_code = GLFW_KEY_F2; break;
    case KeyF3: key_code = GLFW_KEY_F3; break;
    case KeyF4: key_code = GLFW_KEY_F4; break;
    case KeyF5: key_code = GLFW_KEY_F5; break;
    case KeyF6: key_code = GLFW_KEY_F6; break;
    case KeyF7: key_code = GLFW_KEY_F7; break;
    case KeyF8: key_code = GLFW_KEY_F8; break;
    case KeyF9: key_code = GLFW_KEY_F9; break;
    case KeyF10: key_code = GLFW_KEY_F10; break;
    case KeyF11: key_code = GLFW_KEY_F11; break;
    case KeyF12: key_code = GLFW_KEY_F12; break;
    case KeyEscape: key_code = GLFW_KEY_ESCAPE; break;
    case KeyTilde: key_code = GLFW_KEY_GRAVE_ACCENT; break;
    case KeyMinus: key_code = GLFW_KEY_MINUS; break;
    case KeyEquals: key_code = GLFW_KEY_EQUAL; break;
    case KeyBackspace: key_code = GLFW_KEY_BACKSPACE; break;
    case KeyTab: key_code = GLFW_KEY_TAB; break;
    case KeyLeftBracket: key_code = GLFW_KEY_LEFT_BRACKET; break;
    case KeyRightBracket: key_code = GLFW_KEY_RIGHT_BRACKET; break;
    case KeyBackslash: key_code = GLFW_KEY_BACKSLASH; break;
    case KeySemicolon: key_code = GLFW_KEY_SEMICOLON; break;
    case KeyApostrophe: key_code = GLFW_KEY_APOSTROPHE; break;
    case KeyEnter: key_code = GLFW_KEY_ENTER; break;
    case KeyComma: key_code = GLFW_KEY_COMMA; break;
    case KeyPeriod: key_code = GLFW_KEY_PERIOD; break;
    case KeySlash: key_code = GLFW_KEY_SLASH; break;
    case KeySpace: key_code = GLFW_KEY_SPACE; break;
    case KeyUp: key_code = GLFW_KEY_UP; break;
    case KeyDown: key_code = GLFW_KEY_DOWN; break;
    case KeyLeft: key_code = GLFW_KEY_LEFT; break;
    case KeyRight: key_code = GLFW_KEY_RIGHT; break;
  }
  return glfwGetKey(window, key_code) == GLFW_PRESS;
}

Vec2
InputMonitor::get_mouse_position() const
{
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  return Vec2(float(x), float(y));
}

bool
InputMonitor::is_left_mouse_down() const
{
  return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    ? true : false;
}

bool
InputMonitor::is_right_mouse_down() const
{
  return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    ? true : false;
}

bool
InputMonitor::is_middle_mouse_down() const
{
  return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    ? true : false;
}

bool
InputMonitor::is_gamepad_available() const
{
  return glfwJoystickPresent(GLFW_JOYSTICK_1)
    && glfwJoystickIsGamepad(GLFW_JOYSTICK_1);
}

Vec2
InputMonitor::gamepad_left_stick() const
{
  if (!is_gamepad_available())
    return Vec2();
  int axis_count;
  const float *joystick_axes =
    glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
  // TODO: check that axis_count <= 6
  return Vec2(joystick_threshold(joystick_axes[GLFW_GAMEPAD_AXIS_LEFT_X]),
    joystick_threshold(joystick_axes[GLFW_GAMEPAD_AXIS_LEFT_Y]));
}

Vec2
InputMonitor::gamepad_right_stick() const
{
  if (!is_gamepad_available())
    return Vec2();
  int axis_count;
  const float *joystick_axes =
    glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
  // TODO: check that axis_count <= 6
  return Vec2(joystick_threshold(joystick_axes[GLFW_GAMEPAD_AXIS_RIGHT_X]),
    joystick_threshold(joystick_axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]));
}

float
InputMonitor::gamepad_left_trigger() const
{
  // TODO: if !is_gamepad_available() then throw an exception
  int axis_count;
  const float *joystick_axes =
    glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
  // TODO: check that axis_count <= 6
  return joystick_axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
}

float
InputMonitor::gamepad_right_trigger() const
{
  // TODO: if !is_gamepad_available() then throw an exception
  int axis_count;
  const float *joystick_axes =
    glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
  // TODO: check that axis_count >= 6
  return joystick_axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
}

bool
InputMonitor::is_gamepad_button_down(GamepadButton button) const
{
  if (!is_gamepad_available())
    return false;
  int button_code;
  switch (button)
  {
    case ButtonA: button_code = GLFW_GAMEPAD_BUTTON_A; break;
    case ButtonB: button_code = GLFW_GAMEPAD_BUTTON_B; break;
    case ButtonX: button_code = GLFW_GAMEPAD_BUTTON_X; break;
    case ButtonY: button_code = GLFW_GAMEPAD_BUTTON_Y; break;
    case DpadUp: button_code = GLFW_GAMEPAD_BUTTON_DPAD_UP; break;
    case DpadDown: button_code = GLFW_GAMEPAD_BUTTON_DPAD_DOWN; break;
    case DpadLeft: button_code = GLFW_GAMEPAD_BUTTON_DPAD_LEFT; break;
    case DpadRight: button_code = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT; break;
    case BumperL: button_code = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER; break;
    case BumperR: button_code = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER; break;
    case StickLeft: button_code = GLFW_GAMEPAD_BUTTON_LEFT_THUMB; break;
    case StickRight: button_code = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB; break;
    case ButtonStart: button_code = GLFW_GAMEPAD_BUTTON_START; break;
    case ButtonBack: button_code = GLFW_GAMEPAD_BUTTON_BACK; break;
    case ButtonMenu: button_code = GLFW_GAMEPAD_BUTTON_GUIDE; break;
  }
  int button_count;
  const unsigned char *button_states =
    glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);
  // TODO: check that button_count is sufficiently large
  return (button_states[button_code] == GLFW_PRESS) ? true : false;
}

bool
InputMonitor::get_jump_input() const
{
  // TODO: allow these to be customized
  return is_key_down(KeySpace) || is_gamepad_button_down(ButtonA);
}
