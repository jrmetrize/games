#include "core/state.h"
#include "core/graphics.h"
#include "core/input.h"
#include "core/resource.h"
#include "core/util.h"
/*
#include "screens/level_editor_screen.h"
#include "screens/level_screen.h"
#include "screens/options_screen.h"
#include "screens/splash_screen.h"
#include "screens/title_screen.h"
*/

DialoguePoint::DialoguePoint()
{

}

DialoguePoint::DialoguePoint(std::string _text, std::string _next) :
  text(_text), next(_next), choices()
{

}

void
DialoguePoint::add_choice(const DialogueChoice &choice)
{
  choices.push_back(choice);
}

std::string
DialoguePoint::get_text() const
{
  return text;
}

const std::vector<DialogueChoice> &
DialoguePoint::get_choices() const
{
  return choices;
}

DialogueTree::DialogueTree() :
  points(), current_point()
{

}

void
DialogueTree::add_point(std::string name, const DialoguePoint &point)
{
  points[name] = point;
}

void
DialogueTree::set_current(std::string current)
{
  current_point = current;
}

const DialoguePoint &
DialogueTree::get_current_point()
{
  return points[current_point];
}

void
DialogueTree::choice_made(unsigned int index)
{
  current_point = points[current_point].get_choices()[index].next;
}

PropertyData
PropertyData::from_json(const json &spec)
{
  PropertyData data = {};
  data.property_name = spec["name"];
  data.property_type = spec["type"];
  data.property_text = spec["text"];

  data.data = false;
  if (spec.contains("default"))
  {
    if (data.property_type == "bool")
      data.data = (bool)spec["default"];
    else if (data.property_type == "range")
      data.data = (float)spec["default"];
  }

  if (data.property_type == "enum")
  {
    EnumData enum_data = {};
    for (const auto &choice_kv : spec["choices"].items())
    {
      const json &choice = choice_kv.value();
      EnumData::EnumChoice choice_data = {};
      choice_data.name = choice["name"];
      choice_data.text = choice["text"];

      enum_data.choices.push_back(choice_data);
    }
    data.data = enum_data;
  }

  return data;
}

GameState * GameState::instance = nullptr;

GameState::GameState() :
  should_close(false),
  current_screen(nullptr),
  ref(std::chrono::steady_clock::now()),
  last_update(ref),
  properties()
{
  font_bundle = new ResourceBundle(local_to_absolute_path("resources/fonts.rbz"));
  global_bundle = new ResourceBundle(local_to_absolute_path("resources/global.rbz"));

  {
    json settings_data =
      json::parse(((Text *)global_bundle->get_resource("default_settings"))->get_text());

    for (const auto &setting_entry_kv : settings_data["settings"].items())
    {
      const json &setting_entry = setting_entry_kv.value();
      std::string name = setting_entry["name"];
      properties[name] = PropertyData::from_json(setting_entry);
    }

    properties["is_fullscreen"].changed_callback =
      std::bind(&GameState::fullscreen_changed, this, std::placeholders::_1);
  }
}

GameState::~GameState()
{
  delete font_bundle;
}

void
GameState::fullscreen_changed(PropertyData *prop)
{
  GraphicsServer::get()->set_fullscreen(std::get<bool>(prop->data));
}

void
GameState::set_instance(GameState *_instance)
{
  instance = _instance;
}

GameState *
GameState::get()
{
  return instance;
}

void
GameState::update(float time_elapsed)
{
  last_update = std::chrono::steady_clock::now();
  GraphicsServer::get()->set_current_screen(current_screen);
  //current_screen->update(time_elapsed);
}

float
GameState::get_time() const
{
  uint32_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
    last_update - ref
  ).count();
  return float(milliseconds) / 1000.0f;
}

FontFace *
GameState::get_sans()
{
  return reinterpret_cast<FontFace *>(font_bundle->get_resource("sans"));
}

FontFace *
GameState::get_serif()
{
  return reinterpret_cast<FontFace *>(font_bundle->get_resource("serif"));
}

ResourceBundle *
GameState::get_globals()
{
  return global_bundle;
}

PropertyData &
GameState::get_property(std::string name)
{
  return properties[name];
}

void
GameState::switch_to_screen(Screen *target_screen)
{
  //current_screen->to_disappear();
  //target_screen->to_appear();

  GraphicsServer::get()->set_current_screen(target_screen);
  current_screen = target_screen;
}

void
GameState::close_game()
{
  should_close = true;
}

bool
GameState::game_open() const
{
  return !should_close;
}
