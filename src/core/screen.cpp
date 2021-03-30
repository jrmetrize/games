#include "core/screen.h"
#include "core/graphics.h"
#include "core/state.h"
#include "core/audio.h"
#include <algorithm>

MenuControl::MenuControl(Vec2 _origin, Vec2 _size) :
  origin(_origin), size(_size)
{

}

void
MenuControl::play_highlight_sound()
{
  AudioTrack *cursor = (AudioTrack *)GameState::get()->get_globals()->get_resource("menu_cursor");
  AudioServer::get()->play(cursor);
}

void
MenuControl::play_confirm_sound()
{
  AudioTrack *confirm = (AudioTrack *)GameState::get()->get_globals()->get_resource("menu_confirm");
  AudioServer::get()->play(confirm);
}

MenuButton::MenuButton(std::string _text, Vec2 _origin, Vec2 _size,
  std::function<void()> _target) :
  MenuControl(_origin, _size), text(_text), target(_target),
  highlighted(false), pressed(false)
{

}

void
MenuButton::update()
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    bool prev_highlighted = highlighted;
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      origin, size);
    if (prev_highlighted == false && highlighted)
    {
      play_highlight_sound();
    }
  }
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
    {
      target();
      play_confirm_sound();
    }
  }

  if (!button_pressed)
    pressed = false;
}

MenuSwitch::MenuSwitch(Vec2 _origin, Vec2 _size, std::function<void(bool)> _value_changed) :
  MenuControl(_origin, _size), value(false), value_changed(_value_changed),
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

MenuSlider::MenuSlider(Vec2 _origin, Vec2 _size, std::function<void(float)> _value_changed) :
  MenuControl(_origin, _size), value(0.0), value_changed(_value_changed),
  highlighted(false), pressed(false)
{

}

void
MenuSlider::update()
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    float grab_bar_width = 6.0f;
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(origin
      + Vec2((value * size.x) - (0.5f * grab_bar_width), 0), Vec2(grab_bar_width, size.y));
  }

  // If the grab bar is selected, grab the x coordinate of the cursor and clamp
  // it to be within the range of the slider.
  if (pressed)
  {
    float cursor_x = InputMonitor::get()->get_mouse_position().x;
    float normalized = (cursor_x - origin.x) / size.x;
    float clamped = std::clamp(normalized, 0.0f, 1.0f);
    value = clamped;
  }
}

void
MenuSlider::mouse_pressed(MouseButton button, bool button_pressed)
{
  if (highlighted && button_pressed)
    pressed = true;

  if (!button_pressed)
    pressed = false;
}

void
MenuSlider::draw()
{
  Vec4 normal_bg = Vec4(0.8f, 0.8f, 0.8f, 0.4f);
  Vec4 grab_bg = Vec4(0.6f, 0.6f, 0.6f, 1.0f);
  Vec4 pressed_bg = Vec4(1.0f, 0.8f, 1.0f, 1.0f);
  Vec4 highlighted_bg = Vec4(1.0f, 0.8f, 0.8f, 1.0f);
  float slider_bar_width = 4.0f;
  float grab_bar_width = 10.0f;

  GraphicsServer::get()->draw_color_rect(origin + Vec2(0, 0.5f * (size.y - slider_bar_width)),
    Vec2(size.x, slider_bar_width), normal_bg);
  if (pressed)
  {
    GraphicsServer::get()->draw_color_rect(origin + Vec2((value * size.x) - (0.5f * grab_bar_width), 0),
      Vec2(grab_bar_width, size.y), pressed_bg);
  }
  else if (highlighted)
  {
    GraphicsServer::get()->draw_color_rect(origin + Vec2((value * size.x) - (0.5f * grab_bar_width), 0),
      Vec2(grab_bar_width, size.y), highlighted_bg);
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(origin + Vec2((value * size.x) - (0.5f * grab_bar_width), 0),
      Vec2(grab_bar_width, size.y), grab_bg);
  }
}

MenuSelector::MenuSelector(Vec2 _origin, Vec2 _size, PropertyData *_property) :
  MenuControl(_origin, _size), property(_property),
  highlighted(-1), pressed(-1)
{

}

void
MenuSelector::update()
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    float box_separation = 8.0f;
    float dir_button_width = 20.0f;
    Vec2 left_box_origin = origin;
    Vec2 left_box_size = Vec2(dir_button_width, size.y);
    Vec2 right_box_origin = origin + Vec2(size.x - dir_button_width, 0);
    Vec2 right_box_size = Vec2(dir_button_width, size.y);

    bool left_highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      left_box_origin, left_box_size);
    bool right_highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      right_box_origin, right_box_size);

    if (left_highlighted)
      highlighted = 0;
    else if (right_highlighted)
      highlighted = 1;
    else
      highlighted = -1;
  }
}

void
MenuSelector::mouse_pressed(MouseButton button, bool button_pressed)
{
  if (highlighted != -1 && button_pressed)
    pressed = highlighted;

  {
    bool contains_mouse = false;
    float box_separation = 8.0f;
    float dir_button_width = 20.0f;
    Vec2 left_box_origin = origin;
    Vec2 left_box_size = Vec2(dir_button_width, size.y);
    Vec2 right_box_origin = origin + Vec2(size.x - dir_button_width, 0);
    Vec2 right_box_size = Vec2(dir_button_width, size.y);
    if (InputMonitor::get()->get_mouse_position().inside_rect(
      left_box_origin, left_box_size))
    {
      if (pressed == 0 && !button_pressed)
      {
        // Subtract 1 from the index
        EnumData en = std::get<EnumData>(property->data);
        en.current = (en.current - 1) % en.choices.size();
        property->set_data(en);
      }
    } else if (InputMonitor::get()->get_mouse_position().inside_rect(
      right_box_origin, right_box_size))
    {
      if (pressed == 1 && !button_pressed)
      {
        // Add 1 to the index
        EnumData en = std::get<EnumData>(property->data);
        en.current = (en.current + 1) % en.choices.size();
        property->set_data(en);
      }
    }
  }

  if (!button_pressed)
    pressed = -1;
}

void
MenuSelector::draw()
{
  float box_separation = 8.0f;
  float dir_button_width = 20.0f;
  Vec4 normal_bg = Vec4(0.8f, 0.8f, 0.8f, 0.4f);
  Vec4 selected_bg = Vec4(0.5f, 0.5f, 0.5f, 0.4f);
  Vec4 highlighted_bg = Vec4(1.0f, 0.8f, 0.8f, 0.4f);
  Vec4 pressed_bg = Vec4(1.0f, 0.8f, 1.0f, 0.4f);

  Vec2 left_box_origin = origin;
  Vec2 left_box_size = Vec2(dir_button_width, size.y);

  Vec2 right_box_origin = origin + Vec2(size.x - dir_button_width, 0);
  Vec2 right_box_size = Vec2(dir_button_width, size.y);

  if (pressed == 0)
  {
    GraphicsServer::get()->draw_color_rect(left_box_origin, left_box_size,
      pressed_bg);
  }
  else if (highlighted == 0)
  {
    GraphicsServer::get()->draw_color_rect(left_box_origin, left_box_size,
      highlighted_bg);
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(left_box_origin, left_box_size,
      normal_bg);
  }

  if (pressed == 1)
  {
    GraphicsServer::get()->draw_color_rect(right_box_origin, right_box_size,
      pressed_bg);
  }
  else if (highlighted == 1)
  {
    GraphicsServer::get()->draw_color_rect(right_box_origin, right_box_size,
      highlighted_bg);
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(right_box_origin, right_box_size,
      normal_bg);
  }

  TextRenderRequest req = {};
  req.font = GameState::get()->get_serif();
  req.center = true;
  req.center_vertical = true;
  req.size = 24;
  req.color = Vec4(1);
  req.bounding_box_origin = origin + Vec2(dir_button_width + box_separation, 0);
  req.bounding_box_size = Vec2(size.x - 2.0f * (dir_button_width + box_separation), size.y);
  {
    const EnumData &en = std::get<EnumData>(property->data);
    req.text = en.choices[en.current].text;
  }
  GraphicsServer::get()->draw_text_line(req);
}

Screen::Screen()
{

}

void
Screen::resize(Vec2 size)
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
