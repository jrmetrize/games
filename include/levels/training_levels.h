#ifndef TRAINING_LEVELS_H
#define TRAINING_LEVELS_H

#include <map>
#include <string>
#include "linear_algebra.h"

class Tilemap;
class Level;
class LevelState;
class InputMonitor;

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
