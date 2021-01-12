#include "level.h"
#include "graphics.h"
#include "input.h"

AABB::AABB(Vec2 _center, Vec2 _size) :
  center(_center),
  size(_size)
{

}

AABBCollisionShape::AABBCollisionShape(Vec2 _center, Vec2 _size) :
  center(_center),
  size(_size)
{

}

Vec2
AABBCollisionShape::get_center() const
{
  return center;
}

void
AABBCollisionShape::set_center(Vec2 _center)
{
  center = _center;
}

Vec2
AABBCollisionShape::get_size() const
{
  return size;
}

void
AABBCollisionShape::set_size(Vec2 _size)
{
  size = _size;
}

AABB
AABBCollisionShape::get_bounding_aabb() const
{
  return AABB(center, size);
}

CollisionResult
test_collision(AABBCollisionShape &a, AABBCollisionShape &b)
{
  CollisionResult r = {};

  Vec2 a_min = a.get_center() - (0.5 * a.get_size());
  Vec2 a_max = a.get_center() + (0.5 * a.get_size());
  Vec2 b_min = b.get_center() - (0.5 * b.get_size());
  Vec2 b_max = b.get_center() + (0.5 * b.get_size());

  float distances[4] = {
    a_max.x - b_min.x,
    b_max.x - a_min.x,
    a_max.y - b_min.y,
    b_max.y - a_min.y
  };

  Vec2 normals[4] = {
    Vec2(-1, 0),
    Vec2(1, 0),
    Vec2(0, -1),
    Vec2(0, 1)
  };

  float epsilon = 0.001;

  r.intersection = true;
  for (unsigned int i = 0; i < 4; ++i)
  {
    if (distances[i] <= epsilon)
      r.intersection = false;
  }

  unsigned int normal_index = 0;
  for (unsigned int i = 1; i < 4; ++i)
  {
    if (distances[i] < distances[normal_index])
      normal_index = i;
  }
  r.normal = normals[normal_index];
  r.depth = distances[normal_index];

  return r;
}

LevelGeometryBlock::LevelGeometryBlock(Vec2 _position, Vec2 _size) :
  position(_position),
  size(_size),
  left(position - (0.5 * size), position + (0.5 * Vec2(-size.x, size.y))),
  top(position + (0.5 * Vec2(-size.x, size.y)), position + (0.5 * size)),
  right(position + (0.5 * size), position + (0.5 * Vec2(size.x, -size.y))),
  bottom(position + (0.5 * Vec2(size.x, -size.y)), position - (0.5 * size))
{
  left.set_color(Vec3(1, 0, 0));
  right.set_color(Vec3(0, 1, 0));
  top.set_color(Vec3(0.5, 0.5, 0));
  bottom.set_color(Vec3(0, 0, 1));
}

void
LevelGeometryBlock::update_render_geometry()
{
  left = Segment(position - (0.5 * size),
    position + (0.5 * Vec2(-size.x, size.y)));
  top = Segment(position + (0.5 * Vec2(-size.x, size.y)),
    position + (0.5 * size));
  right = Segment(position + (0.5 * size),
    position + (0.5 * Vec2(size.x, -size.y)));
  bottom = Segment(position + (0.5 * Vec2(size.x, -size.y)),
    position - (0.5 * size));
}

Vec2
LevelGeometryBlock::get_position() const
{
  return position;
}

void
LevelGeometryBlock::set_position(Vec2 _position)
{
  position = _position;
  update_render_geometry();
}

Vec2
LevelGeometryBlock::get_size() const
{
  return size;
}

void
LevelGeometryBlock::set_size(Vec2 _size)
{
  size = _size;
  update_render_geometry();
}

void
LevelGeometryBlock::add_geometry_to_tree(RenderTree &tree)
{
  tree.objects.push_back(&top);
  tree.objects.push_back(&bottom);
  tree.objects.push_back(&left);
  tree.objects.push_back(&right);
}

Level::Level() :
  name(""),
  gravity(0),
  player_speed(1),
  camera_mode(Vertical),
  sun_angle(1.6 * 3.14159),
  geometry()
{
  LevelGeometryBlock *block = new LevelGeometryBlock(Vec2(0, -2), Vec2(5, 1));
  geometry.push_back(block);

  LevelGeometryBlock *block2 = new LevelGeometryBlock(Vec2(8, -2), Vec2(5, 1));
  geometry.push_back(block2);
}

Level::~Level()
{
  for (unsigned int i = 0; i < geometry.size(); ++i)
    delete geometry[i];
}

std::string
Level::get_name() const
{
  return name;
}

void
Level::set_name(std::string _name)
{
  name = _name;
}

float
Level::get_gravity() const
{
  return gravity;
}

void
Level::set_gravity(float _gravity)
{
  gravity = _gravity;
}

float
Level::get_player_speed() const
{
  return player_speed;
}

void
Level::set_player_speed(float _player_speed)
{
  player_speed = _player_speed;
}

CameraMode
Level::get_camera_mode() const
{
  return camera_mode;
}

void
Level::set_camera_mode(CameraMode _camera_mode)
{
  camera_mode = _camera_mode;
}

float
Level::get_sun_angle() const
{
  return sun_angle;
}

void
Level::set_sun_angle(float _sun_angle)
{
  sun_angle = _sun_angle;
}

const std::vector<LevelGeometryBlock *> &
Level::get_geometry() const
{
  return geometry;
}

LevelState::LevelState(Level *_level) :
  level(_level),
  player_position(),
  player_velocity(),
  player_camera_angle()
{

}

LevelState::~LevelState()
{

}

void
LevelState::update(InputMonitor *input, float time_elapsed)
{
  Vec2 next_velocity = player_velocity
    + (time_elapsed * Vec2(0, -level->get_gravity()));
  Vec2 next_position = player_position
    + (time_elapsed * next_velocity);

  AABBCollisionShape player_collider = AABBCollisionShape(next_position,
    Vec2(0.5, 0.5));
  CollisionResult collision;
  for (unsigned int i = 0; i < level->get_geometry().size(); ++i)
  {
    LevelGeometryBlock *block = level->get_geometry()[i];
    AABBCollisionShape tile_collider = AABBCollisionShape(block->get_position(),
      block->get_size());
    collision = test_collision(player_collider, tile_collider);
    if (collision.intersection)
      break;
  }
  if (!collision.intersection)
  {
    player_velocity = next_velocity;
    player_position = next_position;
  }
  else
  {
    player_velocity = Vec2();
    player_position = next_position + (collision.depth * collision.normal);
  }
  bool on_ground = false;
  if (collision.intersection && player_velocity.y <= 0)
    on_ground = true;

  if (on_ground && input->get_jump_input())
  {
    player_velocity += Vec2(0, 10);
  }

  if (input->is_key_down(KeyW))
    player_position += time_elapsed * level->get_player_speed() * Vec2(1, 0);
  else if (input->is_key_down(KeyS))
    player_position += time_elapsed * level->get_player_speed() * Vec2(-1, 0);
  player_position += -input->gamepad_left_stick().y * time_elapsed
    * level->get_player_speed() * Vec2(1, 0);

  if (input->is_key_down(KeyUp))
    player_camera_angle += time_elapsed * 1;
  else if (input->is_key_down(KeyDown))
    player_camera_angle += time_elapsed * -1;
  player_camera_angle += -input->gamepad_right_stick().y * time_elapsed * 1;
}

Vec2
LevelState::get_player_position() const
{
  return player_position;
}

Vec2
LevelState::get_player_camera_direction() const
{
  return Vec2(cos(player_camera_angle), sin(player_camera_angle));
}

RenderTree
LevelState::get_render_tree()
{
  const std::vector<LevelGeometryBlock *> &geometry =
    level->get_geometry();

  RenderTree tree = {};
  tree.sun_direction = Vec2(cos(level->get_sun_angle()),
    sin(level->get_sun_angle()));
  tree.objects = std::vector<RenderObject *>();
  for (unsigned int i = 0; i < geometry.size(); ++i)
    geometry[i]->add_geometry_to_tree(tree);

  return tree;
}

void
LevelState::set_player_position(Vec2 _position)
{
  player_position = _position;
}

void
LevelState::draw_editor_view_in_rect(GraphicsServer *graphics_server,
  Vec2 origin, Vec2 size)
{
  // Border and background
  const float margin = 10;
  graphics_server->draw_color_rect(origin - Vec2(margin, margin),
    size + (2 * Vec2(margin, margin)), Vec4(1, 1, 1, 1));
  graphics_server->draw_color_rect(origin,
    size, Vec4(0.2, 0.2, 0.2, 1));

  // Use 16 pixels = 1 meter
  // Draw the player
  graphics_server->draw_color_rect(origin + ((1.0 / 2.0) * size) - Vec2(4, 4),
    Vec2(8, 8), Vec4(0.8, 0.2, 0.1, 1));

  // Draw the level
  const std::vector<LevelGeometryBlock *> &geometry =
    level->get_geometry();
  Mat3 world_to_screen_transform = Mat3::translate(origin + ((1.0 / 2.0) * size))
    * Mat3::scale(Vec2(16, 16)) * Mat3::translate(-player_position);
  for (unsigned int i = 0; i < geometry.size(); ++i)
  {
    Vec2 block_origin = geometry[i]->get_position()
      - ((1.0 / 2.0) * geometry[i]->get_size());
    Vec2 block_size = geometry[i]->get_size();
    Vec3 screen_origin = world_to_screen_transform
      * Vec3(block_origin.x, block_origin.y, 1);
    graphics_server->draw_color_rect(Vec2(screen_origin.x, screen_origin.y),
      16.0 * block_size, Vec4(0, 1, 0, 1));
  }
}
