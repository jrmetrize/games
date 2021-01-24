#include "levels/training_levels.h"
#include "util.h"
#include "resource.h"
#include "input.h"
#include "level.h"
#include "state.h"

#include "json.hpp"
using json = nlohmann::json;

ElectricMaterial::ElectricMaterial(const Vec4 &_color) :
  color(_color)
{
  texture = new NoiseTexture(1234, 2);
}

ElectricMaterial::~ElectricMaterial()
{
  delete texture;
}

void
ElectricMaterial::set_color(const Vec4 &_color)
{
  color = _color;
}

Vec4
ElectricMaterial::light(Vec2 ray_dir, Vec2 hit_location, RenderObject *obj)
{
  Vec3 noise_pos = Vec3(hit_location.x, hit_location.y, GameState::get()->get_time() / 15.0f);

  float x = (0.5 * texture->value_at(noise_pos).x) + 1;
  float dot = ray_dir.normalized() * obj->get_normal();
  Vec4 value = dot * dot * x * color;
  value.w = 1;
  return value;
}

TrainingLevelController::TrainingLevelController() :
  colors()
{
  ResourceBundle *bundle =
    new ResourceBundle(local_to_absolute_path("resources/training_levels.rbz"));

  json j = json::parse(((Text *)bundle->get_resource("data"))->get_text());
  for (const auto &c : j["colors"].items())
  {
    Vec4 color = Vec4(c.value()["r"], c.value()["g"], c.value()["b"], 1);
    colors[c.key()] = color;
  }

  map = new Tilemap(-2, -10, 32, 32);
  for (const auto &c : j["tilemaps"]["level0"].items())
  {
    Tile t = {};
    t.tile = c.value()["tile"];
    map->set_tile(c.value()["x"], c.value()["y"] + 8, t);
  }
  map->set_material("purple_electric_block", new ElectricMaterial(colors["vw_purple"]));

  DialogueTree *tree = new DialogueTree();
  for (const auto &c : j["dialogue"]["dia0"].items())
  {
    std::string name = c.value()["name"];
    std::string text = c.value()["text"];
    std::string next = "";
    if (c.value().contains("next"))
      next = c.value()["next"];
    DialoguePoint p = DialoguePoint(text, next);
    if (c.value().contains("choices"))
    {
      for (const auto &c2 : c.value()["choices"].items())
      {
        DialogueChoice choice = {};
        choice.text = c2.value()["text"];
        choice.next = c2.value()["next"];
        p.add_choice(choice);
      }
    }
    tree->add_point(name, p);
  }
  tree->set_current("0");

  level = new Level(map);
  level->set_gravity(9.81);
  level->set_player_speed(5.0);

  state = new LevelState(level);
  state->set_current_dialogue(tree);

  delete bundle;
}

TrainingLevelController::~TrainingLevelController()
{
  delete map;
  delete level;
  delete state;
}

LevelState *
TrainingLevelController::get_level_state()
{
  return state;
}
