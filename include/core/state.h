#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <variant>

class Screen;

#include "core/resource.h"
#include <json.hpp>
using json = nlohmann::json;

struct DialogueChoice
{
  std::string text;
  std::string next;
};

class DialoguePoint
{
  std::string text;
  std::string next;
  std::vector<DialogueChoice> choices;
public:
  DialoguePoint();

  DialoguePoint(std::string _text, std::string _next);

  void
  add_choice(const DialogueChoice &choice);

  std::string
  get_text() const;

  const std::vector<DialogueChoice> &
  get_choices() const;
};

class DialogueTree
{
  std::map<std::string, DialoguePoint> points;
  std::string current_point;
public:
  DialogueTree();

  void
  add_point(std::string name, const DialoguePoint &point);

  void
  set_current(std::string current);

  const DialoguePoint &
  get_current_point();

  void
  choice_made(unsigned int index);
};

struct EnumData
{
  struct EnumChoice
  {
    std::string name;
    std::string text;
  };

  std::vector<EnumChoice> choices;
  unsigned int current;
};

struct PropertyData
{
  std::string property_name;
  std::string property_type;
  std::string property_text;
  std::variant<bool, float, EnumData> data;
  std::function<void(PropertyData *)> changed_callback;

  static PropertyData
  from_json(const json &spec);

  template<typename T> void
  set_data(T _data)
  {
    data = _data;
    if (changed_callback)
      changed_callback(this);
  }
};

class GameConfig
{
public:
  GameConfig();

  static GameConfig *
  from_json(const json &data);

  json
  get_json() const;
};

class BoundTexture;

class BoundFont
{
  FontFace *face;
  std::map<char, BoundTexture *> textures;
public:
  BoundFont(FontFace *_face);

  ~BoundFont();

  FontFace *
  get_font();

  BoundTexture *
  get_bound_texture(char c);
};

class GameState
{
  std::chrono::time_point<std::chrono::steady_clock> ref;
  std::chrono::time_point<std::chrono::steady_clock> last_update;

  static GameState *instance;

  bool should_close;

  Screen *current_screen;

  ResourceBundle *font_bundle;
  ResourceBundle *global_bundle;

  BoundFont *serif;
  BoundFont *sans;

  std::map<std::string, PropertyData> properties;

  void
  fullscreen_changed(PropertyData *prop);
public:
  GameState();

  ~GameState();

  static void
  set_instance(GameState *_instance);

  static GameState *
  get();

  void
  update(float time_elapsed);

  float
  get_time() const;

  BoundFont *
  get_sans();

  BoundFont *
  get_serif();

  ResourceBundle *
  get_globals();

  PropertyData &
  get_property(std::string name);

  void
  switch_to_screen(Screen *target_screen);

  void
  close_game();

  bool
  game_open() const;
};

#endif
