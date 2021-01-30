#include "state.h"
#include "graphics.h"
#include "input.h"
#include "resource.h"
#include "util.h"
#include "screens/level_editor_screen.h"
#include "screens/level_screen.h"
#include "screens/options_screen.h"
#include "screens/splash_screen.h"
#include "screens/title_screen.h"
#include "json.hpp"
using json = nlohmann::json;

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
  level_editor_screen = new LevelEditorScreen();
  level_screen = new LevelScreen();
  options_screen = new OptionsScreen();
  splash_screen = new SplashScreen();
  title_screen = new TitleScreen();

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

    options_screen->build_tree(settings_data["options_menu"], properties);
  }

  current_screen = splash_screen;
  current_screen->to_appear();
}

GameState::~GameState()
{
  delete level_editor_screen;
  delete level_screen;
  delete options_screen;
  delete splash_screen;
  delete title_screen;

  delete font_bundle;
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
  current_screen->update(time_elapsed);
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

Screen *
GameState::get_level_editor_screen()
{
  return level_editor_screen;
}

Screen *
GameState::get_level_screen()
{
  return level_screen;
}

Screen *
GameState::get_options_screen()
{
  return options_screen;
}

Screen *
GameState::get_splash_screen()
{
  return splash_screen;
}

Screen *
GameState::get_title_screen()
{
  return title_screen;
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
  current_screen->to_disappear();
  target_screen->to_appear();

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
