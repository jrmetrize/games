#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include "linear_algebra.h"
#include "graphics.h"

class InputMonitor;
class Button;
class DialogueTree;

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
  LevelGeometryBlock(Vec2 _position, Vec2 _size, Material *material);

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

struct Tile
{
  std::string tile;
};

class Tilemap
{
  int origin_x;
  int origin_y;

  unsigned int width;
  unsigned int height;

  std::vector<Tile> tiles;

  std::map<std::string, Material *> material_library;
public:
  Tilemap(int _origin_x, int _origin_y,
    unsigned int _width, unsigned int _height);

  ~Tilemap();

  void
  set_tile(unsigned int x, unsigned int y, const Tile &tile);

  void
  set_material(std::string name, Material *material);

  void
  add_geometry(std::vector<LevelGeometryBlock *> &out);
};

class Level
{
  std::string name;
  float gravity;
  float player_speed;
  CameraMode camera_mode;
  float sun_angle;

  Tilemap *tilemap;

  std::vector<LevelGeometryBlock *> geometry;
public:
  Level();

  Level(Tilemap *_tilemap);

  ~Level();

  std::string
  get_name() const;

  void
  set_name(std::string _name);

  float
  get_gravity() const;

  void
  set_gravity(float _gravity);

  float
  get_player_speed() const;

  void
  set_player_speed(float _player_speed);

  CameraMode
  get_camera_mode() const;

  void
  set_camera_mode(CameraMode _camera_mode);

  float
  get_sun_angle() const;

  void
  set_sun_angle(float _sun_angle);

  const std::vector<LevelGeometryBlock *> &
  get_geometry();
};

class LevelState
{
  Level *level;

  Vec2 player_position;
  Vec2 player_velocity;
  float player_camera_angle;

  DialogueTree *current_dialogue;
  Button *dialogue_choices[6];

  void
  dialogue_choice_callback(void *userdata);
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
  set_current_dialogue(DialogueTree *_current_dialogue);

  void
  draw_background_in_rect(GraphicsServer *graphics_server,
    Vec2 origin, Vec2 size);

  void
  draw_stats_in_rect(GraphicsServer *graphics_server,
    Vec2 origin, Vec2 size);

  void
  draw_side_view_in_rect(GraphicsServer *graphics_server,
    Vec2 origin, Vec2 size);

  void
  draw_dialogue_box_in_rect(GraphicsServer *graphics_server,
    Vec2 origin, Vec2 size);
};

#endif
