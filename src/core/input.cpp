#include "core/input.h"
#include "core/state.h"

static MouseButton
glfw_mouse_button_to_mouse_button(int glfw_mouse_button)
{
  MouseButton mouse_button = MouseButtonLeft;
  switch (glfw_mouse_button)
  {
    case GLFW_MOUSE_BUTTON_LEFT: mouse_button = MouseButtonLeft; break;
    case GLFW_MOUSE_BUTTON_MIDDLE: mouse_button = MouseButtonMiddle; break;
    case GLFW_MOUSE_BUTTON_RIGHT: mouse_button = MouseButtonRight; break;
  }
  return mouse_button;
}

static int
mouse_button_to_glfw_mouse_button(MouseButton mouse_button)
{
  int glfw_mouse_button = 0;
  switch (mouse_button)
  {
    case MouseButtonLeft: glfw_mouse_button = GLFW_MOUSE_BUTTON_LEFT; break;
    case MouseButtonMiddle: glfw_mouse_button = GLFW_MOUSE_BUTTON_MIDDLE; break;
    case MouseButtonRight: glfw_mouse_button = GLFW_MOUSE_BUTTON_RIGHT; break;
  }
  return glfw_mouse_button;
}

static Key
glfw_key_to_key(int glfw_key)
{
  Key key = KeyA;
  switch (glfw_key)
  {
    case GLFW_KEY_A: key = KeyA; break;
    case GLFW_KEY_B: key = KeyB; break;
    case GLFW_KEY_C: key = KeyC; break;
    case GLFW_KEY_D: key = KeyD; break;
    case GLFW_KEY_E: key = KeyE; break;
    case GLFW_KEY_F: key = KeyF; break;
    case GLFW_KEY_G: key = KeyG; break;
    case GLFW_KEY_H: key = KeyH; break;
    case GLFW_KEY_I: key = KeyI; break;
    case GLFW_KEY_J: key = KeyJ; break;
    case GLFW_KEY_K: key = KeyK; break;
    case GLFW_KEY_L: key = KeyL; break;
    case GLFW_KEY_M: key = KeyM; break;
    case GLFW_KEY_N: key = KeyN; break;
    case GLFW_KEY_O: key = KeyO; break;
    case GLFW_KEY_P: key = KeyP; break;
    case GLFW_KEY_Q: key = KeyQ; break;
    case GLFW_KEY_R: key = KeyR; break;
    case GLFW_KEY_S: key = KeyS; break;
    case GLFW_KEY_T: key = KeyT; break;
    case GLFW_KEY_U: key = KeyU; break;
    case GLFW_KEY_V: key = KeyV; break;
    case GLFW_KEY_W: key = KeyW; break;
    case GLFW_KEY_X: key = KeyX; break;
    case GLFW_KEY_Y: key = KeyY; break;
    case GLFW_KEY_Z: key = KeyZ; break;
    case GLFW_KEY_1: key = Key1; break;
    case GLFW_KEY_2: key = Key2; break;
    case GLFW_KEY_3: key = Key3; break;
    case GLFW_KEY_4: key = Key4; break;
    case GLFW_KEY_5: key = Key5; break;
    case GLFW_KEY_6: key = Key6; break;
    case GLFW_KEY_7: key = Key7; break;
    case GLFW_KEY_8: key = Key8; break;
    case GLFW_KEY_9: key = Key9; break;
    case GLFW_KEY_0: key = Key0; break;
    case GLFW_KEY_F1: key = KeyF1; break;
    case GLFW_KEY_F2: key = KeyF2; break;
    case GLFW_KEY_F3: key = KeyF3; break;
    case GLFW_KEY_F4: key = KeyF4; break;
    case GLFW_KEY_F5: key = KeyF5; break;
    case GLFW_KEY_F6: key = KeyF6; break;
    case GLFW_KEY_F7: key = KeyF7; break;
    case GLFW_KEY_F8: key = KeyF8; break;
    case GLFW_KEY_F9: key = KeyF9; break;
    case GLFW_KEY_F10: key = KeyF10; break;
    case GLFW_KEY_F11: key = KeyF11; break;
    case GLFW_KEY_F12: key = KeyF12; break;
    case GLFW_KEY_ESCAPE: key = KeyEscape; break;
    case GLFW_KEY_GRAVE_ACCENT: key = KeyTilde; break;
    case GLFW_KEY_MINUS: key = KeyMinus; break;
    case GLFW_KEY_EQUAL: key = KeyEquals; break;
    case GLFW_KEY_BACKSPACE: key = KeyBackspace; break;
    case GLFW_KEY_TAB: key = KeyTab; break;
    case GLFW_KEY_LEFT_BRACKET: key = KeyLeftBracket; break;
    case GLFW_KEY_RIGHT_BRACKET: key = KeyRightBracket; break;
    case GLFW_KEY_BACKSLASH: key = KeyBackslash; break;
    case GLFW_KEY_SEMICOLON: key = KeySemicolon; break;
    case GLFW_KEY_APOSTROPHE: key = KeyApostrophe; break;
    case GLFW_KEY_ENTER: key = KeyEnter; break;
    case GLFW_KEY_COMMA: key = KeyComma; break;
    case GLFW_KEY_PERIOD: key = KeyPeriod; break;
    case GLFW_KEY_SLASH: key = KeySlash; break;
    case GLFW_KEY_SPACE: key = KeySpace; break;
    case GLFW_KEY_UP: key = KeyUp; break;
    case GLFW_KEY_DOWN: key = KeyDown; break;
    case GLFW_KEY_LEFT: key = KeyLeft; break;
    case GLFW_KEY_RIGHT: key = KeyRight; break;
  }
  return key;
}

static int
key_to_glfw_key(Key key)
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
  return key_code;
}

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  Key k = glfw_key_to_key(key);
  if (action == GLFW_PRESS)
    InputMonitor::get()->key_changed(k, true);
  else if (action == GLFW_RELEASE)
    InputMonitor::get()->key_changed(k, false);
}

static void
char_callback(GLFWwindow *window, unsigned int codepoint)
{
  InputMonitor::get()->char_in(codepoint);
}

static void
mouse_callback(GLFWwindow *window, double mouse_x, double mouse_y)
{

}

static void
mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  MouseButton m = glfw_mouse_button_to_mouse_button(button);
  if (action == GLFW_PRESS)
    InputMonitor::get()->mouse_button_press(m, true);
  else if (action == GLFW_RELEASE)
    InputMonitor::get()->mouse_button_press(m, false);
}

static void
scroll_callback(GLFWwindow *window, double scroll_x, double scroll_y)
{
  InputMonitor::get()->mouse_scroll(Vec2(scroll_x, scroll_y));
}

static void
window_close_callback(GLFWwindow *window)
{
  InputMonitor *input = (InputMonitor *)glfwGetWindowUserPointer(window);
  // TODO: check that this pointer is not nullptr
  input->close_window_pressed();
}

InputMonitor * InputMonitor::instance = nullptr;

InputMonitor::InputMonitor(GLFWwindow *_window) :
  window(_window)
{
  glfwSetKeyCallback(window, key_callback);
  glfwSetCharCallback(window, char_callback);
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
InputMonitor::set_instance(InputMonitor *_instance)
{
  instance = _instance;
}

InputMonitor *
InputMonitor::get()
{
  return instance;
}

void
InputMonitor::install_listener(Listener *listener)
{
  listeners.push_back(listener);
}

void
InputMonitor::remove_listener(Listener *listener)
{
  listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void
InputMonitor::close_window_pressed()
{
  EngineState::get()->close_game();
}

void
InputMonitor::mouse_button_press(MouseButton button, bool press)
{
  for (unsigned int i = 0; i < listeners.size(); ++i)
  {
    if (listeners[i]->mouse_button_handle)
      listeners[i]->mouse_button_handle.value()(button, press);
  }
}

void
InputMonitor::mouse_scroll(Vec2 scroll)
{
  for (unsigned int i = 0; i < listeners.size(); ++i)
  {
    if (listeners[i]->scroll_handle)
      listeners[i]->scroll_handle.value()(scroll);
  }
}

void
InputMonitor::key_changed(Key key, bool press)
{
  for (unsigned int i = 0; i < listeners.size(); ++i)
  {
    if (listeners[i]->key_handle)
      listeners[i]->key_handle.value()(key, press);
  }
}

void
InputMonitor::char_in(unsigned int codepoint)
{
  for (unsigned int i = 0; i < listeners.size(); ++i)
  {
    if (listeners[i]->char_handle)
      listeners[i]->char_handle.value()(codepoint);
  }
}

bool
InputMonitor::is_key_down(Key key) const
{
  return glfwGetKey(window, key_to_glfw_key(key)) == GLFW_PRESS;
}

Vec2
InputMonitor::get_mouse_position() const
{
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  return Vec2(float(x), float(height) - float(y));
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

Button::Button(Vec2 _origin, Vec2 _size,
  const std::function<void(void *)> _callback, void *_userdata) :
  origin(_origin), size(_size),
  callback(_callback), userdata(_userdata), enabled(true), state(false)
{
  listener.mouse_button_handle = std::bind(&Button::mouse_button, this,
    std::placeholders::_1, std::placeholders::_2);
  InputMonitor::get()->install_listener(&listener);
}

Button::~Button()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
Button::set_origin(Vec2 _origin)
{
  origin = _origin;
}

void
Button::set_size(Vec2 _size)
{
  size = _size;
}

void
Button::set_enabled(bool _enabled)
{
  enabled = _enabled;
}

void
Button::mouse_button(MouseButton button, bool pressed)
{
  if (!enabled)
    return;
  bool cursor_in = InputMonitor::get()->get_mouse_position().inside_rect(origin, size);
  if (button == MouseButtonLeft && cursor_in && pressed)
    state = true;
  if (button == MouseButtonLeft && state & !pressed)
  {
    state = false;
    callback(userdata);
  }
}
