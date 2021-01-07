#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include "linear_algebra.h"

class GraphicsServer;
class InputMonitor;

enum CameraMode
{
  Vertical = 0,
  Horizontal
};

class LevelGeometryBlock
{
  Vec2 position;
  Vec2 size;
public:
  LevelGeometryBlock(Vec2 _position, Vec2 _size);

  Vec2
  get_position() const;

  void
  set_position(Vec2 _position);

  Vec2
  get_size() const;

  void
  set_size(Vec2 _size);
};

class Level
{
  std::string name;
  float gravity;
  CameraMode camera_mode;

  std::vector<LevelGeometryBlock *> geometry;
public:
  Level();

  ~Level();

  std::string
  get_name() const;

  void
  set_name(std::string _name);

  float
  get_gravity() const;

  void
  set_gravity(float _gravity);

  CameraMode
  get_camera_mode() const;

  void
  set_camera_mode(CameraMode _camera_mode);

  const std::vector<LevelGeometryBlock *> &
  get_geometry() const;
};

class LevelState
{
  Level *level;

  Vec2 player_position;
  Vec2 player_velocity;
public:
  LevelState(Level *_level);

  ~LevelState();

  void
  update(InputMonitor *input, float time_elapsed);

  Vec2
  get_player_position() const;

  void
  set_player_position(Vec2 _position);

  void
  draw_editor_view_in_rect(GraphicsServer *graphics_server,
    Vec2 origin, Vec2 size);
};

#endif
