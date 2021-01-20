#include "levels/training_levels.h"
#include "util.h"
#include "resource.h"
#include "input.h"
#include "level.h"

#include "json.hpp"
using json = nlohmann::json;

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

  level = new Level(map);
  level->set_gravity(9.81);
  level->set_player_speed(5.0);

  state = new LevelState(level);

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
