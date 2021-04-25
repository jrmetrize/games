#include "core/state.h"
#include "core/graphics.h"
#include "core/input.h"
#include "core/resource.h"
#include "core/util.h"
#include "core/screen.h"

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

BoundFont::BoundFont(FontFace *_face) :
  face(_face), textures()
{
  face->generate_textures();
  std::string chars = face->get_chars();
  for (char c : chars)
  {
    Texture *t = face->get_texture(c);
    BoundTexture *bound = GraphicsServer::get()->bind(t);
    textures[c] = bound;
  }
}

BoundFont::~BoundFont()
{
  std::string chars = face->get_chars();
  for (char c : chars)
  {
    delete textures[c];
  }
}

FontFace *
BoundFont::get_font()
{
  return face;
}

BoundTexture *
BoundFont::get_bound_texture(char c)
{
  return textures[c];
}

EngineState * EngineState::instance = nullptr;

EngineState::EngineState() :
  should_close(false),
  current_screen(nullptr),
  ref(std::chrono::steady_clock::now()),
  last_update(ref),
  properties()
{
  font_bundle = new ResourceBundle(local_to_absolute_path("resources/fonts.rbz"));
  global_bundle = new ResourceBundle(local_to_absolute_path("resources/global.rbz"));

  serif = new BoundFont(reinterpret_cast<FontFace *>(font_bundle->get_resource("serif")));
  sans = new BoundFont(reinterpret_cast<FontFace *>(font_bundle->get_resource("sans")));

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
      std::bind(&EngineState::fullscreen_changed, this, std::placeholders::_1);
  }
}

EngineState::~EngineState()
{
  delete font_bundle;
  delete global_bundle;

  delete serif;
  delete sans;
}

void
EngineState::fullscreen_changed(PropertyData *prop)
{
  GraphicsServer::get()->set_fullscreen(std::get<bool>(prop->data));
}

void
EngineState::set_instance(EngineState *_instance)
{
  instance = _instance;
}

EngineState *
EngineState::get()
{
  return instance;
}

void
EngineState::update(float time_elapsed)
{
  last_update = std::chrono::steady_clock::now();
  GraphicsServer::get()->set_current_screen(current_screen);
  if (current_screen != nullptr)
    current_screen->update(time_elapsed);
}

float
EngineState::get_time() const
{
  uint32_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
    last_update - ref
  ).count();
  return float(milliseconds) / 1000.0f;
}

BoundFont *
EngineState::get_sans()
{
  return sans;
}

BoundFont *
EngineState::get_serif()
{
  return serif;
}

ResourceBundle *
EngineState::get_globals()
{
  return global_bundle;
}

PropertyData &
EngineState::get_property(std::string name)
{
  return properties[name];
}

void
EngineState::switch_to_screen(Screen *target_screen)
{
  if (current_screen != nullptr)
  {
    current_screen->remove_listener();
    current_screen->to_disappear();
  }
  target_screen->to_appear();
  target_screen->install_listener();

  GraphicsServer::get()->set_current_screen(target_screen);
  current_screen = target_screen;
}

void
EngineState::close_game()
{
  should_close = true;
}

bool
EngineState::game_open() const
{
  return !should_close;
}
