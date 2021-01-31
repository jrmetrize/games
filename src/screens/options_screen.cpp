#include "screens/options_screen.h"
#include "graphics.h"

const float top_bar_size = 64.0f;

float
OptionsScreen::OptionSelector::draw(Vec2 offset)
{
  bool_switch->origin = offset + Vec2(256, 5);
  bool_switch->size = Vec2(256, 40);
  bool_switch->draw();

  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  TextRenderRequest req = {};
  req.bounding_box_origin = offset;
  req.bounding_box_size = Vec2(window_size.x, 32);
  req.text = text;
  req.font = GameState::get()->get_serif();
  req.center_vertical = true;
  req.size = 18;
  req.color = Vec4(1);
  GraphicsServer::get()->draw_text(req);
  return 50;
}

void
OptionsScreen::OptionSelector::mouse_pressed(MouseButton button, bool pressed)
{
  bool_switch->mouse_pressed(button, pressed);
}

void
OptionsScreen::OptionSelector::bool_switch_changed(bool value)
{
  // If this is being called, we know that the property is a bool
  game_property->set_data(value);
}

void
OptionsScreen::OptionSelector::update(float time_elapsed)
{
  bool_switch->update();
}

float
OptionsScreen::OptionsList::draw(Vec2 offset)
{
  float y_offset = offset.y;

  {
    Image *arrow;
    if (open)
      arrow = (Image *)GameState::get()->get_globals()->get_resource("arrow_close");
    else
      arrow = (Image *)GameState::get()->get_globals()->get_resource("arrow_open");
    arrow->generate_texture();
    if (arrow_button->highlighted)
      GraphicsServer::get()->draw_texture_rect(Vec2(offset.x + 20, offset.y), Vec2(32), *arrow->get_texture());
    else
      GraphicsServer::get()->draw_texture_rect(Vec2(offset.x + 16, offset.y), Vec2(32), *arrow->get_texture());
    arrow_button->origin = Vec2(offset.x + 16, offset.y);
    arrow_button->size = Vec2(32);

    TextRenderRequest req = {};
    req.bounding_box_origin = Vec2(offset.x + 64, y_offset);
    req.bounding_box_size = Vec2(512, 32);
    req.text = name;
    req.font = GameState::get()->get_serif();
    req.center_vertical = true;
    req.size = 18;
    req.color = Vec4(1);
    GraphicsServer::get()->draw_text_line(req);

    y_offset -= 50;
  }

  if (open)
  {
    for (unsigned int i = 0; i < entries.size(); ++i)
    {
      Vec2 entry_offset = Vec2(offset.x + 64, y_offset);
      y_offset -= entries[i]->draw(entry_offset);
    }
  }

  return offset.y - y_offset;
}

void
OptionsScreen::OptionsList::mouse_pressed(MouseButton button, bool pressed)
{
  arrow_button->mouse_pressed(button, pressed);
  if (open)
  {
    for (unsigned int i = 0; i < entries.size(); ++i)
    {
      entries[i]->mouse_pressed(button, pressed);
    }
  }
}

void
OptionsScreen::OptionsList::update(float time_elapsed)
{
  arrow_button->update();
  if (open)
  {
    for (unsigned int i = 0; i < entries.size(); ++i)
    {
      entries[i]->update(time_elapsed);
    }
  }
}

OptionsScreen::OptionsScreen() :
  scroll_offset(0)
{
  listener.mouse_button_handle = std::bind(&OptionsScreen::mouse_pressed, this,
    std::placeholders::_1, std::placeholders::_2);
  listener.scroll_handle = std::bind(&OptionsScreen::mouse_scrolled, this,
    std::placeholders::_1);

  root.name = "Options";

  back_button = new MenuButton("Back", Vec2(0, 0), Vec2(128, 64),
    std::bind(&OptionsScreen::back_pressed, this));
}

OptionsScreen::~OptionsScreen()
{
  delete back_button;
}

void
OptionsScreen::parse_options_group(const json &group, OptionsList *parent,
  std::map<std::string, PropertyData> &properties)
{
  OptionsList *list = new OptionsList();
  list->name = group["group_name"];
  list->open = true;
  list->arrow_button = new MenuButton("", Vec2(), Vec2(),
    std::bind(&OptionsScreen::toggle_list_open, this, list));

  for (const auto &option_kv : group["settings"].items())
  {
    const json &option = option_kv.value();
    if (option.type() == json::value_t::string)
    {
      PropertyData &prop = properties[option];

      OptionSelector *item = new OptionSelector();
      item->text = prop.property_text;
      item->bool_switch = new MenuSwitch(Vec2(), Vec2(),
        std::bind(&OptionsScreen::OptionSelector::bool_switch_changed, item, std::placeholders::_1));
      item->game_property = &prop;
      list->entries.push_back(item);
    }
    else if (option.type() == json::value_t::object)
    {
      parse_options_group(option, list, properties);
    }
  }

  parent->entries.push_back(list);
}

void
OptionsScreen::build_tree(const json &tree_structure,
  std::map<std::string, PropertyData> &properties)
{
  // We have a json structure detailing how the options are arranged and which
  // properties they control, so use this to construct the visual tree.
  root.arrow_button = new MenuButton("", Vec2(), Vec2(),
    std::bind(&OptionsScreen::toggle_list_open, this, &root));
  for (const auto &group_kv : tree_structure.items())
  {
    parse_options_group(group_kv.value(), &root, properties);
  }
}

void
OptionsScreen::mouse_pressed(MouseButton button, bool pressed)
{
  root.mouse_pressed(button, pressed);
  back_button->mouse_pressed(button, pressed);
}

void
OptionsScreen::mouse_scrolled(Vec2 scroll)
{
  scroll_offset += -50.0f * scroll.y;
  if (scroll_offset < 0)
    scroll_offset = 0;
  if (scroll_offset > list_height - GraphicsServer::get()->get_framebuffer_size().y + top_bar_size)
    scroll_offset = list_height - GraphicsServer::get()->get_framebuffer_size().y + top_bar_size;
}

void
OptionsScreen::toggle_list_open(OptionsList *list)
{
  list->open = !list->open;
}

void
OptionsScreen::back_pressed()
{
  GameState::get()->switch_to_screen(GameState::get()->get_title_screen());
}

void
OptionsScreen::update(float time_elapsed)
{
  root.update(time_elapsed);
  back_button->update();
}

void
OptionsScreen::to_appear()
{
  InputMonitor::get()->install_listener(&listener);
}

void
OptionsScreen::to_disappear()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
OptionsScreen::draw_custom()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();

  // Loop through the options entries
  {
    list_height = root.draw(Vec2(0, window_size.y - 50.0f + scroll_offset
      - top_bar_size));
  }

  // Draw the bar at the top
  {
    GraphicsServer::get()->draw_color_rect(Vec2(0, window_size.y - top_bar_size),
      Vec2(window_size.x, top_bar_size), Vec4(0, 0, 0, 1));
    GraphicsServer::get()->draw_color_rect(Vec2(0, window_size.y - top_bar_size),
      Vec2(window_size.x, 2), Vec4(1));
    back_button->origin = Vec2(16, window_size.y - top_bar_size);
    back_button->draw();
  }
}
