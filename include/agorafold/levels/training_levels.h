#ifndef TRAINING_LEVELS_H
#define TRAINING_LEVELS_H

#include <map>
#include <string>
#include "core/linear_algebra.h"
#include "core/graphics.h"

class Tilemap;
class Level;
class LevelState;
class InputMonitor;

class ElectricMaterial : public Material
{
  NoiseTexture *texture;
  Vec4 color;
public:
  ElectricMaterial(const Vec4 &_color);

  ~ElectricMaterial();

  void
  set_color(const Vec4 &_color);

  Vec4
  light(Vec2 ray_dir, Vec2 hit_location, RenderObject *obj);
};

class TrainingLevelController
{
  std::map<std::string, Vec4> colors;

  Tilemap *map;

  Level *level;
  LevelState *state;
public:
  TrainingLevelController();

  ~TrainingLevelController();

  LevelState *
  get_level_state();
};

#endif
