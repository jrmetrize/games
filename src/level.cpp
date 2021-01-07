#include "level.h"
#include "graphics.h"
#include "input.h"

LevelGeometryBlock::LevelGeometryBlock(Vec2 _position, Vec2 _size) :
  position(_position),
  size(_size)
{

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
}

Level::Level() :
  name(""),
  gravity(0),
  camera_mode(Vertical),
  geometry()
{
  LevelGeometryBlock *block = new LevelGeometryBlock(Vec2(0, -2), Vec2(1, 1));
  geometry.push_back(block);
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

const std::vector<LevelGeometryBlock *> &
Level::get_geometry() const
{
  return geometry;
}

LevelState::LevelState(Level *_level) :
  level(_level),
  player_position(),
  player_velocity()
{

}

LevelState::~LevelState()
{

}

void
LevelState::update(InputMonitor *input, float time_elapsed)
{
  player_velocity += time_elapsed * Vec2(0, -level->get_gravity());
  player_position += time_elapsed * player_velocity;

  if (input->is_key_down(KeySpace))
  {
    player_velocity += Vec2(0, 1);
  }

  // Physics
}

Vec2
LevelState::get_player_position() const
{
  return player_position;
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
  graphics_server->draw_color_rect(origin + ((1.0 / 2.0) * size),
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
