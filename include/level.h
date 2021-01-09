#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include "linear_algebra.h"
#include "graphics.h"

class InputMonitor;

struct AABB
{
  Vec2 center;
  Vec2 size;

  AABB(Vec2 _center, Vec2 _size);
};

class CollisionShape
{
public:
  virtual AABB
  get_bounding_aabb() const = 0;
};

class AABBCollisionShape : public CollisionShape
{
  Vec2 center;
  Vec2 size;
public:
  AABBCollisionShape(Vec2 _center, Vec2 _size);

  Vec2
  get_center() const;

  void
  set_center(Vec2 _center);

  Vec2
  get_size() const;

  void
  set_size(Vec2 _size);

  AABB
  get_bounding_aabb() const;
};

struct CollisionResult
{
  bool intersection;
  Vec2 normal; // inward normal on A
  float depth;
};

CollisionResult
test_collision(const AABBCollisionShape &a, const AABBCollisionShape &b);

class LevelGeometryBlock
{
  Vec2 position;
  Vec2 size;

  Segment top;
  Segment bottom;
  Segment left;
  Segment right;

  void
  update_render_geometry();
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

  void
  add_geometry_to_tree(RenderTree &tree);
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
  float player_camera_angle;
public:
  LevelState(Level *_level);

  ~LevelState();

  void
  update(InputMonitor *input, float time_elapsed);

  Vec2
  get_player_position() const;

  Vec2
  get_player_camera_direction() const;

  RenderTree
  get_render_tree();

  void
  set_player_position(Vec2 _position);

  void
  draw_editor_view_in_rect(GraphicsServer *graphics_server,
    Vec2 origin, Vec2 size);
};

#endif
