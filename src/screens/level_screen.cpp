#include "screens/level_screen.h"
#include "state.h"
#include "level.h"
#include "graphics.h"
#include "imgui/imgui.h"
#include "levels/training_levels.h"

LevelScreen::LevelScreen(GameState *_state) :
  Screen(_state)
{
  level = new TrainingLevelController();
}

LevelScreen::~LevelScreen()
{
  delete level;
}

void
LevelScreen::update(InputMonitor *input, float time_elapsed)
{
  level->get_level_state()->update(input, time_elapsed);
}

void
LevelScreen::draw_custom(GraphicsServer *graphics_server)
{
  // Draw the 2D representation of the level
  LevelState *level_state = level->get_level_state();
  RenderRequest req = {};
  req.camera_pos = level_state->get_player_position();
  req.camera_dir = level_state->get_player_camera_direction();
  req.tree = level_state->get_render_tree();
  req.camera_mode = Vertical;
  graphics_server->render_to_rect(Vec2(600, 0), Vec2(64, 720), req);

  ImGui::Begin("Level Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->level_screen_to_title_screen();
  }

  ImGui::End();
}
