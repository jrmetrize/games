#include "screen.h"
#include "graphics.h"

MenuButton::MenuButton(std::string _text, float _height, std::function<void()> _target) :
  text(_text), height(_height), target(_target),
  highlighted(false), pressed(false)
{

}

void
MenuButton::update()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  if (!InputMonitor::get()->is_left_mouse_down())
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      Vec2(0, height), Vec2(window_size.x, 50));
}

void
MenuButton::mouse_pressed(MouseButton button, bool button_pressed)
{
  if (highlighted && button_pressed)
    pressed = true;

  {
    Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
    bool contains_mouse = InputMonitor::get()->get_mouse_position().inside_rect(
      Vec2(0, height), Vec2(window_size.x, 50));
    if (pressed && !button_pressed && contains_mouse)
      target();
  }

  if (!button_pressed)
    pressed = false;
}

Screen::Screen()
{

}

void
Screen::to_appear()
{

}

void
Screen::to_disappear()
{

}
