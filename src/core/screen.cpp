#include "core/screen.h"
#include "core/graphics.h"
#include "core/state.h"
#include "core/audio.h"
#include <algorithm>

MenuControl::MenuControl():
  origin(), size(), hovered(false), focused(false), parent(nullptr)
{

}

MenuControl::MenuControl(Vec2 _origin, Vec2 _size) :
  origin(_origin), size(_size), hovered(false), focused(false),
  tab_next(nullptr), tab_prev(nullptr), parent(nullptr)
{

}

void
MenuControl::origin_changed()
{

}

void
MenuControl::size_changed()
{

}

void
MenuControl::propagate_event(const MenuControlEvent *event)
{
  handle_event(event);
  for (uint32_t i = 0; i < children.size(); ++i)
    children[i]->propagate_event(event);
}

Vec2
MenuControl::get_origin() const
{
  return origin;
}

void
MenuControl::set_origin(Vec2 _origin)
{
  origin = _origin;
  origin_changed();
}

Vec2
MenuControl::get_size() const
{
  return size;
}

void
MenuControl::set_size(Vec2 _size)
{
  size = _size;
  size_changed();
}

void
MenuControl::set_hovered(bool _hovered)
{
  hovered = _hovered;
}

bool
MenuControl::is_hovered() const
{
  return hovered;
}

void
MenuControl::set_focused(bool _focused)
{
  focused = _focused;
}

bool
MenuControl::is_focused() const
{
  return focused;
}

Vec2
MenuControl::get_global_offset() const
{
  Vec2 offset = Vec2();
  const MenuControl *node = this;
  while (node->parent != nullptr)
  {
    offset += node->origin;
    node = node->parent;
  }
  return offset;
}

MenuControl *
MenuControl::get_tab_prev()
{
  return tab_prev;
}

MenuControl *
MenuControl::get_tab_next()
{
  return tab_next;
}

void
MenuControl::set_tab_prev(MenuControl *_prev)
{
  tab_prev = _prev;
}

void
MenuControl::set_tab_next(MenuControl *_next)
{
  tab_next = _next;
}

void
MenuControl::make_tab_list(std::vector<MenuControl *> controls)
{
  if (controls.size() > 1)
  {
    controls[0]->set_tab_next(controls[1]);
    controls[controls.size() - 1]->set_tab_prev(controls[controls.size() - 2]);
  }
  for (size_t i = 1; i < controls.size() - 1; ++i)
  {
    controls[i]->set_tab_prev(controls[i - 1]);
    controls[i]->set_tab_next(controls[i + 1]);
  }

}

MenuControl *
MenuControl::get_control_under(Vec2 position)
{
  /* First check the child nodes. If none of them hit, check this node. */
  /* TODO: allow subclasses to define custom behavior for cursor detection,
     not just cursor-in-rect. */
  MenuControl *hovered = nullptr;

  for (MenuControl *child : children)
  {
    hovered = child->get_control_under(position);
    if (hovered != nullptr)
      return hovered;
  }

  Vec2 offset = get_global_offset();
  if (position.inside_rect(offset, size))
    hovered = this;

  return hovered;
}

void
MenuControl::update_children(float time_elapsed)
{
  this->update(time_elapsed);
  for (MenuControl *child : children)
    child->update_children(time_elapsed);
}

void
MenuControl::draw_children()
{
  this->draw();
  for (MenuControl *child : children)
    child->draw_children();
}

void
MenuControl::add_child(MenuControl *child)
{
  child->parent = this;
  children.push_back(child);
}

void
MenuControl::remove_child(MenuControl *child)
{
  child->parent = nullptr;
  children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void
MenuControl::handle_event(const MenuControlEvent *event)
{

}

Screen::Screen() :
  hovered_control(nullptr), active_control(nullptr)
{
  listener = new Listener();

  listener->mouse_button_handle = std::bind(&Screen::_mouse_button_update, this,
    std::placeholders::_1, std::placeholders::_2);
  listener->cursor_pos_handle = std::bind(&Screen::_cursor_update, this,
    std::placeholders::_1);
  listener->scroll_handle = std::bind(&Screen::_scroll_update, this,
    std::placeholders::_1);
  listener->key_handle = std::bind(&Screen::_key_update, this,
    std::placeholders::_1, std::placeholders::_2);
  listener->gamepad_button_handle = std::bind(&Screen::_gamepad_button_update,
    this, std::placeholders::_1, std::placeholders::_2);
  listener->char_handle = std::bind(&Screen::_char_update, this,
    std::placeholders::_1);
}

Screen::~Screen()
{
  delete listener;
}

void
Screen::_mouse_button_update(MouseButton button, bool pressed)
{
  MenuControlEvent event = MenuControlEvent();
  event.type = MenuControlEventTypeMouseButton;
  event.pressed = pressed;
  event.mouse_button = button;

  propagate_event(&event);

  if (pressed == false)
  {
    if (active_control != nullptr)
      active_control->set_focused(false);
    active_control = hovered_control;
    if (active_control != nullptr)
      active_control->set_focused(true);
  }
}

void
Screen::_cursor_update(Vec2 position)
{
  /* Determine what control, if any, the cursor is hovering over. */
  MenuControl *last_hovered = hovered_control;
  hovered_control = get_control_under(position);
  if (last_hovered != nullptr)
    last_hovered->set_hovered(false);
  if (hovered_control != nullptr)
    hovered_control->set_hovered(true);

  MenuControlEvent event = MenuControlEvent();
  event.type = MenuControlEventTypeCursor;
  event.cursor_position = position;

  propagate_event(&event);
}

void
Screen::_scroll_update(Vec2 scroll)
{
  MenuControlEvent event = MenuControlEvent();
  event.type = MenuControlEventTypeScroll;
  event.scroll_distance = scroll;

  propagate_event(&event);
}

void
Screen::_key_update(Key key, bool pressed)
{
  /* TODO: this should probably be handled by the control itself */
  if (key == KeyTab && pressed == true && active_control != nullptr)
  {
    // Shift + Tab to go backwards, Tab to go forwards
    /* TODO: these bindings should not be coded in place. */
    if (InputMonitor::get()->is_key_down(KeyLeftShift) ||
        InputMonitor::get()->is_key_down(KeyRightShift))
    {
      if (active_control->get_tab_prev() != nullptr)
      {
        active_control->set_focused(false);
        active_control = active_control->get_tab_prev();
        active_control->set_focused(true);
      }
    }
    else
    {
      if (active_control->get_tab_next() != nullptr)
      {
        active_control->set_focused(false);
        active_control = active_control->get_tab_next();
        active_control->set_focused(true);
      }
    }
  }

  MenuControlEvent event = MenuControlEvent();
  event.type = MenuControlEventTypeKey;
  event.pressed = pressed;
  event.key = key;

  propagate_event(&event);
}

void
Screen::_gamepad_button_update(GamepadButton button, bool pressed)
{
  MenuControlEvent event = MenuControlEvent();
  event.type = MenuControlEventTypeGamepadButton;
  event.pressed = pressed;
  event.gamepad_button = button;

  propagate_event(&event);
}

void
Screen::_char_update(unsigned int codepoint)
{
  MenuControlEvent event = MenuControlEvent();
  event.type = MenuControlEventTypeChar;
  event.codepoint = codepoint;

  propagate_event(&event);
}

MenuControl *
Screen::get_active_control()
{
  return active_control;
}

void
Screen::set_active_control(MenuControl *_active_control)
{
  active_control = _active_control;
  /* TODO: notify this control and the old one, if it existed */
}

void
Screen::resize(Vec2 size)
{

}

void
Screen::install_listener()
{
  InputMonitor::get()->install_listener(listener);
}

void
Screen::remove_listener()
{
  InputMonitor::get()->remove_listener(listener);
}

void
Screen::to_appear()
{

}

void
Screen::to_disappear()
{

}

EmptyControl::EmptyControl(Vec2 _origin) :
  MenuControl(_origin, Vec2())
{

}

void
EmptyControl::update(float time_elapsed)
{

}

void
EmptyControl::draw()
{

}

MenuButton::MenuButton(std::string _text, Vec2 _origin, Vec2 _size,
  std::function<void()> _target) :
  MenuControl(_origin, _size), text(_text), target(_target),
  highlighted(false), pressed(false)
{

}

void
MenuButton::update(float time_elapsed)
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    bool prev_highlighted = highlighted;
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      get_origin(), get_size());
    if (prev_highlighted == false && highlighted)
    {
      //play_highlight_sound();
    }
  }
}

void
MenuButton::draw()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  TextRenderRequest req = {};
  req.bounding_box_origin = get_origin();
  req.bounding_box_size = get_size();
  req.text = text;
  req.font = EngineState::get()->get_serif();
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
    float sine = sin(3.0f * EngineState::get()->get_time());
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
MenuButton::handle_event(const MenuControlEvent *event)
{
  if (event->type == MenuControlEventTypeMouseButton)
  {
    if (highlighted && event->pressed)
      pressed = true;

    {
      bool contains_mouse = InputMonitor::get()->get_mouse_position().inside_rect(
        get_global_offset(), get_size());
      if (pressed && !event->pressed && contains_mouse)
      {
        target();
        //play_confirm_sound();
      }
    }

    if (!event->pressed)
      pressed = false;
  }
}

MenuSwitch::MenuSwitch(Vec2 _origin, Vec2 _size, std::function<void(bool)> _value_changed) :
  MenuControl(_origin, _size), value(false), value_changed(_value_changed),
  highlighted(-1), pressed(-1)
{

}

void
MenuSwitch::update(float time_elapsed)
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    float box_separation = 8.0f;
    bool left_highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      get_global_offset(), Vec2(0.5f * (get_size().x - box_separation), get_size().y));
    bool right_highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      get_global_offset() + Vec2((0.5f * (get_size().x - box_separation)) + box_separation, 0),
        Vec2(0.5f * (get_size().x - box_separation), get_size().y));

    if (left_highlighted)
      highlighted = 0;
    else if (right_highlighted)
      highlighted = 1;
    else
      highlighted = -1;
  }
}

void
MenuSwitch::handle_event(const MenuControlEvent *event)
{
  if (event->type == MenuControlEventTypeMouseButton)
  {
    bool button_pressed = event->pressed;
    if (highlighted != -1 && button_pressed)
      pressed = highlighted;

    {
      bool contains_mouse = false;
      float box_separation = 8.0f;
      if (InputMonitor::get()->get_mouse_position().inside_rect(
          get_global_offset(), Vec2(0.5f * (get_size().x - box_separation),
          get_size().y)))
      {
        // See if no was pressed
        if (pressed == 0 && !button_pressed)
        {
          value = false;
          value_changed(value);
        }
      } else if (InputMonitor::get()->get_mouse_position().inside_rect(
        get_global_offset() + Vec2((0.5f * (get_size().x - box_separation)) + box_separation, 0),
        Vec2(0.5f * (get_size().x - box_separation), get_size().y)))
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
}

void
MenuSwitch::draw()
{
  float box_separation = 8.0f;
  Vec4 normal_bg = Vec4(0.8f, 0.8f, 0.8f, 0.4f);
  Vec4 selected_bg = Vec4(0.5f, 0.5f, 0.5f, 0.4f);
  Vec4 highlighted_bg = Vec4(1.0f, 0.8f, 0.8f, 0.4f);
  Vec4 pressed_bg = Vec4(1.0f, 0.8f, 1.0f, 0.4f);

  Vec2 left_box_origin = get_global_offset();
  Vec2 left_box_size = Vec2(0.5f * (get_size().x - box_separation), get_size().y);

  Vec2 right_box_origin = get_global_offset()
    + Vec2((0.5f * (get_size().x - box_separation)) + box_separation, 0);
  Vec2 right_box_size = Vec2(0.5f * (get_size().x - box_separation), get_size().y);

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
  req.font = EngineState::get()->get_serif();
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
MenuSlider::update(float time_elapsed)
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    float grab_bar_width = 6.0f;
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(get_global_offset()
      + Vec2((value * get_size().x) - (0.5f * grab_bar_width), 0), Vec2(grab_bar_width, get_size().y));
  }

  // If the grab bar is selected, grab the x coordinate of the cursor and clamp
  // it to be within the range of the slider.
  if (pressed)
  {
    float cursor_x = InputMonitor::get()->get_mouse_position().x;
    float normalized = (cursor_x - get_global_offset().x) / get_size().x;
    float clamped = std::clamp(normalized, 0.0f, 1.0f);
    value = clamped;
  }
}

void
MenuSlider::handle_event(const MenuControlEvent *event)
{
  if (event->type == MenuControlEventTypeMouseButton)
  {
    bool button_pressed = event->pressed;

    if (highlighted && button_pressed)
      pressed = true;

    if (!button_pressed)
      pressed = false;
  }
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

  GraphicsServer::get()->draw_color_rect(get_global_offset()
    + Vec2(0, 0.5f * (get_size().y - slider_bar_width)),
    Vec2(get_size().x, slider_bar_width), normal_bg);
  if (pressed)
  {
    GraphicsServer::get()->draw_color_rect(get_global_offset()
      + Vec2((value * get_size().x) - (0.5f * grab_bar_width), 0),
      Vec2(grab_bar_width, get_size().y), pressed_bg);
  }
  else if (highlighted)
  {
    GraphicsServer::get()->draw_color_rect(get_global_offset()
      + Vec2((value * get_size().x) - (0.5f * grab_bar_width), 0),
      Vec2(grab_bar_width, get_size().y), highlighted_bg);
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(get_global_offset()
      + Vec2((value * get_size().x) - (0.5f * grab_bar_width), 0),
      Vec2(grab_bar_width, get_size().y), grab_bg);
  }
}

MenuSelector::MenuSelector(Vec2 _origin, Vec2 _size, PropertyData *_property) :
  MenuControl(_origin, _size), property(_property),
  highlighted(-1), pressed(-1)
{

}

void
MenuSelector::update(float time_elapsed)
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    float box_separation = 8.0f;
    float dir_button_width = 20.0f;
    Vec2 left_box_origin = get_global_offset();
    Vec2 left_box_size = Vec2(dir_button_width, get_size().y);
    Vec2 right_box_origin = get_global_offset()
      + Vec2(get_size().x - dir_button_width, 0);
    Vec2 right_box_size = Vec2(dir_button_width, get_size().y);

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
MenuSelector::handle_event(const MenuControlEvent *event)
{
  if (event->type == MenuControlEventTypeMouseButton)
  {
    bool button_pressed = event->pressed;
    if (highlighted != -1 && button_pressed)
      pressed = highlighted;

    {
      bool contains_mouse = false;
      float box_separation = 8.0f;
      float dir_button_width = 20.0f;
      Vec2 left_box_origin = get_global_offset();
      Vec2 left_box_size = Vec2(dir_button_width, get_size().y);
      Vec2 right_box_origin = get_global_offset()
        + Vec2(get_size().x - dir_button_width, 0);
      Vec2 right_box_size = Vec2(dir_button_width, get_size().y);
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
}

void
MenuSelector::draw()
{
  Vec2 offset = get_global_offset();

  float box_separation = 8.0f;
  float dir_button_width = 20.0f;
  Vec4 normal_bg = Vec4(0.8f, 0.8f, 0.8f, 0.4f);
  Vec4 selected_bg = Vec4(0.5f, 0.5f, 0.5f, 0.4f);
  Vec4 highlighted_bg = Vec4(1.0f, 0.8f, 0.8f, 0.4f);
  Vec4 pressed_bg = Vec4(1.0f, 0.8f, 1.0f, 0.4f);

  Vec2 left_box_origin = offset;
  Vec2 left_box_size = Vec2(dir_button_width, get_size().y);

  Vec2 right_box_origin = offset + Vec2(get_size().x - dir_button_width, 0);
  Vec2 right_box_size = Vec2(dir_button_width, get_size().y);

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
  req.font = EngineState::get()->get_serif();
  req.center = true;
  req.center_vertical = true;
  req.size = 24;
  req.color = Vec4(1);
  req.bounding_box_origin = offset + Vec2(dir_button_width + box_separation, 0);
  req.bounding_box_size =
    Vec2(get_size().x - 2.0f * (dir_button_width + box_separation), get_size().y);
  {
    const EnumData &en = std::get<EnumData>(property->data);
    req.text = en.choices[en.current].text;
  }
  GraphicsServer::get()->draw_text_line(req);
}

TextLine::TextLine(Vec2 _origin, Vec2 _size, std::string _text) :
  MenuControl(_origin, _size), text(_text), highlighted(false), pressed(false),
  cursor_pos(0)
{

}

void
TextLine::update(float time_elapsed)
{
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    Vec2 offset = get_global_offset();
    bool h = InputMonitor::get()->get_mouse_position().inside_rect(
      offset, get_size());

    if (h)
      highlighted = true;
    else
      highlighted = false;
  }
}

void
TextLine::handle_event(const MenuControlEvent *event)
{
  if (event->type == MenuControlEventTypeKey)
  {
    Key key = event->key;
    bool pressed = event->pressed;

    if (key == Key::KeyLeft && pressed == true)
    {
      if (cursor_pos > 0)
        cursor_pos -= 1;
    }
    else if (key == Key::KeyRight && pressed == true)
    {
      if (cursor_pos < text.length())
        cursor_pos += 1;
    }
    else if (key == Key::KeyBackspace && pressed == true)
    {
      if (cursor_pos > 0)
      {
        text.erase(cursor_pos - 1, 1);
        cursor_pos -= 1;
      }
    }
  }
  else if (event->type == MenuControlEventTypeChar)
  {
    unsigned int codepoint = event->codepoint;
    text.insert(cursor_pos, 1, (char)codepoint);
    cursor_pos += 1;
  }
  else if (event->type == MenuControlEventTypeMouseButton)
  {
    Vec2 offset = get_global_offset();

    MouseButton button = event->mouse_button;
    bool button_pressed = event->pressed;

    if (highlighted != -1 && button_pressed)
      pressed = highlighted;

    if (InputMonitor::get()->get_mouse_position().inside_rect(
      offset, get_size()))
    {
      if (pressed && !button_pressed)
      {
        /* toggle active */
      }
    }

    if (!button_pressed)
      pressed = -1;
  }
}

void
TextLine::draw()
{
  Vec2 offset = get_global_offset();

  Vec4 normal_bg = Vec4(0.8f, 0.8f, 0.8f, 0.4f);
  Vec4 highlighted_bg = Vec4(1.0f, 0.8f, 0.8f, 0.4f);
  Vec4 selected_bg = Vec4(0.5f, 0.5f, 0.5f, 0.4f);
  if (highlighted)
    GraphicsServer::get()->draw_color_rect(offset, get_size(),
      highlighted_bg);
  else
    GraphicsServer::get()->draw_color_rect(offset, get_size(),
      normal_bg);

  {
    TextRenderRequest req = {};
    req.font = EngineState::get()->get_serif();
    req.center = false;
    req.center_vertical = true;
    req.size = 16;
    req.color = Vec4(0, 0, 0, 1);
    req.bounding_box_origin = offset + Vec2(8, 0);
    req.bounding_box_size = get_size() - Vec2(16, 0);
    req.text = text;
    req.mask_bounds = true;

    req.cursor = is_focused();
    req.cursor_pos = cursor_pos;
    {
      float sine = sin(5.0f * EngineState::get()->get_time());
      float x = sine * sine;
      req.cursor_color = Vec4(1, 1, 1, x);
    }
    GraphicsServer::get()->draw_text_line(req);
  }
}

std::string
TextLine::get_text() const
{
  return text;
}

ColorSelector::ColorSelector() :
  MenuControl(Vec2(32, 32), Vec2(320, 160))
{
  r_component = new TextLine(Vec2(16, 108), Vec2(288, 24), "1.0");
  g_component = new TextLine(Vec2(16, 78), Vec2(288, 24), "1.0");
  b_component = new TextLine(Vec2(16, 48), Vec2(288, 24), "1.0");

  add_child(r_component);
  add_child(g_component);
  add_child(b_component);

  MenuControl::make_tab_list({ r_component, g_component, b_component });
}

ColorSelector::~ColorSelector()
{
  delete r_component;
  delete g_component;
  delete b_component;
}

void
ColorSelector::size_changed()
{
  r_component->set_size(Vec2(get_size().x - 32, 24));
  g_component->set_size(Vec2(get_size().x - 32, 24));
  b_component->set_size(Vec2(get_size().x - 32, 24));
}

void
ColorSelector::update(float time_elapsed)
{

}

void
ColorSelector::draw()
{
  Vec2 offset = get_global_offset();

  // Background
  GraphicsServer::get()->draw_color_rect(offset, get_size(),
    Vec4(0.2, 0.2, 0.2, 1.0));

  /* Display color */
  Vec4 color = Vec4(0, 0, 0, 1);
  try
  {
    Vec4 _color = Vec4(0, 0, 0, 1);
    _color.x = std::stof(r_component->get_text(), nullptr);
    _color.y = std::stof(g_component->get_text(), nullptr);
    _color.z = std::stof(b_component->get_text(), nullptr);
    color = _color;
  }
  catch (const std::exception &)
  {
    /* TODO: if the colors are not numbers, display an image? Or the text inputs
       should only accept valid numbers. */
  }
  GraphicsServer::get()->draw_color_rect(offset + Vec2(16, 16),
    Vec2(get_size().x - 32, 16), color);
}

FlexContainer::FlexContainer(std::string _title, MenuControl *_child) :
  MenuControl(Vec2(128, 128), Vec2(256, 256)), title(_title), child(_child),
  drag_position(false), drag_size(false)
{
  add_child(child);
  resize_child();
}

FlexContainer::~FlexContainer()
{

}

void
FlexContainer::resize_child()
{
  child->set_origin(Vec2(0, 0));
  child->set_size(get_size() - Vec2(0, 16));
}

void
FlexContainer::update(float time_elapsed)
{
  Vec2 cursor_pos = InputMonitor::get()->get_mouse_position();
  if (drag_position)
  {
    set_origin(original_position
      + (cursor_pos - position_grab_point));
  }

  if (drag_size)
  {
    set_size(original_size
      + (Vec2(cursor_pos.x, cursor_pos.y) - size_grab_point));
    resize_child();
  }
}

void
FlexContainer::draw()
{
  Vec2 offset = get_global_offset();

  /* Draw a 16px tall title bar at the top. */
  Vec4 title_bar_color = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
  GraphicsServer::get()->draw_color_rect(offset + Vec2(0, get_size().y) - Vec2(0, 16),
    Vec2(get_size().x, 16), title_bar_color);
  {
    TextRenderRequest req = {};
    req.font = EngineState::get()->get_serif();
    req.center = true;
    req.center_vertical = true;
    req.size = 12;
    req.color = Vec4(0, 0, 0, 1);
    req.bounding_box_origin = offset + Vec2(0, get_size().y) - Vec2(0, 16);
    req.bounding_box_size = Vec2(get_size().x, 16);
    req.text = title;
    req.mask_bounds = true;
    GraphicsServer::get()->draw_text_line(req);
  }

  /* Draw a 16px * 16px resize thing in the top right corner. */
  Vec4 resize_color = Vec4(0.7f, 0.7f, 0.7f, 1.0f);
  GraphicsServer::get()->draw_color_rect(offset + get_size() - Vec2(16, 16),
    Vec2(16, 16), resize_color);
}

void
FlexContainer::handle_event(const MenuControlEvent *event)
{
  if (event->type == MenuControlEventTypeMouseButton)
  {
    Vec2 offset = get_global_offset();
    Vec2 cursor_pos = InputMonitor::get()->get_mouse_position();

    if (!drag_size && event->pressed == true &&
        cursor_pos.inside_rect(offset + get_size() - Vec2(16, 16),
          Vec2(16, 16)))
    {
      drag_size = true;
      original_size = get_size();
      size_grab_point = cursor_pos;
    }
    else if (!drag_position && event->pressed == true &&
             cursor_pos.inside_rect(offset + Vec2(0, get_size().y) - Vec2(0, 16),
               Vec2(get_size().x, 16)))
    {
      drag_position = true;
      original_position = get_origin();
      position_grab_point = cursor_pos;
    }

    if (drag_position && event->pressed == false)
    {
      drag_position = false;
    }

    if (drag_size && event->pressed == false)
    {
      drag_size = false;
    }
  }
}
