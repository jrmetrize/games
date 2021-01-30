#include "screens/options_screen.h"
#include "graphics.h"
#include "imgui/imgui.h"

float
OptionsScreen::OptionSelector::draw(Vec2 offset)
{
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
    GraphicsServer::get()->draw_texture_rect(Vec2(offset.x + 16, offset.y), Vec2(32), *arrow->get_texture());

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

OptionsScreen::OptionsScreen() :
  scroll_offset(0)
{
  listener.mouse_button_handle = std::bind(&OptionsScreen::mouse_pressed, this,
    std::placeholders::_1, std::placeholders::_2);
  listener.scroll_handle = std::bind(&OptionsScreen::mouse_scrolled, this,
    std::placeholders::_1);

  root.name = "Options";
}

OptionsScreen::~OptionsScreen()
{

}

void
OptionsScreen::parse_options_group(const json &group, OptionsList *parent,
  std::map<std::string, PropertyData> &properties)
{
  OptionsList *list = new OptionsList();
  list->name = group["group_name"];
  list->open = true;

  for (const auto &option_kv : group["settings"].items())
  {
    const json &option = option_kv.value();
    if (option.type() == json::value_t::string)
    {
      PropertyData &prop = properties[option];

      OptionSelector *item = new OptionSelector();
      item->text = prop.property_text;
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
  for (const auto &group_kv : tree_structure.items())
  {
    parse_options_group(group_kv.value(), &root, properties);
  }
}

void
OptionsScreen::mouse_pressed(MouseButton button, bool pressed)
{
  // Calculate the locations of the arrow, and collapse groups

}

void
OptionsScreen::mouse_scrolled(Vec2 scroll)
{
  scroll_offset += -50.0f * scroll.y;
  if (scroll_offset < 0)
    scroll_offset = 0;
  if (scroll_offset > list_height - GraphicsServer::get()->get_framebuffer_size().y)
    scroll_offset = list_height - GraphicsServer::get()->get_framebuffer_size().y;
}

void
OptionsScreen::update(float time_elapsed)
{

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
  // Loop through the options entries
  {
    Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
    list_height = root.draw(Vec2(0, window_size.y - 50.0f + scroll_offset));
  }

  ImGui::Begin("Options Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    GameState::get()->switch_to_screen(GameState::get()->get_title_screen());
  }

  ImGui::End();
}
