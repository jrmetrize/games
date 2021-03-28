#include "screens/level_editor_screen.h"
#include "core/state.h"
#include "level.h"
#include "core/graphics.h"
#include "levels/training_levels.h"

LevelEditorScreen::LevelEditorScreen() :
  paused(false)
{
  level = new TrainingLevelController();
}

LevelEditorScreen::~LevelEditorScreen()
{
  delete level;
}

void
LevelEditorScreen::update(float time_elapsed)
{
  if (!paused)
    level->get_level_state()->update(time_elapsed);
}

void
LevelEditorScreen::draw_custom()
{
  // Draw the 2D representation of the level
  LevelState *level_state = level->get_level_state();
  /*
  level_state->draw_side_view_in_rect(graphics_server,
    Vec2(10, 10), Vec2(512, 512));
  level_state->draw_dialogue_box_in_rect(graphics_server,
    Vec2(700, 10), Vec2(512, 512));
  */
  RenderRequest req = {};
  req.camera_pos = level_state->get_player_position();
  req.camera_dir = level_state->get_player_camera_direction();
  req.tree = level_state->get_render_tree();
  req.camera_mode = Vertical;
  GraphicsServer::get()->render_to_rect(Vec2(600, 0), Vec2(64, 720), req);

  /*char name_tmp[256];
  strcpy(name_tmp, level->get_name().c_str());
  float gravity_tmp = level->get_gravity();
  float player_speed_tmp = level->get_player_speed();
  int camera_mode_tmp = level->get_camera_mode();*/
}
