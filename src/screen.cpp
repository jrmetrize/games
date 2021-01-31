#include "screen.h"
#include "graphics.h"
#include "state.h"

MenuButton::MenuButton(std::string _text, Vec2 _origin, Vec2 _size,
  std::function<void()> _target) :
  text(_text), origin(_origin), size(_size), target(_target),
  highlighted(false), pressed(false)
{

}

void
MenuButton::update()
{
  if (!InputMonitor::get()->is_left_mouse_down())
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      origin, size);
}

void
MenuButton::draw()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  TextRenderRequest req = {};
  req.bounding_box_origin = origin;
  req.bounding_box_size = size;
  req.text = text;
  req.font = GameState::get()->get_serif();
  //req.center = true;
  req.center_vertical = true;

  if (pressed)
  {
    req.size = 42;
    req.color = Vec4(0.5, 0.5, 0.5, 1);
  }
  else if (highlighted)
  {
    req.size = 42;
    float sine = sin(3.0f * GameState::get()->get_time());
    float brightness = 0.7f + (0.3f * sine);
    req.color = Vec4(brightness, brightness, brightness, 1);
  }
  else
  {
    req.size = 36;
    req.color = Vec4(1);
  }

  GraphicsServer::get()->draw_text_line(req);
}

void
MenuButton::mouse_pressed(MouseButton button, bool button_pressed)
{
  if (highlighted && button_pressed)
    pressed = true;

  {
    bool contains_mouse = InputMonitor::get()->get_mouse_position().inside_rect(
      origin, size);
    if (pressed && !button_pressed && contains_mouse)
      target();
  }

  if (!button_pressed)
    pressed = false;
}

MenuSwitch::MenuSwitch(Vec2 _origin, Vec2 _size, std::function<void(bool)> _value_changed) :
  origin(_origin), size(_size), value(false), value_changed(_value_changed),
  highlighted(-1), pressed(-1)
{

}

void
MenuSwitch::update()
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    float box_separation = 8.0f;
    bool left_highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      origin, Vec2(0.5f * (size.x - box_separation), size.y));
    bool right_highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      origin + Vec2((0.5f * (size.x - box_separation)) + box_separation, 0),
        Vec2(0.5f * (size.x - box_separation), size.y));

    if (left_highlighted)
      highlighted = 0;
    else if (right_highlighted)
      highlighted = 1;
    else
      highlighted = -1;
  }
}

void
MenuSwitch::mouse_pressed(MouseButton button, bool button_pressed)
{
  if (highlighted != -1 && button_pressed)
    pressed = highlighted;

  {
    bool contains_mouse = false;
    float box_separation = 8.0f;
    if (InputMonitor::get()->get_mouse_position().inside_rect(
      origin, Vec2(0.5f * (size.x - box_separation), size.y)))
    {
      // See if no was pressed
      if (pressed == 0 && !button_pressed)
      {
        value = false;
        value_changed(value);
      }
    } else if (InputMonitor::get()->get_mouse_position().inside_rect(
      origin + Vec2((0.5f * (size.x - box_separation)) + box_separation, 0),
      Vec2(0.5f * (size.x - box_separation), size.y)))
    {
      // See if yes was pressed
      if (pressed == 1 && !button_pressed)
      {
        value = true;
        value_changed(value);
      }
    }
  }

  if (!button_pressed)
    pressed = -1;
}

void
MenuSwitch::draw()
{
  float box_separation = 8.0f;
  Vec4 normal_bg = Vec4(0.8f, 0.8f, 0.8f, 0.4f);
  Vec4 selected_bg = Vec4(0.5f, 0.5f, 0.5f, 0.4f);
  Vec4 highlighted_bg = Vec4(1.0f, 0.8f, 0.8f, 0.4f);
  Vec4 pressed_bg = Vec4(1.0f, 0.8f, 1.0f, 0.4f);

  Vec2 left_box_origin = origin;
  Vec2 left_box_size = Vec2(0.5f * (size.x - box_separation), size.y);

  Vec2 right_box_origin = origin + Vec2((0.5f * (size.x - box_separation)) + box_separation, 0);
  Vec2 right_box_size = Vec2(0.5f * (size.x - box_separation), size.y);

  if (pressed == 0 && value != false)
  {
    GraphicsServer::get()->draw_color_rect(left_box_origin, left_box_size,
      pressed_bg);
  }
  else if (highlighted == 0 && value != false)
  {
    GraphicsServer::get()->draw_color_rect(left_box_origin, left_box_size,
      highlighted_bg);
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(left_box_origin, left_box_size,
      (value == false) ? selected_bg : normal_bg);
  }

  if (pressed == 1 && value != true)
  {
    GraphicsServer::get()->draw_color_rect(right_box_origin, right_box_size,
      pressed_bg);
  }
  else if (highlighted == 1 && value != true)
  {
    GraphicsServer::get()->draw_color_rect(right_box_origin, right_box_size,
      highlighted_bg);
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(right_box_origin, right_box_size,
      value ? selected_bg : normal_bg);
  }

  TextRenderRequest req = {};
  req.font = GameState::get()->get_serif();
  req.center = true;
  req.center_vertical = true;
  req.size = 24;
  req.color = Vec4(1);

  {
    req.bounding_box_origin = left_box_origin;
    req.bounding_box_size = left_box_size;
    req.text = "No";
    GraphicsServer::get()->draw_text_line(req);
  }

  {
    req.bounding_box_origin = right_box_origin;
    req.bounding_box_size = right_box_size;
    req.text = "Yes";
    GraphicsServer::get()->draw_text_line(req);
  }
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
